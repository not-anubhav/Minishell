#include "msh.h"

int check_command_type(char *command)
{
    for (int i = 0; builtins[i] != NULL; i++) // Loop through built in commands 2d array
    {
        if (strcmp(command, builtins[i]) == 0) // Compare the command with builtin commands
        {
            return BUILTIN; // If found return the builtin macro
        }
    }

    for (int i = 0; i < external_cmd_size; i++) // Loop through external commands 2d array
    {
        if (strcmp(command, external_cmd[i]) == 0) // Compare the command with external commands
        {
            return EXTERNAL; // If found return the external macro
        }
    }

    return NO_COMMAND; // If command not matched in both builtin and external return no command macro
}
