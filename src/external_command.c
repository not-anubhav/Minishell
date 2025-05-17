#include "msh.h"

void execute_external_commands(char *input_string) // If command in input string is an external command
{
    char *args[100]; // 2d array to store input string
    char buf[250]; // Temporary buffer

    int i = 0, j = 0, k = 0, pipe_found = 0;

    /* Convert 1D input string into 2D array */
    while(input_string[i])
    {
        if(input_string[i] == '|') // If pipe is present in input string
        {
            pipe_found = 1; // Set the flag
        }
    
        if(input_string[i] == ' ' && k > 0) // When space is found in input string
        {
            buf[k] = '\0'; // Convert the temporary buffer to string
            args[j] = (char*)malloc(sizeof(char) * (k + 1)); // Allocate memory for storing the buffer string
            strcpy(args[j], buf); // Copy the buffer in allocated memory
            j++;
            i++;
            k = 0; // reset the buffer index
            continue; 
        }

        if(input_string[i] != ' ') // If character is not space
        {
            buf[k++] = input_string[i]; // Store it in temporary buffer
        }
        i++;
    }
    /* Store the last word */
    buf[k] = '\0';
    args[j] = (char*)malloc(sizeof(char) * (k + 1));
    strcpy(args[j], buf);
    args[j + 1] = NULL;
    int argc = j + 1; // Create a variable to store the 2D array size

    if(!pipe_found) // If pipe is not found
    {
        int ret = execvp(args[0], args); // Execute the command
        if(ret == -1)// If execvp failed
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else // If pipe found
    {
        n_pipe(argc, args); // function for executing command with n pipes
    }

    for(int l = 0; l < argc; l++) // Clear the memory allocated for 2D array args
    {
        free(args[l]);
    }
}
