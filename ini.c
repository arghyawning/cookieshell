#include "ini.h"

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