#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define CHILDREN_COUNT 3
#define MESS1 "xxxxxxxxxxxxx\n"
#define MESS2 "yyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n"
#define MESS3 "zzz\n"
#define SLEEPTIME 5

int ABLE_TO_WRITE = 0;

void allow_handler(int sig_num) {
    ABLE_TO_WRITE = 1;
}
 
int main(void) {
    pid_t children[CHILDREN_COUNT];
    char *messages[CHILDREN_COUNT] = {MESS1, MESS2, MESS3};
    int fd[2];

    signal(SIGINT, allow_handler);

    if (pipe(fd) == -1) {
        printf("ERROR: pipe() failed.");
        exit(EXIT_FAILURE);
    }
    printf("Parent pid: %d\n", getpid());
    for (int i = 0; i < CHILDREN_COUNT; i++) {
        children[i] = fork();
        if (children[i] == -1) {
            printf("ERROR: fork() failed.");
            exit(EXIT_FAILURE);
        } else if (children[i] == 0) {
            close(fd[0]); // Close reading end of pipe
            if (!ABLE_TO_WRITE) {
                sleep(SLEEPTIME);
                if (!ABLE_TO_WRITE) {
                    printf("ERROR: child with id %d no signal\n", getpid());
                    exit(EXIT_FAILURE);
                }
            }
            if (write(fd[1], messages[i], strlen(messages[i])) == -1) {
                printf("ERROR: write() for child with id %d failed.", getpid());
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            printf("Child with pid %d aw\n", children[i]);
        }
    }
    int wstatus = 0;
    pid_t childpid = 0;
    while (childpid != -1) {
        childpid = wait(&wstatus);
        if (childpid == -1) {
            if (errno == ECHILD) {
                break; // No more children
            }
            printf("ERROR: waitpid %d\n", wstatus);
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
            printf("Child with pid=%d exited, status=%d\n", childpid, WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)) {
            printf("Child with pid=%d was killed by signal %d\n", childpid, WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
            printf("Child with pid=%d was stopped by signal %d\n", childpid, WSTOPSIG(wstatus));
        } else if (WIFCONTINUED(wstatus)) {
            printf("Child with pid=%d continued\n", childpid);
        }
    }

    close(fd[1]); // Close writing end of pipe
    char buffer[256] = "\0";
    int count = 1;
    count = read(fd[0], buffer, sizeof(buffer) - 1);
    if (count == -1) {
        printf("ERROR: read\n");
        exit(EXIT_FAILURE);
    }
    if (count == 0) {
        printf("Pipe empty\n");
    } else {
        printf("message: %s\n", buffer);
    }
    
    // buffer[0] = '\0';
    // for (int i = 0; i < CHILDREN_COUNT; i++) {
        
    // }
    count = read(fd[0], buffer, 1);
    if (count == -1) {
        printf("ERROR: read\n");
        exit(EXIT_FAILURE);
    }
    if (count != 0) {
        printf("ERROR: lost data in pipe\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Pipe empty\n");
    }
    exit(EXIT_SUCCESS);
}