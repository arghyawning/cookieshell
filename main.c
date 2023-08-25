#include "headers.h"
#include "common.h"

#include <math.h>

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

    int pflag = 0;
    char pcom[4096];
    strcpy(pcom, "");

    // Keep accepting commands
    while (1)
    {
        // signal(SIGCHLD, handle_sigchld);
        setbuf(stdout, NULL);

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(pflag, pcom);
        pflag = 0;
        strcpy(pcom, "");

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

                // clock_t start, end;
                time_t start, end;
                double time_taken;

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

                    // start = clock();
                    time(&start);
                    fg(subcom, prev);
                    time(&end);
                    // end = clock();
                    time_taken = ((double)(end - start));
                    // printf("fg took time %lf\n", time_taken);
                    if (time_taken > 2)
                    {
                        pflag = (int)ceil(time_taken);
                        strcpy(pcom, subcom);
                    }
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

                    for (i = 0; i < noa; i++)
                        bg(bgcom[i]);

                    if (subcom != NULL)
                    {
                        // start = clock();
                        time(&start);
                        fg(subcom, prev);
                        time(&end);
                        // end = clock();
                        time_taken = ((double)(end - start));
                        // printf("fg took time %lf\n", time_taken);
                        if (time_taken > 2)
                        {
                            pflag = (int)ceil(time_taken);
                            strcpy(pcom, subcom);
                        }
                    }
                    // fg(subcom, prev);
                }

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
