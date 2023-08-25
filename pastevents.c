#include "headers.h"
#include "common.h"

int backcheck(char *input, char *last)
{
    char *inputtok[125];
    char *lasttok[125];

    int l1 = 0, l2 = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL)
    {
        inputtok[l1] = token;
        l1++;
        token = strtok(NULL, " \t\n");
    }

    token = strtok(last, " \t\n");
    while (token != NULL)
    {
        lasttok[l2] = token;
        l2++;
        token = strtok(NULL, " \t\n");
    }

    if (l1 != l2)
        return 1;

    for (int i = 0; i < l1; i++)
    {
        if (strcmp(inputtok[i], lasttok[i]) != 0)
            return 1;
    }

    return 0;
}

void execpast(char *input)
{
    int pflag = 0;
    char pcom[4096];
    strcpy(pcom, "");

    int i;

    if (input[strlen(input) - 1] == '\n')
        input[strlen(input) - 1] = '\0';

    // tokenising the input
    char temp[strlen(input) + 1];
    strcpy(temp, input);

    char *x;
    char *y;

    // printf("input: %s\n", input);
    char *command = strtok_r(temp, ";", &x);
    while (command != NULL)
    {
        if (strcmp(command, "bye") == 0)
        {
            printf("bye :(\n");
            // updatepastevents(input);
            fflush(stdout);
            exit(0);
        }
        char cmtemp[strlen(command) + 1];
        strcpy(cmtemp, command);

        if (command[strlen(cmtemp) - 1] == '&')
            strcat(cmtemp, " ");

        // clock_t start, end;
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
                printf("bye :(\n");
                // updatepastevents();
                fflush(stdout);
                exit(0);
            }

            // start = clock();
            time(&start);
            fg(subcom);
            time(&end);
            // end = clock();
            time_taken = ((double)(end - start));
            // printf("fg took time %lf\n", time_taken);
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
                    printf("bye :(\n");
                    // updatepastevents(input);
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
                // start = clock();
                time(&start);
                fg(subcom);
                time(&end);
                // end = clock();
                time_taken = ((double)(end - start));
                // printf("fg took time %lf\n", time_taken);
                if (time_taken > 2)
                {
                    pflag = (int)time_taken;
                    strcpy(pcom, subcom);
                }
            }
            // fg(subcom);
        }

        command = strtok_r(NULL, ";", &x);
    }
}

void pastevents(char *command)
{
    char *arg = strtok(command, " \n\t");
    arg = strtok(NULL, " \n\t");

    if (arg == NULL)
    {
        FILE *pef = fopen(pastfile, "r");

        if (pef == NULL)
        {
            perror(ERROR_COLOR "Unable to open the past events" DEFAULT_COLOR "\n");
            fclose(pef);
            return;
        }

        char line[10000];

        int empty = 1;
        // printf("%d\n", nope);
        int i = 0;
        while (fgets(line, sizeof(line), pef))
        {
            empty = 0;
            i++;
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n')
                printf("\n");
        }

        if (empty)
            printf("No past event.\n");

        fclose(pef);
    }
    else if (strcmp(arg, "purge") == 0)
    {
        if (nope == 0)
        {
            printf("No past events to purge.\n");
            return;
        }
        FILE *pef = fopen(pastfile, "w");
        if (pef == NULL)
        {
            perror(ERROR_COLOR "Unable to access past events" DEFAULT_COLOR "\n");
            fclose(pef);
            return;
        }
        fclose(pef);
        nope = 0;
    }
    else if (strcmp(arg, "execute") == 0)
    {
        arg = strtok(NULL, " \n\t");
        int ind = atoi(arg);
        if (ind > 0 && ind <= 15)
        {
            ind = nope - ind + 1;
            FILE *pef = fopen(pastfile, "r");
            if (pef == NULL)
            {
                perror(ERROR_COLOR "Unable to open the file" DEFAULT_COLOR "\n");
                fclose(pef);
                return;
            }

            char line[10000];

            for (int i = 1; i < ind; ++i)
            {
                if (fgets(line, sizeof(line), pef) == NULL)
                {
                    printf(ERROR_COLOR "There are less than %d past events.\n" DEFAULT_COLOR, ind);
                    fclose(pef);
                    return;
                }
            }

            // Print the nth line
            if (fgets(line, sizeof(line), pef) != NULL)
            {
                // printf("Past event %d: %s", ind, line);
                trimstr(line);
                execpast(line);
                if (ind < nope)
                    updatepastevents(line);
            }
            else
                printf(ERROR_COLOR "Error reading the past event %d.\n" DEFAULT_COLOR, ind);

            fclose(pef);
        }
        else
            printf(ERROR_COLOR "pastevents %s: invalid argument" DEFAULT_COLOR "\n", arg);
    }
    else
        printf(ERROR_COLOR "pastevents %s: invalid argument" DEFAULT_COLOR "\n", arg);
}

void updatepastevents(char *input)
{

    if (strstr(input, "pastevents") != NULL)
        return;

    char *past[nope];
    // printf("%d updating history in %s: %s\n", nope, pastfile, input);

    if (nope > 0)
    {
        FILE *pef = fopen(pastfile, "r");
        int i;

        if (pef == NULL)
        {
            perror(ERROR_COLOR "Unable to access past events" DEFAULT_COLOR "\n");
            fclose(pef);
            exit(1);
        }
        else
        {
            char *line = NULL;
            size_t ls = 10000;
            fseek(pef, 0, SEEK_SET);
            for (i = 0; i < nope; ++i)
            {
                past[i] = (char *)malloc(10000 * sizeof(char));
                if (past[i] == NULL)
                {
                    perror(ERROR_COLOR "Unable to allocate memory" DEFAULT_COLOR "\n");
                    exit(1);
                }
                if (getline(&line, &ls, pef) == -1)
                {
                    perror(ERROR_COLOR "Error reading line" DEFAULT_COLOR);
                    exit(1);
                }
                strcpy(past[i], line);
            }
            fclose(pef);
        }
    }
    FILE *pef = fopen(pastfile, "w");
    if (pef == NULL)
    {
        perror(ERROR_COLOR "Unable to access past events" DEFAULT_COLOR "\n");
        fclose(pef);
        exit(1);
    }

    for (int i = 0; i < nope; ++i)
        fprintf(pef, "%s", past[i]);

    if (backcheck(input, past[nope - 1]) == 1)
    {
        fprintf(pef, "%s\n", input);
        nope++;
    }

    fclose(pef);

    for (int i = 0; i < nope - 1; i++)
        free(past[i]);

    // nope++;
    // pastevents("pastevents");
}