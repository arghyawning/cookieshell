#include "headers.h"

void proclore(char *input)
{
    char *arg = strtok(input, " \t");
    arg = strtok(NULL, " \t");

    int pid, gid;
    long int vm;
    char prstat[3];
    char execp[4096];

    char pidstr[20];
    if (arg == NULL)
    {
        pid = getpid();
        sprintf(pidstr, "%d", pid);
    }
    else
    {
        pid = atoi(arg);
        strcpy(pidstr, arg);
    }
    gid = getpgid(pid);

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    vm = usage.ru_maxrss;

    char filename[30];
    strcpy(filename, "/proc/");
    strcat(filename, pidstr);
    strcat(filename, "/status");

    int filestatus = open(filename, O_RDONLY);
    if (filestatus == -1)
    {
        printf("Error opening process information file\n");
        return;
    }

    char words[55][50];
    char filebuffer[55 * 50];
    int flag = read(filestatus, filebuffer, sizeof(filebuffer));
    if (flag == -1)
    {
        perror("Error reading process information file\n");
        close(filestatus);
        return;
    }

    char *word = strtok(filebuffer, " \t\n");
    while (word != NULL)
    {
        if (strcmp(word, "State:") == 0)
        {
            word = strtok(NULL, " \t\n");
            strcpy(prstat, word);
            break;
        }
        word = strtok(NULL, " \t\n");
    }

    printf("%s\n", filename);
    strcpy(filename + strlen(filename) - strlen("status"), "exe");
    flag = readlink(filename, execp, sizeof(execp));
    if (flag < 0)
    {
        printf("Error getting executable path\n");
        return;
    }

    printf("pid: %d\n", pid);
    printf("Process Status: %s\n", prstat);
    printf("Process Group : %d\n", gid);
    printf("Virtual Memory: %ldKB\n", vm);
    printf("Executable path: %s\n", execp);
}