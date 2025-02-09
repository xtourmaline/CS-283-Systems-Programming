#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *temp;
    int first = 0;
    int i = 0;

    while (1) {
        if (first == 0) {
            temp = strtok(cmd_line, PIPE_STRING);
            first = 1;
        } else {
            temp = strtok(NULL, PIPE_STRING);
        }

        if (temp == NULL) {
            return OK;
        }

        if (clist->num == CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        trim(temp, SH_CMD_MAX);

        char *spaceChar = strchrnul(temp, SPACE_CHAR);

        int exeLen = spaceChar - temp;
        int argsLen = strnlen(spaceChar, ARG_MAX);

        if (exeLen >= EXE_MAX || argsLen >= ARG_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strncpy(clist->commands[i].exe, temp, exeLen);
        strncpy(clist->commands[i].args, spaceChar, argsLen);

        trim(clist->commands[i].args, argsLen);

        clist->num += 1;

        i++;
    }


    return EXIT_NOT_IMPL;
}

void trim(char *cmd_line, int cmd_size) {
    int endCmd;
    int foundFirst = 0;
    int newStartCmd = 0;
    int cmdLineLen = strnlen(cmd_line, cmd_size) - 1;

    if (cmdLineLen == 0) {
        return;
    }
    
    for (int i = cmdLineLen; i > 0; i--) {
        if (cmd_line[i] != SPACE_CHAR) {
            endCmd = i;
            break;
        }
    }

    for (int i = 0; i <= endCmd; i++) {
        if (cmd_line[i] != SPACE_CHAR && foundFirst == 0) {
            foundFirst = 1;
        }

        if (foundFirst == 1) {
            cmd_line[newStartCmd] = cmd_line[i];
            newStartCmd++;
        }
    }

    cmd_line[newStartCmd] = '\0';
}

 