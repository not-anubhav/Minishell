#include "msh.h"

char prompt[250] = "@Minishell->"; // Default Prompt
char input[250];
char *external_cmd[200]; // For storing external commands from external_cmd.txt file
int external_cmd_size = 0; // Total external_commands
char curr_path[250]; // Current Directory Path
SLL *head = NULL; // Linked List Head for Jobs List
pid_t pgid;
int interactive_mode;
int shell_terminal;

int main()
{   
    system("clear"); // Clear the terminal first time when program is executed

    scan_input(prompt, input); 

    /* Clear the memory allocated for external_cmd 2D array*/
    for(int i = 0; i < external_cmd_size; i++)
    {
        free(external_cmd[i]);
    }
}

void init_shell()
{
    shell_terminal = STDIN_FILENO;
    interactive_mode = isatty(shell_terminal);

    if(interactive_mode)
    {
        while(tcgetpgrp(shell_terminal) != (pgid = getpgid(0)))
        {
            kill(- pgid, SIGTTIN);
        }

        pgid = getpid();
        if(setpgid(pgid, pgid) == -1)
        {
            perror("setpgid");
            exit(1);
        }

        tcsetpgrp(shell_terminal, pgid);
    }
}
