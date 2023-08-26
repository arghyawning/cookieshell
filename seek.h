#ifndef __SEEK_H
#define __SEEK_H

#include <dirent.h>
#include <sys/stat.h>

#include "headers.h"

void seek(char *input, char *current);
void searchdf(char *current, char *name);
void searchd(char *current, char *name);
void searchf(char *current, char *name);
void searchedf(char *current, char *name);
void searchef(char *current, char *name);
void searched(char *current, char *name);

#define BLUE_PROMPT "\e[0;34m"
#define GREEN_PROMPT "\e[0;32m"

extern int nom; // number of matches

#endif