#include "headers.h"
#include "common.h"

int nom;
char *eflag = NULL;
int fdflag; // stores 1 if file and 2 if directory

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

void searchedf(char *current, char *name)
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
                if (nom > 1)
                    return;
                strcpy(eflag, path);
                fdflag = 2;
                // printf(GREEN_PROMPT "%s\n" DEFAULT_COLOR, path);
            }
            searchedf(path, name); // Recurse into subdirectory
        }
        else if (S_ISREG(file_stat.st_mode) && (strcmp(entry->d_name, name) == 0 || strncmp(entry->d_name, namedot, strlen(namedot)) == 0))
        {
            nom++;
            if (nom > 1)
                return;
            // printf(BLUE_PROMPT "%s\n" DEFAULT_COLOR, path);
            strcpy(eflag, path);
            fdflag = 1;
        }
    }

    closedir(dir);
}

void searchef(char *current, char *name)
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
            searchef(path, name); // Recurse into subdirectory
        else if (S_ISREG(file_stat.st_mode) && (strcmp(entry->d_name, name) == 0 || strncmp(entry->d_name, namedot, strlen(namedot)) == 0))
        {
            nom++;
            if (nom > 1)
                return;
            // printf(GREEN_PROMPT "%s\n" DEFAULT_COLOR, path);
            strcpy(eflag, path);
            fdflag = 1;
        }
    }

    closedir(dir);
}

void searched(char *current, char *name)
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
                if (nom > 1)
                    return;
                // printf(BLUE_PROMPT "%s\n" DEFAULT_COLOR, path);
                strcpy(eflag, path);
                fdflag = 2;
            }
            searched(path, name); // Recurse into subdirectory
        }
    }

    closedir(dir);
}

void special(char *path)
{
    if (fdflag == 1)
    {
        // file
        // printf("special action for file: %s\n", path);
        // char *command = (char *)malloc(strlen(path) + 6);
        // strcpy(command, "cat ");
        // strcat(command, path);
        // system(command);
        FILE *fp = fopen(path, "r");
        if (fp == NULL)
        {
            printf(ERROR_COLOR "Missing permissions for task!\n" DEFAULT_COLOR);
            fclose(fp);
            return;
        }

        char filebuffer[4096];
        int read;
        while ((read = fread(filebuffer, 1, sizeof(filebuffer), fp)) > 0)
            fwrite(filebuffer, 1, read, stdout);

        fclose(fp);

        printf("\n");
    }
    if (fdflag == 2)
    {
        // directory
        // printf("special action for directory: %s\n", path);

        if (access(path, X_OK) != 0)
        {
            printf(ERROR_COLOR "Missing permissions for task!\n" DEFAULT_COLOR);
            return;
        }

        char *command = (char *)malloc(strlen(path) + 6);
        strcpy(command, "warp ");
        strcat(command, path);
        warp(command);
    }
}

