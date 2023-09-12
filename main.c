#include "headers.h"
#include "common.h"

int bgi = 0;
struct backproc *bgs = NULL;

char *pastfile = NULL;
FILE *pef = NULL;
int nope = 0;

char *prev = NULL;

int currfgid = -1;
char *currfgcom = NULL;

int pflag = 0;
char *pcom = NULL;

int zflag = 0;

int main()
{
    initialise(); // set the common constant values

    prev = (char *)(malloc(sizeof(char) * 4096));
    strcpy(prev, "");

    pastfile = (char *)(malloc(sizeof(char) * 4096));
    strcpy(pastfile, rootdir);
    strcat(pastfile, "/pastevents.txt");

    currfgcom = (char *)(malloc(sizeof(char) * 4096));

    pef = fopen(pastfile, "r+");
    if (pef != NULL)
    {
        char line[10000];
        while (fgets(line, sizeof(line), pef))
            nope++;
        fclose(pef);
    }
    // printf("nope=%d\n", nope);

    int i;

    bgs = (struct backproc *)malloc(sizeof(struct backproc) * 250);

    // int pflag = 0;
    // char pcom[4096];
    pcom = (char *)(malloc(sizeof(char) * 4096));
    strcpy(pcom, "");

    signal(SIGINT, ctrlc);
    // signal(SIGTSTP, ctrlz);

    struct sigaction new_action, old_action;

    signal(SIGTSTP, ctrlz);
    new_action.sa_handler = ctrlz;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGTSTP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGTSTP, &new_action, NULL);

    // Keep accepting commands
    while (1)
    {
        setbuf(stdout, NULL);

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(pflag, pcom);
        pflag = 0;
        strcpy(pcom, "");
        // currfgid = -1;

        char input[10000];
        if (fgets(input, 10000, stdin) == NULL)
        {
            fflush(stdout);
            ctrld();
        }
        else
        {
            handle_sigchld();

            if (input[strlen(input) - 1] == '\n')
                input[strlen(input) - 1] = '\0';

            updatepastevents(input);

            if (input == NULL || strlen(input) == 0)
                continue;

            // tokenising the input
            char temp[strlen(input) + 1];
            strcpy(temp, input);

            char *x;

            // printf("input: %s\n", input);
            char *command = strtok_r(temp, ";", &x);
            while (command != NULL)
            {
                if (strcmp(command, "bye") == 0)
                {
                    printf("bye ");
                    sleep(1);
                    printf(":(\n");
                    sleep(1);
                    fflush(stdout);
                    exit(0);
                }

                // printf("command: %s\n", command);

                // piping

                trimstr(command);
                if (command[strlen(command) - 1] == '|')
                {
                    printf("Invalid use of pipes\n");
                    command = strtok_r(NULL, ";", &x);
                    continue;
                }

                int in = dup(STDIN_FILENO);
                int out = dup(STDOUT_FILENO);

                char pipecomm[4096];
                strcpy(pipecomm, command);
                char *pp;
                char *p = strtok_r(pipecomm, "|", &pp);
                char *commands[10];
                int numcom = 0;

                while (p != NULL)
                {
                    commands[numcom] = malloc(sizeof(char) * (strlen(p) + 1));
                    strcpy(commands[numcom], p);
                    trimstr(commands[numcom]);
                    numcom++;
                    p = strtok_r(NULL, "|", &pp);
                }

                if (numcom >= 2)
                {
                    int pipes[numcom - 1][2];
                    for (i = 0; i < numcom; i++)
                    {
                        char pipingcomm[4096];
                        strcpy(pipingcomm, commands[i]);
                        // printf("pipingcomm: %s\n", pipingcomm);

                        if (i != numcom - 1)
                        {
                            pipe(pipes[i]);
                            // printf("%d\n", pipes[i][1]);
                            dup2(pipes[i][1], STDOUT_FILENO);
                        }
                        else
                            dup2(out, STDOUT_FILENO);

                        if (i > 0)
                            dup2(pipes[i - 1][0], STDIN_FILENO);

                        runcom(pipingcomm, input);

                        close(pipes[i - 1][0]);
                        close(pipes[i][1]);
                    }
                    dup2(out, STDOUT_FILENO);
                    dup2(in, STDIN_FILENO);
                }

                else
                    runcom(command, input);
                // printf("command: %s\n\n", command);

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
