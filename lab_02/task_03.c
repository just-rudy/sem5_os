#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define CHILD_CNT 2
#define EXEC1 "for_child1"
#define EXEC2 "for_child2"

int main(void)
{
    pid_t children[CHILD_CNT];
    char *execs[CHILD_CNT] = {EXEC1, EXEC2};

    for (int i = 0; i < CHILD_CNT; ++i)
    {
        children[i] = fork();
        if (children[i] == -1)
        {
            perror("ERROR: fork() failed");
            exit(EXIT_FAILURE);
        }
        else if (children[i] == 0)
        {
            printf("CHILD %d: pid=%d, ppid=%d, gr=%d\n", i, getpid(), getppid(), getpgrp());
            execl(execs[i], NULL);
            printf("ERROR: execl() for child with pid=%d failed.", getpid());
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Parent: pid=%d, childpid=%d, gr=%d\n", getpid(), children[i], getpgrp());
        }
    }

   int wait_status = 0;
   pid_t child_pid = 0;
   while (child_pid != -1)
   {
        child_pid = wait(&wait_status);
        if (child_pid == -1)
        {
            if (errno == ECHILD)
                break;  // no more children
        
            printf("ERROR: wait pid %d\n", wait_status);
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wait_status))
        {
            printf("Child with pid=%d exited, status=%d\n", child_pid, WEXITSTATUS(wait_status));
        }
        else if (WIFSIGNALED(wait_status))
        {
            printf("Child with pid=%d killed by signal %d\n", child_pid, WTERMSIG(wait_status));
        }
        else if (WIFSTOPPED(wait_status))
        {
            printf("Child with pid=%d stopped by signal %d\n", child_pid, WSTOPSIG(wait_status));
        }
        else if (WIFCONTINUED(wait_status))
        {
            printf("Child with pid=%d continued\n", child_pid);
        }
   }
   exit(EXIT_SUCCESS);
}