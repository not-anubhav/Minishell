#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct node
{
    unsigned int node_count;
    pid_t child_PID;
    pid_t child_PGID;
    char curr_state[20];
    char *ip_str;
    struct node *link;
}SLL; 

extern char *external_cmd[200];
extern int external_cmd_size;
extern int last_command_status;
extern char prompt[250];
extern char input[250];
extern char curr_path[250];
extern volatile int childpresent_flag;
extern pid_t child;
extern pid_t pgid;
extern int shell_terminal;
extern int interactive_mode;
extern SLL *head;
extern int count_jobs;
extern struct sigaction new_act;
extern pid_t bg_child_pid;

static char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval", "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source", "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg", "bg", NULL};

void init_shell();
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);
int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void n_pipe(int argc, char **args);
void jobs(SLL **head);
void fg(SLL **head);
void bg(SLL **head);
int insert_at_last(SLL **head, pid_t child_pid, char *input_string);
int delete_at_last(SLL **head);
void child_sig_handler(int signum);

#endif