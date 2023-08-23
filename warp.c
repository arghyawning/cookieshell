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
        printf("Error getting path.\n");

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
                printf("???%s\n", word);
                char relpath[4096];
                if (word[0] != '.' && word[0] != '~')
                {
                    strcpy(relpath, "./");
                    strcat(relpath, word);
                }
                else
                {
                    strcpy(relpath, word);
                }
                int flag = chdir(word);
                if (flag)
                    perror("chdir");
                else
                {
                    char tempdir[4096];
                    if (getcwd(tempdir, 4096) == NULL)
                        printf("Error getting path.\n");
                    if (strcmp(tempdir, currdir) != 0)
                    {
                        strcpy(prev, currdir);
                        strcpy(currdir, tempdir);
                    }
                }
                /*
                int parflag = 0;
                char relpath[4096];
                // printf("%s\n", word);
                if (word[0] == '~' || (word[0] == '.' && word[1] != '.'))
                { // the case where relative filepath is the argument
                    strcpy(relpath, word);
                    if (word[0] == '~')
                        relpath[0] = '.';
                    memmove(word, word + 2, strlen(word) - 1);
                }
                else if (strncmp(word, "../", 3) == 0)
                {
                    parflag = 1;

                    // the case where relative filepath is the argument and its in the parent directory
                    char *last = strrchr(currdir, '/');
                    int len = 1 + last - currdir;
                    if (len <= 1)
                        parflag = 2;
                    strcpy(relpath, word);
                }
                else
                {
                    strcpy(relpath, "./");
                    strcat(relpath, word);
                }
                // printf("pf: %d\n", parflag);

                if (parflag != 2)
                {

                    int flag = chdir(relpath);
                    if (flag)
                        perror("chdir");
                    else
                    {
                        strcpy(prev, currdir);
                        if (parflag == 1)
                        {
                            if (currdir[strlen(currdir) - 1] == '/')
                                currdir[strlen(currdir) - 1] = '\0';
                            char *last = strrchr(currdir, '/');
                            int len = last - currdir;
                            currdir[len] = '\0';
                            strcat(currdir, word + 2);
                        }
                        else
                        {
                            if (currdir[strlen(currdir) - 1] != '/')
                                strcat(currdir, "/");
                            strcat(currdir, word);
                        }
                    }
                }
                */
                printf("%s\n", currdir);
            }
        }

        word = strtok(NULL, " \t");
    }
}