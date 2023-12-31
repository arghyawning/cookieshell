#ifndef __BG_H
#define __BG_H

#include <sys/types.h>
#include <sys/wait.h>

#include "headers.h"
#include "common.h"
#include "syscom.h"

void bg(char *subcom);
void handle_sigchld();
void delete_bg(int pid);

#endif