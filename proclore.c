#include "headers.h"

void proclore(char *command)
{
    char *arg = strtok(command, " \t");
    arg = strtok(NULL, " \t");

    int pid, gid;
    long int vm;
    char execpath[4096];
    char fbg = ' ';

    // getting pid
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

    // getting process group
    gid = getpgid(pid);

    char procstat[30];
    strcpy(procstat, "/proc/");
    strcat(procstat, pidstr);
    strcat(procstat, "/stat");
    FILE *file = fopen(procstat, "r");
    int count = 0, tcpgrp;
    char buffer[16]; // Adjust the buffer size as needed

    while (fscanf(file, "%s", buffer) != EOF)
    {
        count++;
        if (count == 8)
        {
            tcpgrp = atoi(buffer);
            break;
        }
    }
    printf("tcpgrp: %d\n", tcpgrp);
    fclose(file);

    if (pid == tcpgrp)
        fbg = '+'; // foreground process

    // getting vm
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    vm = usage.ru_maxrss;

    // getting status
    char filename[30];
    strcpy(filename, "/proc/");
    strcat(filename, pidstr);
    strcat(filename, "/status");

    int filestatus = open(filename, O_RDONLY);
    if (filestatus == -1)
    {
        perror("open");
        return;
    }

    char filebuffer[4096];
    int flag = read(filestatus, filebuffer, sizeof(filebuffer));
    if (flag == -1)
    {
        perror("read");
        close(filestatus);
        return;
    }
    close(filestatus);

    char *line = strstr(filebuffer, "State:");
    char prstatus = line[strlen("State:") + 1];

    // getting exec path
    strcpy(filename + strlen(filename) - strlen("status"), "exe");
    flag = readlink(filename, execpath, sizeof(execpath));
    if (flag < 0)
    {
        printf("Error getting executable path\n");
        return;
    }

    printf("pid: %d\n", pid);
    printf("Process Status: %c%c\n", prstatus, fbg);
    printf("Process Group : %d\n", gid);
    printf("Virtual Memory: %ldKB\n", vm);
    printf("Executable path: %s\n", execpath);
}