#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<signal.h>
#include<string.h> //?
#include<stdbool.h>

pid_t pid;
bool kill_failed = false;
bool killed_child = false;

void kill_proc(int signal) {
	(void)signal;
	if (kill(pid, SIGKILL) == -1)
		kill_failed = true;
	else 
	    killed_child = true;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    if((pid = fork()) < 0)
        return -1;
        
    else if(pid == 0)
    {
        //alarm(timeout);
        f();
        exit(0);
    }
    
    // Proceso padre
    struct sigaction sa;
    sa.sa_handler = kill_proc;
    sa.sa_flags = SA_RESTART; //se reiniciará automáticamente la llamada al sistema si se interrumpe por una señal, en lugar de devolver un error EINTR
    
    if (sigaction(SIGALRM, &sa, NULL))
        return -1;

    alarm(timeout);
    
    int wstatus;
    
    if (waitpid(pid, &wstatus, 0) == -1)
    {
		//printf("INSIDE IFFFF\n");
		return -1;
    }
    
   
    if (kill_failed)
    return -1;    
    
    //alarm(0); // Cancelar alarma en el padre si el hijo ya terminó
    
    if(WIFEXITED(wstatus)) // Proceso terminó normalmente
    {
        if(WEXITSTATUS(wstatus) == 0)
        {
            if(verbose)
                printf("Nice function!\n");
            return 1;
        }
        if(verbose)
            printf("Bad function: exited with code %d\n", WEXITSTATUS(wstatus));
        return 0;
    }
    
    else if(WIFSIGNALED(wstatus)) // Proceso terminó por una señal
    {
        if(WTERMSIG(wstatus) == SIGKILL && killed_child) // SIGALRM if (WTERMSIG(wstatus) == SIGKILL && killed_child) 
        {
            if(verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return 0;
        }
        if(verbose)
            printf("Bad function: %s\n", strsignal(WTERMSIG(wstatus)));
        return 0;
    }
    else   
        return -1;
}

//ps aux | grep Z
//TESTS
void cancel_alarm (void)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGALRM, &sa, NULL);
    sleep(2);
}

void bad_exit(void)
{
	exit(41);
}

void inf_f(void)
{
	printf("I will run forevaaaa\n");
	while (1);
}
void leak_f(void)
{
	int *p = NULL;
	*p = 4;
}

void f6(void) {
	
    (void)printf("I will block all signalsss (and run forever)!\n");
	(void)sigaction(SIGALRM, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGKILL, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGSTOP, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGTERM, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGSEGV, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGPIPE, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGBUS, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGCHLD, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGHUP, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGTSTP, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGTTIN, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGTTOU, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGURG, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	(void)sigaction(SIGQUIT, &(struct sigaction){.sa_handler = SIG_IGN, .sa_flags = SA_RESTART}, NULL);
	while (true) {
	}
}

int main (void)
{
    //sandbox(bad_exit, 3, true);
    //sandbox(inf_f, 2, true);
    //sandbox(leak_f, 2, true);
    //sandbox(f6, 2, true);
    sandbox(cancel_alarm, 3, true);
    return(0);
}
