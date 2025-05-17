#include "msh.h"

void extract_external_commands(char **external_commands) // Extract external commands from the external_cmd.txt file
{
    for(int i = 0; i < external_cmd_size; i++)
    {
        free(external_cmd[i]);
    }

    int Filefd = open("external_cmd.txt", O_RDONLY); // Open the file in read only mode using open system call

    if (Filefd == -1) // If filefd is -1 print error
    {
        perror(ANSI_COLOR_RED"Failed to open external_cmd.txt"ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    char buf[50]; // Buffer for each command
    int i = 0, j = 0;
    while (read(Filefd, buf + i, 1) != 0) // Read the characters from file using read system call
    {
        if (buf[i] == '\n') // If new line is found store the command in 2d array
        {
            buf[i] = '\0'; // Convert to string
            external_cmd[j] = (char *)malloc(sizeof(char) * strlen(buf) + 1); // Allocate the memory for 2d array column
            strcpy(external_cmd[j], buf); // Copy the buf string into allocated memory
            i = 0; // Reset the buffer index
            j++; // Increment Command size
            continue; // Skip to next iteration
        }
        i++;
    }

    external_cmd_size = j; // Store the size in global variable
}