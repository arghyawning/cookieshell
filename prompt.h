#ifndef __PROMPT_H
#define __PROMPT_H

#define CYAN_PROMPT "\e[0;36m"
#define DEFAULT_PROMPT "\x1b[0m"
#define PURPLE_PROMPT "\e[0;35m"
#define YELLOW_PROMPT "\e[0;33m"

void prompt(int pflag, char *pcom);

#endif