#include "headers.h"
#include "common.h"

int main()
{
    initialise(); // set the common constant values

    int i;
    for (i = 0; i < 250; i++)
        bgs[i] = (char *)malloc(4096 * sizeof(char));

    char prev[4096];
    strcpy(prev, "");

    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();

        char input[4096];
        if (fgets(input, 4096, stdin) != NULL)
        {
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
                    fg(subcom, prev);
                }
                else
                {

                    char bgcom[noa][4096]; // background commands
                    subcom = strtok_r(cmtemp, "&", &y);
                    for (i = 0; i < noa; i++)
                    {
                        strcpy(bgcom[i], subcom);
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
