#include "msh.h"

int count_jobs = 1; // Keep track of total number of jobs
pid_t bg_child_pid = -1; // Store the pid of process resumed by bg command

void jobs(SLL **head) // Function for printing jobs list
{
    if (*head == NULL) // If list is empty
    {
        return;
    }

    SLL *temp = *head;
    SLL *prev = NULL;
    while (temp) // Traverse each node and print information about jobs
    {
        printf("[%d] %d\t%s\t%s\n", temp->node_count, temp->child_PID, temp->curr_state, temp->ip_str);
        if(strcmp(temp->curr_state, "Done") == 0)
        {
            count_jobs--;
            SLL *delete = temp;
            if (prev == NULL)
            {
                (*head) = temp->link;
                temp = temp->link;
            }
            else
            {
                prev->link = temp->link;
                temp = temp->link;
            }
            free(delete->ip_str);
            free(delete);
            continue;
        }
        prev = temp;
        temp = temp->link;
    }
}

void fg(SLL **head) // Function for making the stopped process alive again in foreground
{
    if (*head == NULL) // If list is empty, no process pending
    {
        printf("No process in background\n");
        return;
    }
    else
    {
        int fg_status = 0;
        childpresent_flag = 1;
        SLL *temp = *head;

        while (temp->link != NULL) // Move to the last node
        {
            temp = temp->link;
        }

        tcsetpgrp(shell_terminal, temp->child_PGID);

        if(kill(- temp->child_PGID, SIGCONT) < 0) // Continue the last stopped process using SIGCONT signal
        {
            perror("kill"); // In case kill failed
            return;
        }
        
        printf("%s\n", temp->ip_str);
        strcpy(temp->curr_state, "Running"); // Change the state of process from stopped to running
        signal(SIGCHLD, SIG_DFL);
        waitpid(temp->child_PID, &fg_status, WUNTRACED); // Wait for the process to terminate or stop
        signal(SIGCHLD, child_sig_handler);
        if (WIFEXITED(fg_status)) // If terminated
        {
            delete_at_last(head); // Remove the process from the list
        }
        else if(WIFSIGNALED(fg_status))
        {
            printf("\n");
            delete_at_last(head); // Remove the process from the list
        }
        else if (WIFSTOPPED(fg_status)) // If foreground process stopped again using SIGTSTP
        {
            printf("\n");
            strcpy(temp->curr_state, "Stopped"); // Make the state of process as Stopped
            printf("[%d] %d\t%s\t%s\n", temp->node_count, temp->child_PID, temp->curr_state, temp->ip_str); // Print the details of stopped job
        }

        tcsetpgrp(shell_terminal, pgid);
        childpresent_flag = 0;
    }
}

void bg(SLL **head) // Function for making the stopped process move to background
{
    if (*head == NULL) // If list is empty, no process pending
    {
        printf("No process in background\n");
    }
    else
    {
        SLL *temp = *head;
        while (temp->link != NULL) // Move to the last node
        {
            if(strcmp(temp->link->curr_state, "Running") == 0)
            {
                break;
            }
            temp = temp->link;
        }

        if (kill(- temp->child_PGID, SIGCONT) < 0) // Continue the last stopped process using SIGCONT signal
        {
            perror("kill"); // In case kill failed
            return;
        }
        printf("[%d] %s\n", temp->node_count, temp->ip_str);
        strcpy(temp->curr_state, "Running"); // Change the state of process from stopped to running
        bg_child_pid = temp->child_PID; // Store the pid of process inside bg child pid global variable
    }
}

/* Function for inserting stopped jobs inside linked list */
int insert_at_last(SLL **head, pid_t child_pid, char *input_string)
{
    SLL *newnode = (SLL *)malloc(sizeof(SLL));

    if (newnode == NULL)
    {
        return 1;
    }

    newnode->node_count = count_jobs++;
    newnode->child_PID = child_pid;
    newnode->child_PGID = getpgid(child_pid);
    newnode->ip_str = (char *)malloc(sizeof(char) * strlen(input_string) + 1);
    strcpy(newnode->ip_str, input_string);
    strcpy(newnode->curr_state, "Stopped");

    if (*head == NULL)
    {
        *head = newnode;
        return 0;
    }

    SLL *temp = *head;
    while (temp->link != NULL)
    {
        temp = temp->link;
    }

    temp->link = newnode;

    return 0;
}
/* Function for deleting terminated jobs from linked list */
int delete_at_last(SLL **head)
{
    if (*head == NULL)
    {
        return 1;
    }

    SLL *temp = *head;
    SLL *prev = NULL;

    if ((*head)->link == NULL)
    {
        (*head)->node_count = count_jobs--;
        *head = NULL;
        free(temp->ip_str);
        free(temp);
        return 0;
    }

    while (temp->link != NULL)
    {
        prev = temp;
        temp = temp->link;
    }
    temp->node_count = count_jobs--;
    prev->link = NULL;
    free(temp->ip_str);
    free(temp);

    return 0;
}