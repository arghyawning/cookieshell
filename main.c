#include "headers.h"
#include "common.h"

int bgi = 0;
struct backproc *bgs = NULL;

char *pastfile = NULL;
FILE *pef = NULL;
int nope = 0;

char *prev = NULL;

int main()
{
    initialise(); // set the common constant values

    prev = (char *)(malloc(sizeof(char) * 4096));
    strcpy(prev, "");

    pastfile = (char *)(malloc(sizeof(char) * 4096));
    strcpy(pastfile, rootdir);
    strcat(pastfile, "/pastevents.txt");

    pef = fopen(pastfile, "r+");
    if (pef != NULL)
    //     perror(ERROR_COLOR "No past event found" DEFAULT_COLOR "\n");
    // else
    {
        char line[10000];
        while (fgets(line, sizeof(line), pef))
            nope++;
        fclose(pef);
    }
    printf("nope=%d\n", nope);

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

        char input[10000];
        if (fgets(input, 10000, stdin) != NULL)
        {
            handle_sigchld();

            if (input[strlen(input) - 1] == '\n')
                input[strlen(input) - 1] = '\0';

            updatepastevents(input);

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
                    // updatepastevents(input);
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
                        // updatepastevents();
                        fflush(stdout);
                        exit(0);
                    }

                    // start = clock();
                    time(&start);
                    fg(subcom);
                    time(&end);
                    // end = clock();
                    time_taken = ((double)(end - start));
                    // printf("fg took time %lf\n", time_taken);
                    if (time_taken > 2)
                    {
                        pflag = (int)time_taken;
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
                            updatepastevents(input);
                            fflush(stdout);
                            exit(0);
                        }
                        subcom = strtok_r(NULL, "&", &y);
                    }

                    for (i = 0; i < noa; i++)
                        bg(bgcom[i]);

                    if (subcom != NULL)
                    {
                        trimstr(subcom);
                        // start = clock();
                        time(&start);
                        fg(subcom);
                        time(&end);
                        // end = clock();
                        time_taken = ((double)(end - start));
                        // printf("fg took time %lf\n", time_taken);
                        if (time_taken > 2)
                        {
                            pflag = (int)time_taken;
                            strcpy(pcom, subcom);
                        }
                    }
                    // fg(subcom);
                }

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
