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
            // char *word = strtok(temp, " ");
            char *word = strtok(temp, " \t");

            // echo
            if (strcmp(word, "echo") == 0)
            {
                word = strtok(NULL, " \t");
                while (word != NULL)
                {
                    if (word[strlen(word) - 1] == '\n')
                        printf("%s", word);
                    else
                        printf("%s ", word);
                    word = strtok(NULL, " \t");
                }
            }

            // warp
            else if (strcmp(word, "warp") == 0)
                warp(input, prev);

            // proclore
            else if (strcmp(word, "proclore") == 0)
                proclore(input);

            else
            {
                printf("%s is not a valid command\n", word);
            }
        }
    }

    return 0;
}
