#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "prompt.h"
#include "warp.h"
#include "proclore.h"
#include "peek.h"
#include "fg.h"
#include "bg.h"

#define ERROR_COLOR "\e[0;31m"
#define DEFAULT_COLOR "\x1b[0m"

extern int bgi;    // index of current background process
// extern int nextbg; // index of next background process to be processed
struct backproc
{
    /* data */
    int id;
    char comm[4096];
};
extern struct backproc *bgs; // list of background processes

#endif