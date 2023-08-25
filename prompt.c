#include "headers.h"
#include "common.h"

void prompt(int pflag, char *pcom)
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

    char *command = strtok(pcom, " \n\t");

    // printf(CYAN_PROMPT "<%s@%s:" PURPLE_PROMPT "%s" CYAN_PROMPT ">" DEFAULT_PROMPT " ", uname, sysname, prom);
    printf(CYAN_PROMPT "<%s@%s:" PURPLE_PROMPT "%s", uname, sysname, prom);
    if (pflag != 0)
        printf(YELLOW_PROMPT " %s: %ds", command, pflag);
    printf(CYAN_PROMPT ">" DEFAULT_PROMPT " ");
}
