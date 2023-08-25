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
    else
        system(subcom);
}