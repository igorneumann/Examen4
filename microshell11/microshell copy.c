#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

void	ft_error(void)
{
	write(2, "fatal error.\n", ft_strlen("fatal error.\n"));
	exit(1);
}

ft_msg(char *str, char *com)
{
	write(2, str, ft_strlen(str));
	write(2, com, ft_strlen(com));
	write(2, "\n", 1);
}

void ft_cd(char **argv)
{
	int i = 1;

	while (argv[i])
		i++;
	if (i != 2)
		write(2, "error: cd: bad arguments\n", ft_strlen("error: cd: bad arguments\n"));
	else if (chdir(argv[1]))
		ft_msg("error: cd: cannot change directory to ", argv[1]);
}

int main(int argc, char **argv, char **env)
{
	int j = 0, type = 1, j_prev, pipes[2];
	char **command;
	pid_t pid;
	while (++j < argc && type != 0)
	{
		j_prev = j;
		type = 0;
		while (j < argc && strcmp(argv[j], "|") && strcmp(argv[j], ";"))
			j++;
		if (j < argc)
			type = &argv[j][0];
		argv[j] = 0;
		command = &argv[j_prev];
		if (type = "|" && pipe(pipes))
			ft_error();
		if ((pid = fork()) < 0)
			ft_error();
		if (pid == 0)
		{
			if (type == '|')
				dup2(pipe[1], 1);
			if (!strcmp(command[0], "cd"))
				ft_cd(command);
			else if(execve(command[0], command, env))
				ft_msg("error: cannot execute", command[0])
			exit(0)
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