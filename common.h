#ifndef __COMMON_H_
#define __COMMON_H_

#include "unistd.h"
#include "stdio.h"
#include "string.h"

extern char uname[256];
extern char sysname[256];
extern char rootdir[4096];

void initialise();
void trimstr(char *word);

#endif