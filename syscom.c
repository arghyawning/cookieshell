
#include "headers.h"
#include "common.h"

int syscom(char *input)
{
    char comm[4096];
    strcpy(comm, input);
    // printf("%s\n", comm);

    int i = 0, n;
    char *args[101];
    for (i = 0; i < 100; i++)
        args[i] = (char *)malloc(4096 * sizeof(char));

    char *arg = strtok(comm, " \n\t");
    i = 0;
    while (arg != NULL)
    {
        strcpy(args[i++], arg);
        arg = strtok(NULL, " \n\t");
    }
    args[i] = NULL;
    n = i;

    return execvp(args[0], args);
}