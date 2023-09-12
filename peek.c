#include "headers.h"
#include "common.h"

void print_colored_name(const char *name, mode_t mode)
{
    if (S_ISDIR(mode))
        printf(PRINT_BLUE "%s" PRINT_DEFAULT, name);
    else if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        printf(PRINT_GREEN "%s" PRINT_DEFAULT, name);
    else
        printf("%s", name);
}

void print_permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int compare_names(const struct dirent **a, const struct dirent **b)
{
    const char *name_a = (*a)->d_name;
    const char *name_b = (*b)->d_name;

    // Skip the leading dot (.) if present
    if (name_a[0] == '.')
        name_a++;
    if (name_b[0] == '.')
        name_b++;

    return strcasecmp(name_a, name_b);
}

int compare_strings(const void *a, const void *b)
{
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;

    // Skip the leading dot (.) if present
    if (str_a[0] == '.')
        str_a++;
    if (str_b[0] == '.')
        str_b++;

    return strcasecmp(str_a, str_b);
}

void colorprint(const char *finalpath, char *contents[], int noc)
{
    for (int i = 0; i < noc; i++)
    {
        if (contents[i][0] != '\0')
        {
            struct stat entry_stat;
            char full_path[4096];

            snprintf(full_path, sizeof(full_path), "%s/%s", finalpath, contents[i]);

            if (stat(full_path, &entry_stat) == 0)
            {
                if (S_ISDIR(entry_stat.st_mode))
                    printf(PRINT_BLUE "%s" PRINT_DEFAULT "\n", contents[i]);
                else if (access(full_path, X_OK) == 0)
                    printf(PRINT_GREEN "%s" PRINT_DEFAULT "\n", contents[i]);
                else
                    printf("%s\n", contents[i]);
            }
        }
    }
}

char *processpath(char *path)
{
    // char finalpath[strlen(path) + 3];
    char *finalpath = (char *)malloc(4096);
    // if (path[0] == '~')
    //     path[0] = '.';
    if (strcmp(path, "~") == 0) // shell home
        strcpy(finalpath, rootdir);
    else
    {
        if (path[0] != '.' && path[0] != '/')
            strcpy(finalpath, "./");
        strcpy(finalpath, path);
    }

    // if (access(path, F_OK) == -1)
    //     return "not valid";
    return finalpath;
}

void peek_nf(char *path)
{
    // printf("ls %s\n", path);

    char *finalpath = processpath(path);
    // printf("final path: %s\n", finalpath);
    DIR *dir;
    struct dirent *content;

    dir = opendir(finalpath);
    if (dir == NULL)
    {
        printf(ERROR_COLOR);
        perror("opendir");
        printf(DEFAULT_COLOR);
        return;
    }

    int noc = 0; // content count
    content = readdir(dir);
    while (content != NULL)
    {
        if (content->d_name[0] != '.')
            noc++;
        content = readdir(dir);
    }
    closedir(dir);

    char *contents[noc];
    int i = 0;
    dir = opendir(finalpath);
    content = readdir(dir);
    while (content != NULL)
    {
        if (content->d_name[0] != '.')
            contents[i++] = content->d_name;
        // printf("%ld\t%s\t%ld\t%d\t%d\n", content->d_ino, content->d_name, content->d_off, content->d_reclen, content->d_type);
        content = readdir(dir);
    }
    closedir(dir);

    qsort(contents, noc, sizeof(char *), compare_strings);

    colorprint(finalpath, contents, noc);
}

void peek_a(char *path)
{
    // printf("ls -a %s\n", path);

    char *finalpath = processpath(path);
    // printf("final path: %s\n", finalpath);
    DIR *dir;
    struct dirent *content;

    dir = opendir(finalpath);
    if (dir == NULL)
    {
        printf(ERROR_COLOR);
        perror("opendir");
        printf(DEFAULT_COLOR);
        return;
    }

    int noc = 0;
    content = readdir(dir);
    while (content != NULL)
    {
        noc++;
        content = readdir(dir);
    }
    closedir(dir);

    char *contents[noc];
    int i = 0;
    dir = opendir(finalpath);
    content = readdir(dir);
    while (content != NULL)
    {
        contents[i++] = content->d_name;
        content = readdir(dir);
    }
    closedir(dir);

    qsort(contents, noc, sizeof(char *), compare_strings);

    colorprint(finalpath, contents, noc);
}

