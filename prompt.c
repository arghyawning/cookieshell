#include "headers.h"
#include "common.h"

#define CYAN_PROMPT "\e[0;36m"
#define DEFAULT_PROMPT "\x1b[0m"

void prompt()
{
    // printf("%s\n", uname);
    // printf("%s\n", sysname);
    // printf("%s\n", rootdir);

    // getting the absolute path of the current directory
    char currdir[4096];
    if (getcwd(currdir, 4096) == NULL)
    {
        printf("Error getting path.\n");
        return;
    }

    char prom[4096];
    if (strcmp(rootdir, currdir) == 0)
        strcpy(prom, "~");
    else if (strstr(rootdir, currdir))
        strcpy(prom, currdir); // outside shell: absolute path
    else
    {
        strcpy(prom, "~");
        strcat(prom, currdir + strlen(rootdir)); // inside shell: relative path
    }

    printf(CYAN_PROMPT "<%s@%s:%s>" DEFAULT_PROMPT " ", uname, sysname, prom);
}
