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
        // setsid();
        int flag = syscom(subcom);
        if (flag == -1)
            printf(ERROR_COLOR "Erorr executing %s as a system command!" DEFAULT_COLOR "\n", subcom);
        else
        {
            if (WIFEXITED(flag))
            {
                int exit = WEXITSTATUS(flag); // 0 if successful
                if (exit == 0)
                    printf("%s executed successfully.\n", subcom);
                else
                    printf(ERROR_COLOR "%s executed with non-zero exit status: %d\n" DEFAULT_COLOR, subcom, exit);
            }
            else if (WIFSIGNALED(flag))
                printf(ERROR_COLOR "%s terminated by signal.\n" DEFAULT_COLOR, subcom);
        }
    }
    else
    { // parent
        printf("Child process running in the background with PID: %d\n", child);
    }
}