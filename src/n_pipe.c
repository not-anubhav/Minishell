#include "msh.h"

void n_pipe(int argc, char **args) // Function for Handling input string with pipes
{
    int pipe_count = 0; // Track number of pipes present in input string
    int cmd_pos[argc]; // Store the position of commands in input string inside this array
    cmd_pos[0] = 0; // Store the first command postion as 0 at first index in cmd_pos array
    int cmd_index = 1; // Initilize the command index with 1

    for(int i = 0; args[i] != NULL; i++) 
    {
        if(strcmp(args[i], "|") == 0) 
        {
            pipe_count++; // Count the pipes in input string 
            args[i] = NULL; // Replace pipe with NULL in input string
            cmd_pos[cmd_index++] = i + 1; // Store the command position next to found pipe position
        }
    }

    if(pipe_count == cmd_index - 1) // if commands are n + 1, then pipes should be n
    {
        int pipefd[2]; // array for storing pipe read and write fd 
        int stdin_bckup = dup(0); // Duplicate stdin fd

        for(int i = 0; i <= pipe_count; i++) // Loop for pipe count times
        {
            if(i < pipe_count) // Avoid creating pipe for last iteration as we only need n pipes for n+1 commands
            {
                if (pipe(pipefd) == -1) // Create pipe and store pipe read and write fd in pipefd array
                {
                    perror("pipe"); // If pipe creation failed
                    exit(1);
                }
            }

            pid_t pid = fork(); // Create child process

            if(pid > 0) // Parent Process
            {
                if(i < pipe_count) // Avoid duplicating and closing pipe for last iteration
                {
                    if(dup2(pipefd[0], 0) == -1) // Duplicate pipe read fd to stdin
                    {
                        perror("dup2");
                        exit(1);
                    }
                    
                    if(close(pipefd[1]) == -1) // Close pipe write fd
                    {
                        perror("close");
                        exit(1);
                    }

                    if(close(pipefd[0]) == -1) // Close pipe read fd
                    {
                        perror("close");
                        exit(1);
                    }
                }
            }
            else if(pid == 0) // Child Process
            {
                if(i < pipe_count) // Avoid duplication and closing of pipe fd's for last iteration
                {         
                    if(close(pipefd[0]) == -1) // Close the pipe read fd
                    {
                        perror("close");
                        exit(1);
                    }

                    if(dup2(pipefd[1], 1) == -1) // Duplicate the pipe write fd to stdout
                    {
                        perror("dup2");
                        exit(1);
                    }

                    if(close(pipefd[1]) == -1) // Close pipe write fd
                    {
                        perror("close");
                        exit(1);
                    }
                }
                
                execvp(args[cmd_pos[i]], &args[cmd_pos[i]]); // Execute each command inside args array upto NULL
                perror("execvp"); // In case execvp failed
                exit(1);
            }
            else
            {
                perror("fork");
            }
        }

        dup2(stdin_bckup, 0); // Restore stdin back 
        close(stdin_bckup); // Close stdin backup fd

        for(int i =0; i <= pipe_count; i++)
        {
            wait(NULL); // Wait for all the child process to terminate in parent process
        }
    }
}
