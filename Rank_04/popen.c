#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int ft_popen(const char *file, char *const argv [], char type)
{
    
    if(!file || !argv || (type != 'r' && type != 'w'))
        return -1;

    int fd[2];
    if(pipe(fd) == -1) //crea el pipe
        return -1;
        
    pid_t pid = fork(); //crea el proceso hijo duplicando el actual
    if (pid < 0)
    {
        close(fd[1]);
        close(fd[0]);
        return -1;
    }
    
    //child process
    if(pid == 0)
    {
        if(type == 'r') //modo lectura del padre
            dup2(fd[1], STDOUT_FILENO); //Redirige la SALIDA estándar del child
        
        else if(type == 'w') //modo escritura del padre
            dup2(fd[0], STDIN_FILENO); //Redirige la ENTRADA estándar del child
        
        close(fd[0]);
        close(fd[1]);
        
        execvp(file, argv); //reemplaza el proceso hijo con el nuevo programa a ejecutar post pipes. si tiene exito, se va de ft_popen
        perror("execvp"); //si falla, retorna -1 y continua con el codigo, que seria el mensaje de error
        exit (127); // Código de error estándar si exec falla
    }
    
    //parent process
    else 
    {
        if(type == 'r')
        {
            close(fd[1]); //cierra el fd que el child duplico
            return(fd[0]); //retorna el otro
        }
        if(type == 'w')
        {
            close(fd[0]);
            return(fd[1]);
        }
    }
    return (-1);
}

//test 'r'
int main (void)
{
    int fdd;
    char buf[1];
    fdd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r'); //con los []
    while(read(fdd, buf, 1))
        write(1, buf, 1);
    close(fdd);
    return(0);
}

//test 'w'
int main (void)
{
    int fdd;
    char *str = "hello\n world\n";
    fdd = ft_popen("wc", (char *const[]){"wc", NULL}, 'w'); //con los []
    write(fdd, str, strlen(str));
    close(fdd);
    return(0);
}










