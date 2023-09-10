#include "headers.h"

void bgfg(char *subcom)
{
    char *pidstr = strtok(subcom, " \t\n");
    pidstr = strtok(NULL, " \t\n");

    if (pidstr == NULL)
    {
        printf(ERROR_COLOR "Invalid command: No pid.\n" DEFAULT_COLOR);
        return;
    }
    if (strtok(NULL, " \t\n") != NULL)
    {
        printf(ERROR_COLOR "Invalid command: Too many arguments.\n" DEFAULT_COLOR);
        return;
    }

    int pid = atoi(pidstr);

    // Check if the process with the given PID exists in the background processes list
    int i;
    for (i = 0; i < bgi; i++)
    {
        if (bgs[i].id == pid)
            break;
    }
    if (i == bgi)
    {
        printf(ERROR_COLOR "Invalid command: No such process in background.\n" DEFAULT_COLOR);
        return;
    }

    // setting the signals to their default values
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    int pgid = getpgid(pid);
    int shellpgid = getpgid(0);

    // bringing the background process to the foreground
    if (tcsetpgrp(0, pgid) == -1)
        printf(ERROR_COLOR "Error moving processto foreground.\n" DEFAULT_COLOR);

    // telling the process to start executing again in the foreground
    if (kill(pid, SIGCONT) == -1)
        printf(ERROR_COLOR "Error executing process in foreground.\n" DEFAULT_COLOR);

    // parent waiting for process to execute
    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1)
        perror("Kill");

    // remove process from background processes list
    int j;
    for (j = i; j < bgi - 1; j++)
    {
        bgs[j].id = bgs[j + 1].id;
        strcpy(bgs[j].comm, bgs[j + 1].comm);
    }
    bgi--;

    // giving control of the foreground back to the terminal
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(0, shellpgid) == -1)
        printf(ERROR_COLOR "Error passing control back to shell.\n" DEFAULT_COLOR);

    // setting the signals to their default values
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}

void bgrun(char *subcom)
{
}