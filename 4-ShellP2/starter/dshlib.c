#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "dshlib.h"

void trim(char *cmd_line, int cmd_size);
void printerr();

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd = {0};

    cmd_buff = malloc(sizeof(char) * SH_CMD_MAX);
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    }

    // TODO IMPLEMENT MAIN LOOP
    while (true) {
        printf("%s", SH_PROMPT);
            if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
                printf("\n");
                break;
            }
        
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 
        //IMPLEMENT THE REST OF THE REQUIREMENTS
        clear_cmd_buff(&cmd);
        int ret = build_cmd_buff(cmd_buff, &cmd);

        if (ret != OK) {
            switch (ret) {
                case ERR_CMD_ARGS_BAD:
                    fprintf(stderr, "Badly formed arguments given\n");
                    continue;
                case ERR_MEMORY:
                    fprintf(stderr, "Failed to allocate memory\n");
                    continue;
                default:
                    fprintf(stderr, "Unexpected error when reading cmd_buff\n");
                    continue;
            }
        }

        if (cmd.argc == 0)
            continue;
        
        if (strncmp(cmd.argv[0], "cd", 2) == 0) {
            if (cmd.argc == 2) {
                if (chdir(cmd.argv[1]) == -1) {
                    rc = errno;
                    fprintf(stderr, "Failed to chdir\n");
                    printerr(rc);
                }
            }
        } else if (strncmp(cmd.argv[0], "dragon", 6) == 0) {
            print_dragon();
        } else if (strncmp(cmd.argv[0], "exit", 4) == 0) {
            free(cmd_buff);
            exit(0);
        } else if (strncmp(cmd.argv[0], "rc", 2) == 0) {
            printf("%d\n", rc);
        } else {
            pid_t pid = fork();

            if (pid == -1) {
                fprintf(stderr, "Failed to fork process\n");
                printerr(errno);
                rc = ERR_EXEC_CMD;
            }
            else if (pid == 0) {
                if (execvp(cmd.argv[0], cmd.argv) == -1) {
                    _exit(errno);
                }
            }
            else {
                // parent
                int wstatus;
                if (wait(&wstatus) == -1) {
                    perror(NULL);
                }

                if (WIFEXITED(wstatus)) {
                    rc = WEXITSTATUS(wstatus);
                    if (rc != 0) {
                        printerr(rc);
                    }
                }
            }
        }
    }

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    for (int i = 0; i < cmd_buff->argc; ++i) {
        free(cmd_buff->argv[i]);
        cmd_buff->argv[i] = NULL;
    }

    cmd_buff->argc = 0;

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    trim(cmd_line, SH_CMD_MAX);

    char *next;
    int length;

    while (cmd_line[0] != '\0') {
        trim(cmd_line, SH_CMD_MAX);

        if (cmd_line[0] == '"') {
            next = strchr(cmd_line + 1, '"');
            if (next == NULL) {
                return ERR_CMD_ARGS_BAD;
            }

            length = next - cmd_line - 1; // disregard end quote
            
            next += 1; // move past end quote
            cmd_line += 1; // move past start quote
        }
        else {
            next = strchrnul(cmd_line, SPACE_CHAR);
            length = next - cmd_line;
        }

        cmd_buff->argv[cmd_buff->argc] = malloc(sizeof(char) * (length + 1)); // null terminator
        if (cmd_buff->argv[cmd_buff->argc] == NULL) {
            return ERR_MEMORY;
        }

        memcpy(cmd_buff->argv[cmd_buff->argc], cmd_line, sizeof(char) * length);
        cmd_buff->argv[cmd_buff->argc][length] = '\0';

        cmd_buff->argc += 1;

        cmd_line = next;
    }

    return OK;
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

void printerr(int _errno) {
    switch (_errno) {
        case EACCES:
            fprintf(stderr, "Permission denied.\n");
            return;
        case EIO:
            fprintf(stderr, "An I/O error occurred.\n");
            return;
        case ELOOP:
            fprintf(stderr, "Too many symbolic links to follow.\n");
            return;
        case ENOENT:
            fprintf(stderr, "Path does not exist.\n");
            return;
        case ENOTDIR:
            fprintf(stderr, "Not a directory.\n");
            return;
        case EAGAIN:
            fprintf(stderr, "Cannot create any more threads.\n");
            return;
        case ENOMEM:
            fprintf(stderr, "Not enough memory available.\n");
            return;
        case ENOSYS:            
            fprintf(stderr, "Operation not supported.\n");
            return;
        case ENOEXEC:
            fprintf(stderr, "Cannot execute file.\n");
            return;
        case EISDIR:
            fprintf(stderr, "Is a directory.\n");
            return;
        case EPERM:
            fprintf(stderr, "Insufficient permissions.\n");
            return;
    }
}