#include "msh.h"

void signal_handler(int sig_num)
{
    if (sig_num == SIGINT || sig_num == SIGTSTP) // ctrl + c or ctrl + z is given during read input
    {
        if (childpresent_flag == 0) // If no child process is running
        {
            printf("\n");
            fflush(stdout);
            printf(ANSI_COLOR_BLUE "%s " ANSI_COLOR_RESET, prompt);
            fflush(stdout);
        }
    }
}

void child_sig_handler(int signum) // Child handler function for handling background jobs
{
    int bg_child_status; // Store the exit code of process
    pid_t pid;           // store the return pid value of process

    /* Continuously monitor the process till process terminates or stopped*/
    while ((pid = waitpid(-1, &bg_child_status, WNOHANG)) > 0) // Wait for the process to terminate inside the loop using WNOHANG which immediately return if no child exited
    {
        if (WIFEXITED(bg_child_status) || WIFSIGNALED(bg_child_status)) // When child terminates
        {
            if(head == NULL)
            {
                signal(SIGCHLD, SIG_DFL);
                break;
            }
            /* Delete this particular child from the linked list/Job list */
            SLL *temp = head;
            SLL *prev = NULL;

            while (temp != NULL)
            {
                if (temp->child_PID == pid) // If node with child pid is found delete that node
                {
                    strcpy(temp->curr_state, "Done");
                    break;
                }
                prev = temp;
                temp = temp->link;
            }
        }
    }
}
