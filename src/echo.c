#include "msh.h"

void echo(char *input_string, int status) // If the Input command is echo
{
    if (strcmp(input_string, "echo $$") == 0) // Print the process id
    {
        printf(ANSI_COLOR_GREEN"%d\n"ANSI_COLOR_RESET, getpid());
    }
    else if (strcmp(input_string, "echo $?") == 0) // Print the exit status of last executed command
    {
        printf(ANSI_COLOR_GREEN"%d\n"ANSI_COLOR_RESET, WEXITSTATUS(last_command_status));
    }
    else if (strcmp(input_string, "echo $SHELL") == 0) // Print the shell enviromental variable
    {
        printf(ANSI_COLOR_GREEN"%s\n"ANSI_COLOR_RESET, getenv("SHELL"));
    }
    else // Handle other cases
    {
        char *space = strchr(input_string, ' '); // Find the space in the input string with echo command
        if (space != NULL)
        {
            /* Trim the spaces after command */
            space++;
            while (*space == ' ')
                space++;
            char *dollar = strchr(input_string, '$'); // Find the $ character present with echo command
            if (dollar == NULL) // If $ character is not present
            {
                printf("%s\n", space); // Print the rest of string after space
            }
            else // If $ is found
            {
                char *env = (char *)malloc(sizeof(char) * strlen(dollar + 1)); // Allocate the memory to store input string after $
                strcpy(env, dollar + 1); // Copy the input string after $ into the allocated memory
                printf(ANSI_COLOR_GREEN"%s\n"ANSI_COLOR_RESET, getenv(env)); // Get the enviromental variable and print it
                free(env); // Free the allocated memory for env
            }
        }
    }
}