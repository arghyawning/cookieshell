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
    char stat[10];
    strcpy(stat, "Running");
    qsort(bgs, bgi, sizeof(struct backproc), compareComm);

    int i = 0;
    for (i = 0; i < bgi; i++)
    {
        printf("%d : %s - %s\n", bgs[i].id, bgs[i].comm, stat);
    }
}