#include "headers.h"
#include "ini.h"

#define CYAN_PROMPT "\e[0;36m"

void prompt()
{
    // printf("%s\n", uname);
    // printf("%s\n", sysname);
    // printf("%s\n", rootdir);

    // getting the absolute path of the current directory
    char currdir[256];
    if (getcwd(currdir, 256) == NULL)
    {
        printf("Error getting path.\n");
        return;
    }
    // printf("%s\n", currdir);

    // getting the relative path of current directory
    char relpath[256];
    strcpy(relpath, currdir + strlen(rootdir));
    if (strlen(relpath) == 0)
        strcpy(relpath, "~");

    printf(CYAN_PROMPT "<%s@%s:%s>", uname, sysname, relpath);
}
