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
    // printf("coming\n");

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
            // char *inpworedir = strtok(temp, "<>");
            // strcpy(temp, inpworedir);
            // printf("inp w/o redir=%s\n", temp);

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

                // printf("command: %s\t%s", command, temp);
                // printf("go\n");
                runcom(command, temp, input);
                // printf("command: %s\n\n", command);

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
