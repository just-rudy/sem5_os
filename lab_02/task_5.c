#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define NUM_CHILDREN 2

int static flag = 0;

void sig_handler(int sig_numb)
{
    flag = 1;
}

int main(void)
{
    pid_t children[NUM_CHILDREN];
    char *messages[2] = {"aaaa", "bbbbbbbbbbbbbbbb"};
    int fdpipe[2];

    if (pipe(fdpipe) == -1)
    {
        perror("ERROR: Can't pipe\n");
        exit(1);
    }

    if (signal(SIGINT, sig_handler) == -1)
    {
        perror("ERROR: can't signal\n");
        exit(1);
    }
    sleep(2);

    for (size_t i = 0; i < NUM_CHILDREN; ++i)
    {
        if ((children[i] = fork()) == -1)
        {
            perror("ERROR: Can't fork\n");
            exit(1);
        }
        else if (children[i] == 0)
        {
            if (flag)
            {
                printf("PID %d sent message: %s\n", getpid(), messages[i]);
                close(fdpipe[0]);
                write(fdpipe[1], messages[i], strlen(messages[i]) + 1);
            }
            else
            {
                printf("PID %d no signal\n", getpid());
            }
            exit(0);
        }
    }

    char buf[128];
    for (size_t i = 0; i < NUM_CHILDREN; ++i)
    {
        close(fdpipe[1]);
        read(fdpipe[0], buf, strlen(messages[i]) + 1);
        buf[25] = '\0';
        printf("PID %d received message: %s\n", getpid(), buf);
    }

    buf[0] = '\0';
    close(fdpipe[1]);

    int read_bytes_cnt = read(fdpipe[0], buf, sizeof(buf) - 1);
    printf("PID %d recieved messages: %s\n", getpid(), buf);

    int wstatus = 0;
    pid_t childpid = 0;
    while (childpid != -1) {
        childpid = wait(&wstatus);
        if (childpid == -1) {
            if (errno == ECHILD) {
                break; // no more children
            }
            printf("Error: waitpid %d\n", wstatus);
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
            printf("Child with pid=%d exited, status=%d\n", childpid, WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)) {
            printf("Child with pid=%d killed by signal %d\n", childpid, WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
            printf("Child with pid=%d stopped by signal %d\n", childpid, WSTOPSIG(wstatus));
        } else if (WIFCONTINUED(wstatus)) {
            printf("Child with pid=%d continued\n", childpid);
        }
    }

    exit(0);
}
