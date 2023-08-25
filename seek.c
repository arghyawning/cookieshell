#include "headers.h"

int nom;

void searchf(char *current, char *name)
{
    DIR *dir = opendir(current);
    if (dir == NULL)
    {
        printf(ERROR_COLOR "Error opening current directory\n" DEFAULT_COLOR);
        return;
    }

    char *namedot = malloc(strlen(name) + 1);
    strcpy(namedot, name);
    strcat(namedot, ".");

    // printf("%s %s\n", namedot, name);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[4096]; // Adjust this size as needed
        snprintf(path, sizeof(path), "%s/%s", current, entry->d_name);
        // printf("%s\n", path);

        struct stat file_stat;
        if (stat(path, &file_stat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode))
            searchf(path, name); // Recurse into subdirectory
        else if (S_ISREG(file_stat.st_mode) && (strcmp(entry->d_name, name) == 0 || strncmp(entry->d_name, namedot, strlen(namedot)) == 0))
        {
            nom++;
            printf(GREEN_PROMPT "%s\n" DEFAULT_COLOR, path);
        }
    }

    closedir(dir);
}

void searchd(char *current, char *name)
{
    DIR *dir = opendir(current);
    if (dir == NULL)
    {
        printf(ERROR_COLOR "Error opening current directory\n" DEFAULT_COLOR);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[4096]; // Adjust this size as needed
        snprintf(path, sizeof(path), "%s/%s", current, entry->d_name);
        // printf("%s\n", path);

        struct stat file_stat;
        if (stat(path, &file_stat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode))
        {
            if (strcmp(entry->d_name, name) == 0)
            {
                nom++;
                printf(BLUE_PROMPT "%s\n" DEFAULT_COLOR, path);
            }
            searchd(path, name); // Recurse into subdirectory
        }
    }

    closedir(dir);
}

void searchdf(char *current, char *name)
{
    DIR *dir = opendir(current);
    if (dir == NULL)
    {
        printf(ERROR_COLOR "Error opening current directory\n" DEFAULT_COLOR);
        return;
    }

    char *namedot = malloc(strlen(name) + 1);
    strcpy(namedot, name);
    strcat(namedot, ".");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[4096]; // Adjust this size as needed
        snprintf(path, sizeof(path), "%s/%s", current, entry->d_name);
        // printf("%s\n", path);

        struct stat file_stat;
        if (stat(path, &file_stat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode))
        {
            if (strcmp(entry->d_name, name) == 0)
            {
                nom++;
                printf(GREEN_PROMPT "%s\n" DEFAULT_COLOR, path);
            }
            searchdf(path, name); // Recurse into subdirectory
        }
        else if (S_ISREG(file_stat.st_mode) && (strcmp(entry->d_name, name) == 0 || strncmp(entry->d_name, namedot, strlen(namedot)) == 0))
        {
            nom++;
            printf(BLUE_PROMPT "%s\n" DEFAULT_COLOR, path);
        }
    }

    closedir(dir);
}

void seek(char *input, char *current)
{
    nom = 0;
    // printf("%s\n", input);

    char temp[strlen(input) + 1];
    strcpy(temp, input); // full command
    // printf("%s\n", temp);

    char *word = strtok(temp, " \t\n");
    word = strtok(NULL, " \t\n");

    if (word == NULL) // no flag
    {
        printf(ERROR_COLOR "No arguments provided\n" DEFAULT_COLOR);
        return;
    }

    else if (strcmp(word, "-d") == 0)
    {
        word = strtok(NULL, " \t\n");
        if (word == NULL)
        {
            printf(ERROR_COLOR "No directory name provided\n" DEFAULT_COLOR);
            return;
        }
        else if (word[0] == '-')
        {
            printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
            return;
        }
        searchd(current, word);
        if (nom == 0)
            printf("No such directory found\n");
        nom = 0;
    }

    else if (strcmp(word, "-e") == 0)
    {
    }
    else if (strcmp(word, "-f") == 0)
    {
        word = strtok(NULL, " \t\n");
        if (word == NULL)
        {
            printf(ERROR_COLOR "No file name provided\n" DEFAULT_COLOR);
            return;
        }
        else if (word[0] == '-')
        {
            printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
            return;
        }
        searchf(current, word);
        // strcat(word, ".");
        // searchf(current, word);
        if (nom == 0)
            printf("No such file found\n");
        nom = 0;
    }
    else
    {
        // printf("no%s\n", word);
        searchdf(current, word);
        // strcat(word, ".");
        // searchdf(current, word);
        if (nom == 0)
            printf("No such file found\n");
        nom = 0;
    }
}