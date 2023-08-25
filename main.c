#include "headers.h"
#include "common.h"

int bgi = 0;
struct backproc *bgs = NULL;

int main()
{
    initialise(); // set the common constant values

    char prev[4096];
    strcpy(prev, "");

    int i;

    bgs = (struct backproc *)malloc(sizeof(struct backproc) * 250);

    // signal(SIGCHLD, handle_sigchld);

    // Keep accepting commands
    while (1)
    {
        // signal(SIGCHLD, handle_sigchld);
        setbuf(stdout, NULL);

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();

        char input[4096];
        if (fgets(input, 4096, stdin) != NULL)
        {
            handle_sigchld();
            if (input[strlen(input) - 1] == '\n')
                input[strlen(input) - 1] = '\0';

            // tokenising the input
            char temp[strlen(input) + 1];
            strcpy(temp, input);

            char *x;
            char *y;

            // printf("input: %s\n", input);
            char *command = strtok_r(temp, ";", &x);
            while (command != NULL)
            {
                if (strcmp(command, "bye") == 0)
                {
                    printf("bye :(\n");
                    fflush(stdout);
                    exit(0);
                }
                char cmtemp[strlen(command) + 1];
                strcpy(cmtemp, command);

                if (command[strlen(cmtemp) - 1] == '&')
                    strcat(cmtemp, " ");

                int noa = -1; // number of ampersands
                char *subcom = strtok_r(cmtemp, "&", &y);
                trimstr(subcom);
                // printf("%s\t", subcom);

                while (subcom != NULL)
                {
                    noa++;
                    subcom = strtok_r(NULL, "&", &y);
                }

                memset(cmtemp, '\0', sizeof(cmtemp));
                char *y = NULL;
                strcpy(cmtemp, command);
                // printf("noa=%d\n", noa);

                if (noa == 0)
                {
                    subcom = strtok_r(cmtemp, "&", &y);
                    trimstr(subcom);
                    if (strcmp(subcom, "bye") == 0)
                    {
                        printf("bye :(\n");
                        fflush(stdout);
                        exit(0);
                    }
                    fg(subcom, prev);
                }
                else
                {

                    char bgcom[noa][4096]; // background commands
                    subcom = strtok_r(cmtemp, "&", &y);
                    for (i = 0; i < noa; i++)
                    {
                        strcpy(bgcom[i], subcom);
                        if (strcmp(subcom, "bye") == 0)
                        {
                            printf("bye :(\n");
                            fflush(stdout);
                            exit(0);
                        }
                        subcom = strtok_r(NULL, "&", &y);
                    }
                    // printf("bg commands:  ");
                    for (i = 0; i < noa; i++)
                        bg(bgcom[i]);
                    // printf("%s\t", bgcom[i]);
                    // printf("\n");
                    if (subcom != NULL)
                        fg(subcom, prev);
                    // printf("fg command: %s\n", subcom);
                }
                // printf("\n");

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
