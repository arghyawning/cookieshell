
#include "headers.h"
#include "common.h"

void syscom(char *input)
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

    int flag = execvp(args[0], args);
    if (flag == -1)
        printf(ERROR_COLOR "Erorr executing %s as a system command!" DEFAULT_COLOR "\n", input);
    else
    {
        if (WIFEXITED(flag))
        {
            int exit = WEXITSTATUS(flag); // 0 if successful
            if (exit == 0)
                printf("%s executed successfully.\n", input);
            else
                printf(ERROR_COLOR "%s executed with non-zero exit status: %d\n" DEFAULT_COLOR, input, exit);
        }
        else if (WIFSIGNALED(flag))
            printf(ERROR_COLOR "%s terminated by signal.\n" DEFAULT_COLOR, input);
    }
}