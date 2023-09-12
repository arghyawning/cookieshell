[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)

# cookieshell

i built my own shell from scratch!

## Instructions

### To compile:

`make`

### To run:

`./cookieshell`

### To quit:

`bye`

## Description

- main.c: to keep accepting commands and calling the relevant functions
- common.c: to set the global values which will remain constant throughout
- prompt.c: to set correct prompts (based on the current directory) every time
- warp.c: for all `warp` commands (resembling `cd` in bash)
- proclore.c: for all `proclore` commands
- peek.c: for all `peek` commands (resembling `ls` in bash)
- bg.c: for handling background processes
- fg.c: for handling foreground processes
- syscom.c: for handling system commands (like `echo`, `sleep`, etc)
- pastevents.c: for handling `pastevents` commands
- seek.c: for handling `seek` commands
- activities.c: for handling the `activities` command
- fgbg.c: for handling `fg` and `bg` commands
- signals.c: for handling signal-related i.e the `ping` commands.
- neonate.c: for handling the `neonate` command.
- iman.c: for handling `iMan` commands.

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
- If I have a command like `warp .. xyz` where `warp ..` is valid but `warp xyz` isn't, then we will just warp to `..` and then display the error message for the second part. Note that since half of the command was executed, we will include the entire command in `pastevents`.
- Maximum number of arguments in a command is 100.
- The maximum length of an argument is 4096.
- There can't be more than 250 background processes at a time
- Maximum length of a command is 10000.
- No file named `pastevents.txt` will be added to this directory.
- File name inputs are case sensitive
- In `neonate`,
  - if non-integer values are entered as the time argument, they will be rounded to their floor value.
  - non-negative values will not be entered.
