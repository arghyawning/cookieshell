#include "headers.h"
#include "common.h"

void bg(char *subcom)
{
    int child = fork();

    if (child == -1)
    {
        printf(ERROR_COLOR "%s: fork fail!" DEFAULT_COLOR "\n", subcom);
        return;
    }
    else if (child == 0)
    { // child
        trimstr(subcom);
        setpgrp();
        syscom(subcom);
    }
    else
    { // parent
        printf("Child process running in the background with PID: %d\n", child);
        bgs[bgi].id = child;
        strcpy(bgs[bgi].comm, subcom);
        bgi++;
    }
}

void handle_sigchld()
{
    int status;
    int pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Process with PID %d exited normally.\n", pid);
            delete_bg(pid);
            // fflush(stdout);
        }
        else if (WIFSIGNALED(status))
        {
            printf("Process with PID %d exited due to signal.\n", pid);
            delete_bg(pid);
            // fflush(stdout);
        }
        else if (WIFSTOPPED(status))
            printf("Process with PID %d stopped.\n", pid);
        else if (WIFCONTINUED(status))
            printf("Process with PID %d continued.\n", pid);
    }
}

void delete_bg(int pid)
{
    int i;
    for (i = 0; i < bgi; i++)
    {
        if (bgs[i].id == pid)
        {
            int j;
            for (j = i; j < bgi - 1; j++)
            {
                bgs[j].id = bgs[j + 1].id;
                strcpy(bgs[j].comm, bgs[j + 1].comm);
            }
            bgi--;
            break;
        }
    }
}