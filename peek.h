#ifndef __PEEK_H
#define __PEEK_H

#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define PRINT_DEFAULT "\x1b[0m"
#define PRINT_GREEN "\x1b[32m"
#define PRINT_BLUE "\x1b[34m"

int compare_strings(const void *a, const void *b);
int compare_names(const struct dirent **a, const struct dirent **b);
void colorprint(const char *finalpath, char *contents[], int noc);
char *processpath(char *path);
void print_colored_name(const char *name, mode_t mode);
void print_permissions(mode_t mode);
void peek(char *input);
void peek_nf(char *path);
void peek_a(char *path);
void peek_l(char *path);
void peek_al(char *path);

#endif