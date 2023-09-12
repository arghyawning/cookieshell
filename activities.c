#include "headers.h"
#include "common.h"

int compareComm(const void *a, const void *b)
{
    const struct backproc *procA = (const struct backproc *)a;
    const struct backproc *procB = (const struct backproc *)b;
    return strcmp(procA->comm, procB->comm);
}

void activities()
{
    char stat[20];
    qsort(bgs, bgi, sizeof(struct backproc), compareComm);

    int i, status;
    for (i = 0; i < bgi; i++)
    {
        int pid = waitpid(bgs[i].id, &status, WNOHANG | WUNTRACED);
        // printf("%d\n", pid);

        if (WIFSTOPPED(status))
            strcpy(stat, "Stopped");
        else
            strcpy(stat, "Running");

        printf("%d : %s - %s\n", bgs[i].id, bgs[i].comm, stat);
    }
}