#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include "prompt.h"
#include "warp.h"
#include "proclore.h"
#include "peek.h"
#include "fg.h"
#include "bg.h"
#include "pastevents.h"
#include "seek.h"
#include "activities.h"
#include "iman.h"
#include "fgbg.h"
#include "signals.h"

#define ERROR_COLOR "\e[0;31m"
#define DEFAULT_COLOR "\x1b[0m"

extern int bgi; // index of current background process
struct backproc
{
    /* data */
    int id;
    char comm[4096];
};
extern struct backproc *bgs; // list of background processes

extern char *pastfile;
extern int nope; // number of past events
// extern FILE *pef; // past events file
// extern int pesi; // past events start index
// extern int peei; // past events end index

extern int currfgid; // current foreground process id
extern char *currfgcom; // current foreground process command

extern char *prev; // previous directory

#endif