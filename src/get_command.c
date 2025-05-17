#include "msh.h"

char *get_command(char *input_string) // Get command from the input string
{
    static char cmd[50]; // Static command array

    int i = 0;

    while (input_string[i] != ' ' && input_string[i] && i < 249) // Store the characters in array till space is found or '\0'
    {
        cmd[i] = input_string[i];
        i++;
    }
    cmd[i] = '\0'; // Command string
    return cmd; // Return the address of command array
}