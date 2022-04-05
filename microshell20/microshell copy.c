#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int	ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return(i);
}

void ft_error(char *msg, char *com)
{
	write (2, msg, ft_strlen(msg));
	write (2, com, ft_strlen(com));
	write (2, "\n", 1);
}

void	ft_fatal(void)
{
	ft_error("error: ", "fatal");
	exit(1);
}

void	ft_cd(char **argv)
{
	int i = 0;
	while (argv[i])
		i++;
	if (i != 2)
		ft_error("error: ", "bad arguments");
	else if (chdir(argv[1]))
		ft_error("Error: cannot change directory to: ", argv[1]);
}

int main(int argc, char **argv, char **env)
{
	int i = 0, type = 1, i_prev, pipes[2];
	char **command;
	pid_t pid;
	while (++i < argc && type != 0)
	{
		type = 0;
		i_prev = i;
		while (i < argc && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (i < argc)
			type = argv[i][0];
		argv[i] = 0;
		command = &argv[i_prev];
		if (type == '|' && pipe(pipes))
			ft_fatal();
		if ((pid = fork()) < 0)
			ft_fatal();
		if (pid == 0)
		{
			if (type == '|')
				dup2(pipes[1], 1);
			if (!strcmp(command[0], "cd"))
				ft_cd(command);
			else if (execve(command[0], command, env))
				ft_error("error: cannot execute ", command[0]);
			exit (0);
		}
		else
			wait(0);
		if (type == '|')
		{
			dup2(pipes[0], 0);
			close(pipes[1]);
			close(pipes[0]);
		}
	}
	return (0);
}