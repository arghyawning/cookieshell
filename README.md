# cookieshell

## Instructions

### To compile:

`make`

### To run:

`./cookieshell`

## Description

- main.c: to keep accepting commands and calling the relevant functions
- common.c: to set the global values which will remain constant throughout
- prompt.c: to set correct prompts (based on the current directory) every time
- warp.c: for all `warp` commands
- proclore.c: for all `proclore` commands

## Assumptions

- The maximum size of file name is 255.
- The maximum size of system name is 255.
- The maximum size of user name is 255.
- The maximum size of a path is 4096.
- If I do `cd ..` when I am at `/`, where there isn't any further parent directory to go back to, I will just stay there (similar to the Ubuntu terminal).
- For `warp`,
  - If relative path is given as argument, it will be given in the format: `~/xyz` or `./xyz`
  - Absolute path is given as argument if and only if it begins with `/`
  - No file has space(s) in its name
- The assumptions for `warp` mostly follow through for `peek` and `seek` as well.
