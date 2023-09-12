#include "headers.h"
#include "common.h"

void ctrlz()
{
    if (currfgid != -1)
    {
        bgs[bgi].id = currfgid;
        strcpy(bgs[bgi].comm, currfgcom);
        bgi++;

        setpgid(currfgid, currfgid);
        kill(currfgid, SIGTSTP);
        printf("Process with PID %d stopped.\n", currfgid);
        currfgid = -1;
    }
}

void ctrlc()
{
    // printf("%d\n", currfgid);
    if (currfgid != -1)
    {
        // printf("%d\n", currfgid);
        kill(currfgid, SIGINT);
        currfgid = -1;
    }
    // printf("ok\n");
}

void ctrld()
{
    // kill all background processes
    int i, j;
    for (i = 0; i < bgi; i++)
        kill(bgs[i].id, SIGKILL);
    bgi = 0;
    printf("\nbye :(");
    exit(0);
}

void signals(char *subcom)
{
    char *token = strtok(subcom, " \t");
    token = strtok(NULL, " \t");
    if (token == NULL)
    {
        printf(ERROR_COLOR "Too less arguments\n" DEFAULT_COLOR);
        return;
    }
    int pid = atoi(token);
    token = strtok(NULL, " \t");
    if (token == NULL)
    {
        printf(ERROR_COLOR "Too less arguments\n" DEFAULT_COLOR);
        return;
    }

    int signum = atoi(token) % 32;
    token = strtok(NULL, " \t");
    if (token != NULL)
    {
        printf(ERROR_COLOR "Too many arguments\n" DEFAULT_COLOR);
        return;
    }

    if (kill(pid, signum) < 0)
    {
        printf(ERROR_COLOR "Error in sending signal\n" DEFAULT_COLOR);
        return;
    }

    // // remove from bgs
    // int i;
    // for (i = 0; i < bgi; i++)
    // {
    //     if (bgs[i].id == pid)
    //     {
    //         int j;
    //         for (j = i; j < bgi - 1; j++)
    //         {
    //             bgs[j].id = bgs[j + 1].id;
    //             strcpy(bgs[j].comm, bgs[j + 1].comm);
    //         }
    //         bgi--;
    //         break;
    //     }
    // }

    return;
}