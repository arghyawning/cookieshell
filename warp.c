#include "headers.h"
#include "common.h"

void warp(char *input, char *prev)
{
    char temp[strlen(input) + 1];
    strcpy(temp, input);
    char *word = strtok(temp, " \t");
    word = strtok(NULL, " \t");

    char currdir[4096];
    if (getcwd(currdir, 4096) == NULL)
        printf(ERROR_COLOR "Error getting path." DEFAULT_COLOR "\n");

    // just stay
    if (word == NULL)
    {
        if (strcmp(currdir, rootdir) != 0)
        {
            strcpy(prev, currdir);
            chdir(rootdir);
        }
        printf("%s\n", rootdir);
    }

    while (word != NULL)
    {
        if (word[strlen(word) - 1] == '\n')
            word[strlen(word) - 1] = '\0';

        // home directory of shell
        if (strcmp(word, "~") == 0) // shell home
        {
            if (strcmp(currdir, rootdir) != 0)
            {
                strcpy(prev, currdir);
                chdir(rootdir);
            }
            printf("%s\n", rootdir);
        }
        else if (strcmp(word, ".") == 0) // stay
            printf("%s\n", currdir);
        else if (strcmp(word, "..") == 0 || strcmp(word, "../") == 0) // parent
        {
            if (strcmp(currdir, "/") == 0)
                printf("%s\n", currdir);
            else
            {
                if (currdir[strlen(currdir) - 1] == '/')
                    currdir[strlen(currdir) - 1] = '\0';

                if (word[strlen(word) - 1] == '/')
                    word[strlen(word) - 1] = '\0';

                char *last = strrchr(currdir, '/');
                int len = 1 + last - currdir;
                if (len > 1)
                    strcpy(prev, currdir);
                // strncpy(currdir, currdir, len);
                currdir[len] = '\0';
                // printf("should be: %s\n", currdir);
                chdir(currdir);

                char disp[strlen(currdir)];
                strcpy(disp, currdir);
                if (strlen(currdir) > 1)
                    disp[strlen(currdir) - 1] = '\0';
                printf("%s\n", disp);
            }
        }
        // previous directory
        else if (strcmp(word, "-") == 0)
        {
            if (strlen(prev) == 0)
                printf("OLDPWD not set\n");
            else
            {
                printf("%s\n", prev);
                chdir(prev);
            }
        }
        // go to a particular directory
        else
        {
            // absolute path
            if (word[0] == '/')
            {
                // printf("eh %s\n", word);
                int flag = chdir(word);
                if (flag)
                    perror("chdir");
                else
                {
                    strcpy(prev, currdir);
                    strcpy(currdir, word);
                }
                printf("%s\n", currdir);
            }
            // relative path
            else
            {
                char relpath[4096];
                if (word[0] != '.' && word[0] != '~')
                {
                    strcpy(relpath, "./");
                    strcat(relpath, word);
                }
                else
                    strcpy(relpath, word);
                int flag = chdir(word);
                if (flag)
                    perror("chdir");
                else
                {
                    char tempdir[4096];
                    if (getcwd(tempdir, 4096) == NULL)
                        printf(ERROR_COLOR "Error getting path." DEFAULT_COLOR "\n");
                    if (strcmp(tempdir, currdir) != 0)
                    {
                        strcpy(prev, currdir);
                        strcpy(currdir, tempdir);
                    }
                }
                printf("%s\n", currdir);
            }
        }

        word = strtok(NULL, " \t");
    }
}