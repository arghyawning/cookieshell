#include "headers.h"

void runcom(char *command, char *temp, char *input)
{
    int i;
    char *y = NULL;

    // checking for i/o redirection
    int iored = 0; /*
     0 for no redirection,
     1 for input redirection,
     2 for output redirection (w),
     3 for output redirection (a),
     4 for both input and output redirection (w),
     5 for both input and output redirection (a)*/
    char inputcopy[10000];
    strcpy(inputcopy, command);
    if (strstr(inputcopy, "<<") != NULL)
    {
        printf(ERROR_COLOR "Error: Invalid redirection type\n" DEFAULT_COLOR);
        return;
    }
    char iredf[4096];
    char oredf[4096];
    strcpy(iredf, "");
    strcpy(oredf, "");
    char *iort;
    char *redtemp = strstr(inputcopy, ">>");
    if (redtemp == NULL)
    {
        // printf("%s is null\n", inputcopy);
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
        // printf("input redirection only\n");
        FILE *iredir = freopen(iredf, "r", stdin);
        if (iredir == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        break;
    case 2:
        FILE *oredirw = freopen(oredf, "w", stdout);
        if (oredirw == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        break;
    case 3:
        FILE *oredira = freopen(oredf, "a", stdout);
        if (oredira == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        break;
    case 4:
        // printf("input and output redirection (w)\n");
        FILE *ioredir4 = freopen(iredf, "r", stdin);
        if (ioredir4 == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        FILE *ioredirw = freopen(oredf, "w", stdout);
        if (ioredirw == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        break;
    case 5:
        // printf("input and output redirection (a)\n");
        FILE *ioredir5 = freopen(iredf, "r", stdin);
        if (ioredir5 == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        FILE *ioredira = freopen(oredf, "a", stdout);
        if (ioredira == NULL)
        {
            printf(ERROR_COLOR "Error: No such file or directory\n" DEFAULT_COLOR);
            return;
        }
        break;
    }

    // printf("1command: %s\n", command);
    char *xyz;
    char temptemp[strlen(command) + 1];
    strcpy(temptemp, command);
    // char *inpworedir = strtok_r(temp, "<>", &xyz);
    char *inpworedir = strtok_r(temptemp, "<>", &xyz);
    // strcpy(command, inpworedir);
    // printf("1command: %s\n", command);

    char cmtemp[strlen(inpworedir) + 1];
    strcpy(cmtemp, inpworedir);

    // char cmtemp[strlen(command) + 1];
    // strcpy(cmtemp, command);

    // if (command[strlen(cmtemp) - 1] == '&')
    //     strcat(cmtemp, " ");

    if (inpworedir[strlen(inpworedir) - 1] == '&')
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
    y = NULL;
    strcpy(cmtemp, inpworedir);
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
    // resetting the redirection
    switch (iored)
    {
    case 0:
        break;
    case 1:
        freopen("/dev/tty", "r", stdin);
        break;
    case 2:
        freopen("/dev/tty", "w", stdout);
        break;
    case 3:
        freopen("/dev/tty", "w", stdout);
        break;
    case 4:
        freopen("/dev/tty", "r", stdin);
        freopen("/dev/tty", "w", stdout);
        break;
    case 5:
        freopen("/dev/tty", "r", stdin);
        freopen("/dev/tty", "w", stdout);
        break;
    }
}