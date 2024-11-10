#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SLEEP_TIME 3

int main(void)
{
    pid_t cpid[2];

    printf("PARENT - pid: %d, ppid: %d, gpid: %d\n", getpid(), getppid(), getpgrp());

    for (int i = 0; i < 2; ++i)
    {
        cpid[i] = fork();
        if (cpid[i] == -1)
        {
            perror("can't fork\n");
            exit(1);
        }
        else if (cpid[i] == 0)
        {
            printf("%d CHILD - pid: %d, ppid: %d, gr: %d\n", i, getpid(), getppid(), getpgrp());
            sleep(SLEEP_TIME);
            printf("%d CHILD - pid: %d, ppid: %d, gr: %d\n", i, getpid(), getppid(), getpgrp());
            exit(0);
        }
        else
        {
            printf("PARENT pid: %d, cpid: %d, gr: %d\n", getpid(), cpid[i], getpgrp());
        }
    }
    exit(0);
}