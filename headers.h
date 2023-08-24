#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "prompt.h"
#include "warp.h"
#include "proclore.h"
#include "peek.h"
#include "fg.h"
#include "bg.h"

#define ERROR_COLOR "\e[0;31m"
#define DEFAULT_COLOR "\x1b[0m"

char *bgs[250];

#endif