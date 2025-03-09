#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

bool killfail = false;
bool killchild = false;
pid_t pid;

void handlerfuction(int sig)
{
    (void)sig;
    if((kill(pid, SIGKILL)) < 0)
        killfail = true;
    else
        killchild = true;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    if ((pid = fork()) < 0)
        return -1;
    if (pid == 0)
    {
        f();
        exit(0);
    }
    
    struct sigaction sa;
    sa.sa_handler = handlerfuction;
    sa.sa_flags = SA_RESTART;
    if((sigaction(SIGALRM, &sa, NULL))) //SIGALRM   SIN < 0
        return -1;
    
    alarm(timeout);
    
    int wstatus;
    if ((waitpid(pid, &wstatus, 0)) < 0)
        return -1;
        
    if(killfail)
        return -1;
        
    if(WIFEXITED(wstatus))
    {
        if(WEXITSTATUS(wstatus) == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return 1;
        }
        if (verbose)
            printf("Bad function: exited with code %d\n", WEXITSTATUS(wstatus));
        return 0;
    }
    if(WIFSIGNALED(wstatus))
    {
        if(WTERMSIG(wstatus) == SIGKILL && killchild)
        {
            if (verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return 0;
        }
        if (verbose)
            printf("Bad function: %s\n", strsignal(WTERMSIG(wstatus)));
        return 0;
    }
    return -1;
}

void test1 (void) //bad exit
{
    printf("test 1\n");
    exit (41);
}
void test2 (void) //inf
{
    printf("test 2\n");
    printf("soy eterno\n");
    while(1);
}
void test3 (void) //leak
{
    printf("test 3\n");
    int *p = NULL;
    *p = 42;
}
void test4 (void) //correct
{
    printf("test 4\n");
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGALRM, &sa, NULL);
    sleep(4);
}

int main (void)
{
    sandbox(test1, 3, true);
    sandbox(test2, 3, true);
    sandbox(test3, 3, true);
    sandbox(test4, 3, true);
    return 0;
}