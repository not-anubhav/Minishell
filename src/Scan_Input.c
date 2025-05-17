#include "msh.h"

int last_command_status;                // To store the exit status of the child process
volatile int childpresent_flag = 0; // Flag for determining the child is exited or still present
pid_t child;               // For storing pid of child

void scan_input(char *prompt, char *input)
{
    int ps_flag = 0;

    extract_external_commands(external_cmd); // Function for extracting external commands from file

    signal (SIGINT, signal_handler);
    signal (SIGTSTP, signal_handler);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, child_sig_handler);

    while (1)
    {
        init_shell();
        
        int command_flag = 0; // Flag for determining if input string is PS1 command or any other command

        if (ps_flag == 0) // Concatenate path only when PS1 command is not used
        {
            char *curr_path_ptr = getcwd(curr_path, 250); // get the current working directory path

            if (curr_path_ptr != NULL)
            {
                strcat(prompt, curr_path_ptr); // If path is not NULL concatenate the path with prompt
            }

            strcat(prompt, "->");
        }

        printf(ANSI_COLOR_BLUE "%s " ANSI_COLOR_RESET, prompt); // Print the Prompt

        memset(input, '\0', 250); // Reset the input buffer

        scanf("%249[^\n]", input); // Read the input

        __fpurge(stdin); // Clear the stdin buffer

        if (strcmp(input, "") == 0 || input[0] == '\n') // Handle empty string
        {
            /* Reset all the buffers */

            memset(input, '\0', 250);

            memset(prompt + 12, '\0', 238);

            memset(curr_path, '\0', 250);

            continue; // Skip this iteration
        }

        if (strncmp(input, "PS1", 3) == 0) // If the command is PS1
        {
            char *equal_sign = strchr(input, '='); // Find the equal sign in Input string

            char *space = strchr(input, ' '); // Find the space in Input String

            if (space != NULL) // If Space is Present
            {
                command_flag = 1; // Set the command flag
            }

            else if (equal_sign == NULL) // If equal sign is not present
            {
                command_flag = 1; // Set the command flag
            }

            else if (*(input + 3) == ' ' || *(equal_sign + 1) == ' ') // Check if space is present before or after equal sign
            {
                command_flag = 1; // Set the command flag
            }

            else // Input string is valid PS1 command
            {
                strcpy(prompt, input + 4); // Copy the input string after PS1 command to Prompt
                ps_flag = 1;
            }
        }

        else // If Input string is not PS1 then set the command flag
        {
            command_flag = 1;
        }

        if (command_flag)
        {
            char *cmd = get_command(input); // Get the command from input string

            if (cmd == NULL)
            {
                continue;
            }

            int res = check_command_type(cmd); // Check the command type (eg. Builtin, external or Invalid)

            if (res == BUILTIN) // If the command in input string is an internal command
            {
                if (strcmp(cmd, "exit") == 0) // If the command is exit
                {
                    // Free the external_cmd array memory
                    for (int i = 0; i < external_cmd_size; i++)
                    {
                        free(external_cmd[i]);
                    }
                    _exit(0); // Call exit system call to exit from program
                }
                // If command is not exit
                execute_internal_commands(input); // Function call for Executing command
            }

            else if (res == EXTERNAL) // If the command in input string is an External command
            {
                int status = 0;

                child = fork(); // Create a child process

                childpresent_flag = 1; // Set the child present flag

                if (child > 0) // Parent process
                {
                    pid_t pid = getpid();

                    setpgid(child, child);

                    tcsetpgrp(shell_terminal, child);

                    signal(SIGCHLD, SIG_DFL);

                    waitpid(child, &status, WUNTRACED); // Wait for the child process to exit

                    signal(SIGCHLD, child_sig_handler);
                    
                    tcsetpgrp(shell_terminal, pgid);

                    if(WIFSIGNALED(status))
                    {
                        last_command_status = status;
                        printf("\n");
                        fflush(stdout);   
                    }
                    else if (WIFSTOPPED(status)) // If the child is stopped by signal
                    {
                        printf("\n");
                        insert_at_last(&head, child, input); // Insert the job inside the linked list
                        SLL *temp = head;
                        while (temp->link != NULL) // Navigate to last node where the job is insert
                        {
                            temp = temp->link;
                        }
                        printf("[%d] %d\t%s\t%s\n", temp->node_count, temp->child_PID, temp->curr_state, temp->ip_str);
                    }

                    if(setpgid(pid, pgid) < 0)
                    {
                        perror("setpgid");
                        exit(1);
                    }
                    childpresent_flag = 0; // Reset the child present flag
                }

                else if (child == 0) // Child Process
                {
                    pid_t pid;
                    pid = getpid();
                    if(setpgid(pid, pid) < 0)
                    {
                        perror("setpgid");
                        exit(1);
                    }
                    signal(SIGINT, SIG_DFL);            // Set the SIGINT signal to Default handler
                    signal(SIGTSTP, SIG_DFL);           // Set the SIGTSTP signal to Default handler
                    execute_external_commands(input);   // Call function to execute the external command
                    _exit(0);
                }
                else // Fork Failed
                {
                    perror("fork");
                }
            }
            else // If command is neither PS1 nor internal or external
            {
                printf(ANSI_COLOR_RED "Command not found %s\n" ANSI_COLOR_RESET, cmd);
            }
        }
        memset(prompt + 12, '\0', 238); // Reset the prompt buffer after the default prompt
        memset(curr_path, '\0', 250);   // Reset the current path buffer
    }
}
