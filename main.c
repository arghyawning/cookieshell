#include "headers.h"
#include "common.h"

int bgi = 0;
struct backproc *bgs = NULL;

char *pastfile = NULL;
FILE *pef = NULL;
int nope = 0;

char *prev = NULL;

int currfgid = -1;
char *currfgcom = NULL;

int main()
{
    initialise(); // set the common constant values

    prev = (char *)(malloc(sizeof(char) * 4096));
    strcpy(prev, "");

    pastfile = (char *)(malloc(sizeof(char) * 4096));
    strcpy(pastfile, rootdir);
    strcat(pastfile, "/pastevents.txt");

    currfgcom = (char *)(malloc(sizeof(char) * 4096));

    pef = fopen(pastfile, "r+");
    if (pef != NULL)
    {
        char line[10000];
        while (fgets(line, sizeof(line), pef))
            nope++;
        fclose(pef);
    }
    // printf("nope=%d\n", nope);

    int i;

    bgs = (struct backproc *)malloc(sizeof(struct backproc) * 250);

    int pflag = 0;
    char pcom[4096];
    strcpy(pcom, "");

    signal(SIGINT, ctrlc);
    // signal(SIGTSTP, ctrlz);

    // struct sigaction sigact;
    // sigact.sa_handler = ctrlz;
    // sigemptyset(&sigact.sa_mask);
    // sigact.sa_flags = 0;
    // sigaction(SIGTSTP, &sigact, NULL);

    struct sigaction new_action, old_action;

    signal(SIGTSTP, ctrlz);
    new_action.sa_handler = ctrlz;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGTSTP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGTSTP, &new_action, NULL);

    // Keep accepting commands
    while (1)
    {
        setbuf(stdout, NULL);

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(pflag, pcom);
        pflag = 0;
        strcpy(pcom, "");
        // currfgid = -1;

        char input[10000];
        if (fgets(input, 10000, stdin) == NULL)
            ctrld();
        else
        {
            handle_sigchld();

            if (input[strlen(input) - 1] == '\n')
                input[strlen(input) - 1] = '\0';

            updatepastevents(input);

            // checking for i/o redirection
            int iored = 0; /*
             0 for no redirection,
             1 for input redirection,
             2 for output redirection (w),
             3 for output redirection (a),
             4 for both input and output redirection (w),
             5 for both input and output redirection (a)*/
            char inputcopy[10000];
            strcpy(inputcopy, input);
            if (strstr(inputcopy, "<<") != NULL)
            {
                printf(ERROR_COLOR "Error: Invalid redirection type\n" DEFAULT_COLOR);
                continue;
            }
            char iredf[4096];
            char oredf[4096];
            strcpy(iredf, "");
            strcpy(oredf, "");
            char *iort;
            char *redtemp = strstr(inputcopy, ">>");
            if (redtemp == NULL)
            {
                // printf("redtemp is null\n");
                redtemp = strstr(inputcopy, ">");
                if (redtemp == NULL)
                {
                    // printf("redtemp is null\n");
                    redtemp = strstr(inputcopy, "<");
                    if (redtemp != NULL)
                    {
                        iored = 1;
                        strcpy(iredf, redtemp + 1);
                        trimstr(iredf);
                    }
                }
                else
                {
                    iored = 2;

                    // before and after >
                    char before[4096];
                    char after[4096];
                    char *bef = strtok_r(inputcopy, ">", &iort);
                    strcpy(before, bef);
                    strcpy(after, redtemp + 1);
                    trimstr(after);
                    trimstr(before);
                    // printf("%s, %s\n", before, after);

                    char tt[4096];
                    strcpy(tt, after);
                    iort = NULL;
                    bef = strtok_r(tt, "<", &iort);
                    strcpy(oredf, bef);

                    // checking for input redirection before and after >
                    redtemp = strstr(before, "<");
                    if (redtemp == NULL)
                    {
                        // printf("redtemp is null\n");
                        redtemp = strstr(after, "<");
                        if (redtemp != NULL)
                        {
                            iored = 4;
                            strcpy(iredf, redtemp + 1);
                            trimstr(iredf);
                        }
                    }
                    else
                    {
                        iored = 4;
                        strcpy(iredf, redtemp + 1);
                        trimstr(iredf);
                    }
                }
            }
            else
            {
                iored = 3;

                // before and after >>
                char before[4096];
                char after[4096];
                char *bef = strtok_r(inputcopy, ">", &iort);
                strcpy(before, bef);
                strcpy(after, redtemp + 2);
                trimstr(after);
                trimstr(before);
                // printf("%s, %s\n", before, after);

                char tt[4096];
                strcpy(tt, after);
                iort = NULL;
                bef = strtok_r(tt, "<", &iort);
                strcpy(oredf, bef);

                // checking for input redirection before and after >>
                redtemp = strstr(before, "<");
                if (redtemp == NULL)
                {
                    // printf("redtemp is null\n");
                    redtemp = strstr(after, "<");
                    if (redtemp != NULL)
                    {
                        iored = 5;
                        strcpy(iredf, redtemp + 1);
                        trimstr(iredf);
                    }
                }
                else
                {
                    iored = 5;
                    strcpy(iredf, redtemp + 1);
                    trimstr(iredf);
                }
            }
            // printf("iored=%d\n", iored);
            // printf("iredf=%s\n", iredf);
            // printf("oredf=%s\n", oredf);

            // redirection
            switch (iored)
            {
            case 0:
                break;
            case 1:
                printf("input redirection only\n");
                // if (strlen(iredf) == 0)
                // {
                //     printf(ERROR_COLOR "Error: No input file specified\n" DEFAULT_COLOR);
                //     continue;
                // }
                // else
                // {
                //     int fd = open(iredf, O_RDONLY);
                //     if (fd < 0)
                //     {
                //         printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
                //         continue;
                //     }
                //     else
                //     {
                //         dup2(fd, STDIN_FILENO);
                //         close(fd);
                //     }
                // }
                break;
            case 2:
                FILE *oredirw = freopen(oredf, "w", stdout);
                if (oredirw == NULL)
                {
                    printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
                    continue;
                }
                break;
            case 3:
                FILE *oredira = freopen(oredf, "a", stdout);
                if (oredira == NULL)
                {
                    printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
                    continue;
                }
                break;
            case 4:
                printf("input and output redirection (w)\n");
                FILE *ioredirw = freopen(oredf, "w", stdout);
                if (ioredirw == NULL)
                {
                    printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
                    continue;
                }
                break;
            case 5:
                printf("input and output redirection (a)\n");
                FILE *ioredira = freopen(oredf, "a", stdout);
                if (ioredira == NULL)
                {
                    printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
                    continue;
                }
                break;
            }

            // tokenising the input
            char temp[strlen(input) + 1];
            strcpy(temp, input);
            char *inpworedir = strtok(temp, "<>");
            strcpy(temp, inpworedir);
            // printf("inp w/o redir=%s\n", temp);

            char *x;
            char *y;

            // printf("input: %s\n", input);
            char *command = strtok_r(temp, ";", &x);
            while (command != NULL)
            {
                if (strcmp(command, "bye") == 0)
                {
                    printf("bye ");
                    sleep(1);
                    printf(":(\n");
                    sleep(1);
                    fflush(stdout);
                    exit(0);
                }
                char cmtemp[strlen(command) + 1];
                strcpy(cmtemp, command);

                if (command[strlen(cmtemp) - 1] == '&')
                    strcat(cmtemp, " ");

                time_t start, end;
                double time_taken;

                int noa = -1; // number of ampersands
                char *subcom = strtok_r(cmtemp, "&", &y);
                trimstr(subcom);
                // printf("%s\t", subcom);

                while (subcom != NULL)
                {
                    noa++;
                    subcom = strtok_r(NULL, "&", &y);
                }

                memset(cmtemp, '\0', sizeof(cmtemp));
                char *y = NULL;
                strcpy(cmtemp, command);
                // printf("noa=%d\n", noa);

                if (noa == 0)
                {
                    subcom = strtok_r(cmtemp, "&", &y);
                    trimstr(subcom);
                    if (strcmp(subcom, "bye") == 0)
                    {
                        // printf("bye :(\n");
                        printf("bye ");
                        sleep(1);
                        printf(":(\n");
                        sleep(1);
                        fflush(stdout);
                        exit(0);
                    }

                    // start = clock();
                    time(&start);
                    fg(subcom);
                    // currfgid = -1;
                    time(&end);
                    time_taken = ((double)(end - start));
                    if (time_taken > 2)
                    {
                        pflag = (int)time_taken;
                        strcpy(pcom, subcom);
                    }
                }
                else
                {

                    char bgcom[noa][4096]; // background commands
                    subcom = strtok_r(cmtemp, "&", &y);
                    for (i = 0; i < noa; i++)
                    {
                        strcpy(bgcom[i], subcom);
                        if (strcmp(subcom, "bye") == 0)
                        {
                            printf("bye ");
                            sleep(1);
                            printf(":(\n");
                            sleep(1);
                            updatepastevents(input);
                            fflush(stdout);
                            exit(0);
                        }
                        subcom = strtok_r(NULL, "&", &y);
                    }

                    for (i = 0; i < noa; i++)
                        bg(bgcom[i]);

                    if (subcom != NULL)
                    {
                        trimstr(subcom);
                        time(&start);
                        fg(subcom);
                        // currfgid = -1;
                        time(&end);
                        time_taken = ((double)(end - start));
                        if (time_taken > 2)
                        {
                            pflag = (int)time_taken;
                            strcpy(pcom, subcom);
                        }
                    }
                }

                command = strtok_r(NULL, ";", &x);
            }

            // resetting the redirection
            switch (iored)
            {
            case 0:
                break;
            case 1:
                break;
            case 2:
                freopen("/dev/tty", "w", stdout);
                break;
            case 3:
                freopen("/dev/tty", "w", stdout);
                break;
            case 4:
                freopen("/dev/tty", "w", stdout);
                break;
            case 5:
                freopen("/dev/tty", "w", stdout);
                break;
            }
        }
    }

    return 0;
}
