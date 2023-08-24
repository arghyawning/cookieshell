#include "common.h"

char uname[256];
char sysname[256];
char rootdir[4096];

void initialise()
{
    // getting the username
    if (getlogin_r(uname, 256) != 0)
    {
        printf("Error getting username.\n");
        return;
    }

    // getting the system name
    if (gethostname(sysname, 256) != 0)
    {
        printf("Error getting system name.\n");
        return;
    }

    // getting the absolute path of the root directory
    if (getcwd(rootdir, 4096) == NULL)
    {
        printf("Error getting path.\n");
        return;
    }
}

// Function to remove leading and trailing whitespace, tabs, and newlines
void trimstr(char *word)
{
    if (word == NULL)
        return;

    // Find the first non-whitespace character
    char *start = word;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n'))
        ++start;

    // Find the last non-whitespace character
    char *end = start + strlen(start) - 1;
    while (end >= start && (*end == ' ' || *end == '\t' || *end == '\n'))
        --end;

    // Move the non-whitespace portion to the beginning of the string
    size_t length = end - start + 1;
    memmove(word, start, length);
    word[length] = '\0';
}