#include "headers.h"

void fg(char *subcom)
{
    // printf("fg %s\n", subcom);
    if (strncmp(subcom, "warp", 4) == 0 && (strlen(subcom) == 4 || subcom[4] == ' ' || subcom[4] == '\t' || subcom[4] == '\n'))
        warp(subcom);
    else if (strncmp(subcom, "proclore", 8) == 0 && (strlen(subcom) == 8 || subcom[8] == ' ' || subcom[8] == '\t' || subcom[8] == '\n'))
        proclore(subcom);
    else if (strncmp(subcom, "peek", 4) == 0 && (strlen(subcom) == 4 || subcom[4] == ' ' || subcom[4] == '\t' || subcom[4] == '\n'))
        peek(subcom);
    else if (strncmp(subcom, "pastevents", 10) == 0 && (strlen(subcom) == 10 || subcom[10] == ' ' || subcom[10] == '\t' || subcom[10] == '\n'))
        pastevents(subcom);
    else if (strncmp(subcom, "seek", 4) == 0 && (strlen(subcom) == 4 || subcom[4] == ' ' || subcom[4] == '\t' || subcom[4] == '\n'))
    {
        char current[4096];
        if (getcwd(current, 4096) == NULL)
        {
            printf(ERROR_COLOR "Error getting path.\n" DEFAULT_COLOR);
            return;
        }
        // printf("curr %s\n", current);
        seek(subcom, current);
    }
    else if (strncmp(subcom, "activities", 10) == 0 && (strlen(subcom) == 10 || subcom[10] == ' ' || subcom[10] == '\t' || subcom[10] == '\n'))
        activities();
    else if (strncmp(subcom, "iMan", 4) == 0 && (strlen(subcom) == 4 || subcom[4] == ' ' || subcom[4] == '\t' || subcom[4] == '\n'))
        iman(subcom);
    else if (strncmp(subcom, "fg", 2) == 0 && (strlen(subcom) == 2 || subcom[2] == ' ' || subcom[2] == '\t' || subcom[2] == '\n'))
        bgfg(subcom);
    else if (strncmp(subcom, "bg", 2) == 0 && (strlen(subcom) == 2 || subcom[2] == ' ' || subcom[2] == '\t' || subcom[2] == '\n'))
        bgrun(subcom);
    else
    // system(subcom);
    {
        char *args[4096];
        char *y = NULL;
        char *token = strtok_r(subcom, " \t\n", &y);
        int i = 0;
        while (token != NULL)
        {
            args[i] = token;
            i++;
            token = strtok_r(NULL, " \t\n", &y);
        }
        args[i] = NULL;
        int child = fork();
        if (child == 0)
        {
            if (execvp(args[0], args) < 0)
            {
                printf(ERROR_COLOR "Invalid command.\n" DEFAULT_COLOR);
                exit(0);
            }
        }
        else
        {
            int status;
            waitpid(child, &status, WUNTRACED);
        }
    }
}