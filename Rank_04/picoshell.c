#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
	int i = 0;
	int prev_fd = 0;
	pid_t pid;
	int fd[2];

	while(cmds[i]) //recordar ponerle el i o da seg fault
	{
		if(cmds[i+1]) // Crear el pipe si no es el último comando
			pipe(fd);
		pid = fork();
		if(pid == 0)
		{
			if(prev_fd !=0) // cuando no es el primero // Redirigir la entrada del comando actual
			{
				dup2(prev_fd, STDIN_FILENO); //prev sera la nueva entrada
				close(prev_fd);
			}
			if(cmds[i+1]) // Redirigir la salida si no es el último comando
			{
				close(fd[0]); //va en orden
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]); // Ejecutar el comando
			exit(1);
		}
		else
		{
			// En el proceso padre, cerrar fds
			if(prev_fd !=0) //no estamos en el primero, osea HAY un previo
				close(prev_fd);
			if(cmds[i+1])
			{
				close(fd[1]);//cerramos solo la salida
				prev_fd = fd[0]; //prev ahora va a ser fdd 0
			}
		}
		i++;
	}
	while(wait(NULL) > 0)
		;
	return (0);
}

int	main(int argc, char **argv)
{
	int	cmds_size = 1;
	char	***cmds = NULL;
	int	ret;

	// count pipes to deduce how many ***cmds, then malloc
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
			cmds_size++;
	}
	cmds = calloc(cmds_size + 1, sizeof(char **));
	if (!cmds)
	{
		dprintf(2, "Malloc error: %m\n");
		return 1;
	}
	cmds[cmds_size] = NULL;

	cmds[0] = argv + 1;
	int cmds_i = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
		{
			cmds[cmds_i] = argv + i + 1;
			argv[i] = NULL;
			cmds_i++;
		}
	}
	ret = picoshell(cmds);
	if (ret)
		perror("picoshell");
	free(cmds);
	return ret;
}


