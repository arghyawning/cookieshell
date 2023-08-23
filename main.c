#include "headers.h"
#include "common.h"

// Function to remove leading and trailing whitespace, tabs, and newlines
void trimstr(char *word)
{
    if (word == NULL)
    {
        return;
    }

    // Find the first non-whitespace character
    char *start = word;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n'))
    {
        ++start;
    }

    // Find the last non-whitespace character
    char *end = start + strlen(start) - 1;
    while (end >= start && (*end == ' ' || *end == '\t' || *end == '\n'))
    {
        --end;
    }

    // Move the non-whitespace portion to the beginning of the string
    size_t length = end - start + 1;
    memmove(word, start, length);
    word[length] = '\0';
}

int main()
{
    initialise(); // set the common constant values

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

            char *command = strtok_r(temp, ";", &x);
            while (command != NULL)
            {
                char cmtemp[strlen(command) + 1];
                strcpy(cmtemp, command);

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
                // memset(y, '\0', sizeof(y));
                strcpy(cmtemp, command);

                if (noa == 0)
                {
                    subcom = strtok_r(cmtemp, "&", &y);
                    trimstr(subcom);
                    // printf("just <%s>\n", subcom);
                    if (strncmp(subcom, "warp", 4) == 0 && (strlen(subcom) == 4 || subcom[4] == ' ' || subcom[4] == '\t' || subcom[4] == '\n'))
                        warp(subcom, prev);
                    else if (strncmp(subcom, "proclore", 8) == 0 && (strlen(subcom) == 8 || subcom[8] == ' ' || subcom[8] == '\t' || subcom[8] == '\n'))
                        proclore(subcom);
                    else if (strncmp(subcom, "peek", 4) == 0 && (strlen(subcom) == 4 || subcom[4] == ' ' || subcom[4] == '\t' || subcom[4] == '\n'))
                        peek(subcom);
                    else
                        printf(ERROR_COLOR "%s is not a valid command!" DEFAULT_COLOR "\n", subcom);
                }
                else
                {

                    char bgcom[noa][4096]; // background commands
                    subcom = strtok_r(cmtemp, "&", &y);
                    for (int i = 0; i < noa; i++)
                    {
                        strcpy(bgcom[i], subcom);
                        subcom = strtok_r(NULL, "&", &y);
                    }
                    printf("bg commands:  ");
                    for (int i = 0; i < noa; i++)
                        printf("%s\t", bgcom[i]);
                    printf("\nfg command: %s\n", subcom);
                }
                // printf("\n");

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
