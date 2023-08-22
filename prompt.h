#ifndef __PROMPT_H
#define __PROMPT_H

#include "unistd.h"
#include "string.h"

#define CYAN_PROMPT "\e[0;36m"
#define DEFAULT_PROMPT "\x1b[0m"
#define PURPLE_PROMPT "\e[0;35m"

void prompt();

#endif