void peek_l(char *path)
{
    // printf("ls -l %s\n", path);

    // struct dirent *content;
    struct stat info;
    struct passwd *pw;
    struct group *gr;

    // DIR *dir = opendir(path);
    // if (!dir)
    // {
    //     perror("opendir");
    //     return;
    // }

    struct dirent **namelist;
    // int num_entries = scandir(path, &namelist, NULL, compare_names);
    int noc = scandir(path, &namelist, NULL, (int (*)(const struct dirent **, const struct dirent **))compare_names);
    if (noc < 0)
    {
        printf(ERROR_COLOR);
        perror("scandir");
        printf(DEFAULT_COLOR);
        return;
    }

    for (int i = 0; i < noc; ++i)
    {
        if (namelist[i]->d_name[0] != '.')
        {
            char cont_path[4096];
            snprintf(cont_path, sizeof(cont_path), "%s/%s", path, namelist[i]->d_name);

            struct stat info;
            if (lstat(cont_path, &info) == -1)
            {
                printf(ERROR_COLOR);
                perror("lstat");
                printf(DEFAULT_COLOR);
                continue;
            }

            print_permissions(info.st_mode);
            printf(" %2ld", info.st_nlink);

            if ((pw = getpwuid(info.st_uid)) != NULL)
                printf(" %s", pw->pw_name);
            else
                printf(" %d", info.st_uid);

            if ((gr = getgrgid(info.st_gid)) != NULL)
                printf(" %s", gr->gr_name);
            else
                printf(" %d", info.st_gid);

            printf(" %8ld", info.st_size);

            char date[50];
            strftime(date, sizeof(date), "%b %d %H:%M ", localtime(&info.st_mtime));
            printf(" %s", date);

            print_colored_name(namelist[i]->d_name, info.st_mode);
            printf("\n");
        }

        free(namelist[i]);
    }

    free(namelist);

    // closedir(dir);
}

void peek_al(char *path)
{
    // printf("ls -al %s\n", path);

    // printf("ls -l %s\n", path);

    // struct dirent *content;
    struct stat info;
    struct passwd *pw;
    struct group *gr;

    // DIR *dir = opendir(path);
    // if (!dir)
    // {
    //     perror("opendir");
    //     return;
    // }

    struct dirent **namelist;
    // int num_entries = scandir(path, &namelist, NULL, compare_names);
    int noc = scandir(path, &namelist, NULL, (int (*)(const struct dirent **, const struct dirent **))compare_names);
    if (noc < 0)
    {
        printf(ERROR_COLOR);
        perror("scandir");
        printf(DEFAULT_COLOR);
        return;
    }

    for (int i = 0; i < noc; ++i)
    {
        char cont_path[4096];
        snprintf(cont_path, sizeof(cont_path), "%s/%s", path, namelist[i]->d_name);

        struct stat info;
        if (lstat(cont_path, &info) == -1)
        {
            printf(ERROR_COLOR);
            perror("lstat");
            printf(DEFAULT_COLOR);
            continue;
        }

        print_permissions(info.st_mode);
        printf(" %2ld", info.st_nlink);

        if ((pw = getpwuid(info.st_uid)) != NULL)
            printf(" %s", pw->pw_name);
        else
            printf(" %d", info.st_uid);

        if ((gr = getgrgid(info.st_gid)) != NULL)
            printf(" %s", gr->gr_name);
        else
            printf(" %d", info.st_gid);

        printf(" %8ld", info.st_size);

        char date[50];
        strftime(date, sizeof(date), "%b %d %H:%M ", localtime(&info.st_mtime));
        printf(" %s", date);

        print_colored_name(namelist[i]->d_name, info.st_mode);
        printf("\n");

        free(namelist[i]);
    }

    free(namelist);

    // closedir(dir);
}

void peek(char *input)
{
    char temp[strlen(input) + 1];
    strcpy(temp, input); // full command
    // printf("%s\n", temp);

    char *word = strtok(temp, " \t");
    word = strtok(NULL, " \t");

    if (word == NULL)
    {
        // printf("no\n");
        // just ls for current directory
        peek_nf(".");
    }
    else
    {
        // printf("%s\n", word);

        if (strcmp(word, "-a") == 0)
        {
            word = strtok(NULL, " \t");
            if (word == NULL) // ls -a
                peek_a(".");
            else if (strcmp(word, "-l") == 0) // ls -a -l
            {
                word = strtok(NULL, "\t");
                if (word == NULL) // ls -a -l
                    peek_al(".");
                else // ls -a -l dir
                    peek_al(word);
            }
            else // ls -a dir
                peek_a(word);
        }
        else if (strcmp(word, "-l") == 0)
        {
            word = strtok(NULL, " \t");
            if (word == NULL) // ls -l
                peek_l(".");
            else if (strcmp(word, "-a") == 0) // ls -l -a
            {
                word = strtok(NULL, "\t");
                if (word == NULL) // ls -l -a
                    peek_al(".");
                else // ls -l -a dir
                    peek_al(word);
            }
            else // ls -l dir
                peek_l(word);
        }
        else if (strcmp(word, "-la") == 0 || strcmp(word, "-al") == 0)
        {
            word = strtok(NULL, " \t");
            if (word == NULL) // ls -al or ls -la
                peek_al(".");
            else // ls -al dir or ls -la dir
                peek_al(word);
        }
        else
        {
            // path
            peek_nf(word);
        }
    }
}