#include "msh.h"

void execute_internal_commands(char *input_string) // Function for executing commands recognized as internal
{
    char *cmd = get_command(input_string); // Get the command from input string

    if (strcmp(cmd, "pwd") == 0) // If command is pwd print current working directory
    {
        char path[250];
        char *p = getcwd(path, 250); // Get the current working directory using getcwd library function
        if (p != NULL)
        {
            printf(ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, p); // Print the current working directory i.e path
        }
    }

    else if (strcmp(cmd, "cd") == 0) // If the command is cd, implement change directory
    {
        /* Trim the white spaces from the input string after first space */
        char *space = strchr(input_string, ' ');
        if (space != NULL)
        {
            space++;
            while (*space == ' ')
            {
                space++;
            }
        }

        if (strcmp(input_string, "cd") == 0) // If the input string is only cd
        {
            /* Navigate to HOME directory */
            char *path = getenv("HOME"); // Get home directory path using getenv library function
            if (path != NULL)
            {
                int res = chdir(path); // Change the directory using chdir system call
                if (res == -1)         // If chdir failed print error information
                {
                    if (errno == EACCES)
                    {
                        printf(ANSI_COLOR_RED "Permission denied\n" ANSI_COLOR_RESET);
                    }
                    else if (errno == ENOENT)
                    {
                        printf(ANSI_COLOR_RED "No such file or directory\n" ANSI_COLOR_RESET);
                    }
                }
            }
        }
        else // If input string is not just cd and contain path with it
        {
            char path[250];
            strcpy(path, space);   // Store the path after space inside path array
            int res = chdir(path); // Navigate to path using chdir system call
            if (res == -1)         // If chdir failed print error information
            {
                if (errno == EACCES)
                {
                    printf(ANSI_COLOR_RED "Permission denied\n" ANSI_COLOR_RESET);
                }
                else if (errno == ENOENT)
                {
                    printf(ANSI_COLOR_RED "No such file or directory\n" ANSI_COLOR_RESET);
                }
            }
        }
    }
    else if (strcmp(cmd, "echo") == 0) // If internal command is echo
    {
        echo(input_string, last_command_status); // Handle inside echo function
    }
    else if (strcmp(cmd, "jobs") == 0) // If internal command is jobs
    {
        jobs(&head); // Handle inside jobs function
    }
    else if (strcmp(cmd, "fg") == 0) // If internal command is fg
    {
        fg(&head); // Handle inside fg function
    }
    else if (strcmp(cmd, "bg") == 0) // If internal command is bg
    {
        bg(&head); // Handle inside bg function
    }
}
