#include "headers.h"
#include "common.h"

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

                char *word = strtok_r(cmtemp, " \t", &y);
                // echo
                if (strcmp(word, "echo") == 0)
                {
                    word = strtok_r(NULL, " \t", &y);
                    while (word != NULL)
                    {
                        if (word[strlen(word) - 1] == '\n')
                            printf("%s", word);
                        else
                            printf("%s ", word);
                        word = strtok_r(NULL, " \t", &y);
                    }
                }

                // warp
                else if (strcmp(word, "warp") == 0)
                    warp(command, prev);

                // proclore
                else if (strcmp(word, "proclore") == 0)
                    proclore(command);

                // peek
                else if (strcmp(word, "peek") == 0)
                    peek(command);

                else
                {
                    printf("%s is not a valid command\n", word);
                }

                command = strtok_r(NULL, ";", &x);
            }
        }
    }

    return 0;
}
