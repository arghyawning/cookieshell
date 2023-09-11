#include "headers.h"
#include "common.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

int pidverify(const char *name)
{
    int l = strlen(name);
    for (int i = 0; i < l; i++)
    {
        if (!isdigit(name[i]))
            return 0;
    }
    return 1;
}

int kbhit()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void neonate(char *subcom)
{
    // printf("%s\n", subcom);

    char *token = strtok(subcom, " \t\n");
    token = strtok(NULL, " \t\n");
    if (token == NULL)
    {
        printf(ERROR_COLOR "Too less arguments for neonate\n" DEFAULT_COLOR);
        return;
    }

    // printf("Flag is %s\n", token);
    if (strcmp(token, "-n") != 0)
    {
        printf(ERROR_COLOR "Invalid flag for neonate\n" DEFAULT_COLOR);
        return;
    }

    token = strtok(NULL, " \t\n");
    if (token == NULL)
    {
        printf(ERROR_COLOR "Too less arguments for neonate\n" DEFAULT_COLOR);
        return;
    }
    int timearg = atoi(token);
    // printf("%s seconds\n", token);

    token = strtok(NULL, " \t\n");
    if (token != NULL)
    {
        printf(ERROR_COLOR "Too many arguments for neonate\n" DEFAULT_COLOR);
        return;
    }

    if (timearg <= 0)
    {
        printf(ERROR_COLOR "Invalid time argument for neonate\n" DEFAULT_COLOR);
        return;
    }
    // printf("Time argument is %d\n", timearg);

    enableRawMode();

    // temporarily block signals
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    while (1)
    {
        int maxtime = 0;
        int maxpid = -2;

        DIR *dir;
        struct dirent *entry;

        // Open the /proc directory
        dir = opendir("/proc");
        if (dir == NULL)
        {
            // perror("opendir");
            printf(ERROR_COLOR "Failed to open /proc directory\n" DEFAULT_COLOR);
            return;
        }

        // Iterate through the directory entries
        while ((entry = readdir(dir)) != NULL)
        {
            if (pidverify(entry->d_name))
            {
                // printf("PID folder: %s\n", entry->d_name);
                char fname[4096];
                sprintf(fname, "/proc/%s/stat", entry->d_name);

                FILE *statfile = fopen(fname, "r");
                if (statfile == NULL)
                {
                    // printf(ERROR_COLOR "Failed to open stat file for PID %s\n" DEFAULT_COLOR, entry->d_name);
                    // exit(EXIT_FAILURE);
                    continue;
                }

                char *line = NULL;
                size_t len = 0;
                ssize_t read;

                if ((read = getline(&line, &len, statfile)) != -1)
                {
                    char *token = strtok(line, " ");
                    int count = 0;
                    unsigned long long frtime;

                    while (token != NULL)
                    {
                        if (count == 21) // man proc for details
                        {
                            frtime = strtoul(token, NULL, 10);
                            break;
                        }

                        count++;
                        token = strtok(NULL, " ");
                    }

                    if (frtime > maxtime)
                    {
                        maxtime = frtime;
                        maxpid = atoi(entry->d_name);
                    }
                }
                // else
                //     printf(ERROR_COLOR "Failed to read stat file for PID %s\n" DEFAULT_COLOR, entry->d_name);

                free(line);
                fclose(statfile);
            }
        }
        closedir(dir);

        // printf("Most recent process has pid %d with starting time %d\n", maxpid, maxtime);
        printf("%d\n", maxpid);

        // check for key hit
        char key;
        if (kbhit())
        {
            key = getchar();
            if (key == 'x')
            {
                // printf("Exiting neonate\n");
                break;
            }
        }

        sleep(timearg);
    }

    // unblock signals
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    disableRawMode();
}