void seek(char *input, char *current)
{
    fdflag = 0;
    eflag = (char *)malloc(4096);

    nom = 0;
    // printf("input: %s\n", input);

    char temp[strlen(input) + 1];
    strcpy(temp, input); // copy of full command
    // printf("%s\n", temp);

    char *word = strtok(temp, " \t\n");
    word = strtok(NULL, " \t\n");

    if (word == NULL) // no flag or name or path
    {
        printf(ERROR_COLOR "No arguments provided\n" DEFAULT_COLOR);
        return;
    }

    else if (strcmp(word, "-d") == 0) // -d
    {
        word = strtok(NULL, " \t\n");
        if (word == NULL) // -d no name or path
        {
            printf(ERROR_COLOR "No directory name provided\n" DEFAULT_COLOR);
            return;
        }
        else if (strcmp(word, "-e") == 0) // -d -e
        {
            // printf("-d -e case\n");
            // return;

            word = strtok(NULL, " \t\n");
            if (word == NULL) // -e -d no name or path
            {
                printf(ERROR_COLOR "No directory name provided\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '-') // invalid flags
            {
                printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-d path
            {
                printf(ERROR_COLOR "Path provided but no directory name to find\n" DEFAULT_COLOR);
                return;
            }

            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);
            // -e -d name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-e -d name no path
                searched(current, name);
            else
            {
                if (word[0] == '/') // -e -d name absolute path
                    searched(word, name);
                else if (strcmp(word, ".") == 0)
                    searched(current, name);
                else if (strcmp(word, "~") == 0)
                    searched(rootdir, name);
                else if (word[0] == '~') // -e -d name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searched(rel, name);
                }
                else if (word[0] == '.') // -e -d name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searched(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
            if (nom == 0)
                printf("No such directory found\n");
            else if (nom == 1)
                special(eflag);
            // printf("%s: Special action\n", eflag);
            else
                printf("Multiple directories found\n");
            nom = 0;
            return;
        }
        else if (word[0] == '-') // invalid flags
        {
            printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
            return;
        }
        else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-d path
        {
            printf(ERROR_COLOR "Path provided but no directory name to find\n" DEFAULT_COLOR);
            return;
        }

        else
        {

            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);
            // -d name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-d name no path
                searchd(current, name);
            else
            {
                if (word[0] == '/') // -d name absolute path
                    searchd(word, name);
                else if (strcmp(word, ".") == 0)
                    searchd(current, name);
                else if (strcmp(word, "~") == 0)
                    searchd(rootdir, name);
                else if (word[0] == '~') // -d name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchd(rel, name);
                }
                else if (word[0] == '.') // -d name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchd(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
        }

        if (nom == 0)
            printf("No such directory found\n");
        nom = 0;
    }

    else if (strcmp(word, "-e") == 0) //-e
    {
        word = strtok(NULL, " \t\n");
        if (word == NULL) //-e no other flag or name or path
        {
            printf(ERROR_COLOR "No file or directory name provided\n" DEFAULT_COLOR);
            return;
        }

        else if (strcmp(word, "-d") == 0) // -e -d
        {
            // printf("-e -d case\n");
            // return;
            word = strtok(NULL, " \t\n");
            if (word == NULL) // -e -d no name or path
            {
                printf(ERROR_COLOR "No directory name provided\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '-') // invalid flags
            {
                printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-d path
            {
                printf(ERROR_COLOR "Path provided but no directory name to find\n" DEFAULT_COLOR);
                return;
            }

            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);
            // -e -d name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-e -d name no path
                searched(current, name);
            else
            {
                if (word[0] == '/') // -e -d name absolute path
                    searched(word, name);
                else if (strcmp(word, ".") == 0)
                    searched(current, name);
                else if (strcmp(word, "~") == 0)
                    searched(rootdir, name);
                else if (word[0] == '~') // -e -d name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searched(rel, name);
                }
                else if (word[0] == '.') // -e -d name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searched(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
        }

        else if (strcmp(word, "-f") == 0) // -e -f
        {
            // printf("-e -f case\n");
            // return;

            word = strtok(NULL, " \t\n");
            if (word == NULL) // -e -f no name or path
            {
                printf(ERROR_COLOR "No file name provided\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '-') // invalid flags
            {
                printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-e -f path
            {
                printf(ERROR_COLOR "Path provided but no file name to find\n" DEFAULT_COLOR);
                return;
            }

            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);

            // -e -f name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-e -f name no path
                searchef(current, name);
            else
            {
                if (word[0] == '/') // -f name absolute path
                    searchef(word, name);
                else if (strcmp(word, ".") == 0)
                    searchef(current, name);
                else if (strcmp(word, "~") == 0)
                    searchef(rootdir, name);
                else if (word[0] == '~') // -f name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchef(rel, name);
                }
                else if (word[0] == '.') // -d name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchef(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
        }

        else if (word[0] == '-') // invalid flags
        {
            printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
            return;
        }
        else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-e path
        {
            printf(ERROR_COLOR "Path provided but no file or directory name to find\n" DEFAULT_COLOR);
            return;
        }

        else
        {
            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);
            // -e name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-e name no path
                searchedf(current, name);
            else
            {
                if (word[0] == '/') // -e name absolute path
                    searchedf(word, name);
                else if (strcmp(word, ".") == 0)
                    searchedf(current, name);
                else if (strcmp(word, "~") == 0)
                    searchedf(rootdir, name);
                else if (word[0] == '~') // -e name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchedf(rel, name);
                }
                else if (word[0] == '.') // -e name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchedf(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
        }
        if (nom == 0)
            printf("No such file or directory found\n");
        else if (nom == 1)
            special(eflag);
        // printf("%s: Special action\n", eflag);
        else
            printf("Multiple files or directories found\n");
        nom = 0;
    }

    else if (strcmp(word, "-f") == 0) // -f
    {
        word = strtok(NULL, " \t\n");
        if (word == NULL) // -f no name or path
        {
            printf(ERROR_COLOR "No file name provided\n" DEFAULT_COLOR);
            return;
        }
        else if (strcmp(word, "-e") == 0) // -f -e
        {
            // printf("-f -e case\n");
            // return;

            word = strtok(NULL, " \t\n");
            if (word == NULL) // -e -f no name or path
            {
                printf(ERROR_COLOR "No file name provided\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '-') // invalid flags
            {
                printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
                return;
            }
            else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-e -f path
            {
                printf(ERROR_COLOR "Path provided but no file name to find\n" DEFAULT_COLOR);
                return;
            }

            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);

            // -e -f name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-e -f name no path
            {
                // printf("hi");
                searchef(current, name);
                // printf("bye");
            }
            else
            {
                if (word[0] == '/') // -e -f name absolute path
                    searchef(word, name);
                else if (strcmp(word, ".") == 0)
                    searchef(current, name);
                else if (strcmp(word, "~") == 0)
                    searchef(rootdir, name);
                else if (word[0] == '~') // -f name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchef(rel, name);
                }
                else if (word[0] == '.') // -d name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchef(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
            if (nom == 0)
                printf("No such file found\n");
            else if (nom == 1)
                special(eflag);
            // printf("%s: Special action\n", eflag);
            else
                printf("Multiple files found\n");
            nom = 0;
            return;
        }
        else if (word[0] == '-') // invalid flags
        {
            printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
            return;
        }
        else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-f path
        {
            printf(ERROR_COLOR "Path provided but no file name to find\n" DEFAULT_COLOR);
            return;
        }

        else
        {
            char *name = malloc(strlen(word) + 1);
            strcpy(name, word);

            // -f name
            word = strtok(NULL, " \t\n");
            if (word == NULL) //-f name no path
                searchf(current, name);
            else
            {
                if (word[0] == '/') // -f name absolute path
                    searchf(word, name);
                else if (strcmp(word, ".") == 0)
                    searchf(current, name);
                else if (strcmp(word, "~") == 0)
                    searchf(rootdir, name);
                else if (word[0] == '~') // -f name home path
                {
                    char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                    strcpy(rel, rootdir);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchf(rel, name);
                }
                else if (word[0] == '.') // -d name relative path
                {
                    char *rel = malloc(strlen(current) + strlen(word) + 1);
                    strcpy(rel, current);
                    strcat(rel, word + 1);
                    // printf("searching path: %s\n", rel);
                    searchf(rel, name);
                }
                else
                {
                    printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                    return;
                }
            }
        }
        if (nom == 0)
            printf("No such file found\n");
        nom = 0;
    }

    else // no valid flag
    {
        if (word[0] == '-')
        {
            printf(ERROR_COLOR "Invalid flags!\n" DEFAULT_COLOR);
            return;
        }
        else if (word[0] == '.' || word[0] == '~' || word[0] == '/') //-d path
        {
            printf(ERROR_COLOR "Path provided but not file or directory name\n" DEFAULT_COLOR);
            return;
        }
        char *name = malloc(strlen(word) + 1);
        strcpy(name, word); // name
        word = strtok(NULL, " \t\n");
        if (word == NULL) //-d name no path
            searchdf(current, name);
        else
        {
            if (word[0] == '/') // -d name absolute path
                searchdf(word, name);
            else if (strcmp(word, ".") == 0)
                searchdf(current, name);
            else if (strcmp(word, "~") == 0)
                searchdf(rootdir, name);
            else if (word[0] == '~') // -d name home path
            {
                char *rel = malloc(strlen(rootdir) + strlen(word) + 1);
                strcpy(rel, rootdir);
                strcat(rel, word + 1);
                // printf("searching path: %s\n", rel);
                searchdf(rel, name);
            }
            else if (word[0] == '.') // -d name relative path
            {
                char *rel = malloc(strlen(current) + strlen(word) + 1);
                strcpy(rel, current);
                strcat(rel, word + 1);
                // printf("searching path: %s\n", rel);
                searchdf(rel, name);
            }
            else
            {
                printf(ERROR_COLOR "Invalid path\n" DEFAULT_COLOR);
                return;
            }
        }

        if (nom == 0)
            printf("No such file or directory found\n");
        nom = 0;
    }
}