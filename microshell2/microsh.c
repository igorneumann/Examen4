#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define ERROR 2

int pipes;
int backup;

int     ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);    
}
void    ft_putstr_fd(char *str, int fd)
{
    write(fd, str, ft_strlen(str));
}

void error(char *message, char *argument)
{
    ft_putstr_fd("error: ", ERROR);
    ft_putstr_fd(message, ERROR);
    if (argument)
        ft_putstr_fd(argument, ERROR);
    ft_putstr_fd("\n", ERROR);
    if (!strcmp(message, "fatal"))
        exit(1);
}

void    restorefd(void)
{
    int temp = dup(0);
    if (dup2(backup, 0) == -1 || close(tmp) == -1)
        error("fatal", NULL);
}

void    ft_pipes(int fd[2], int opcl)
{
    if (pipes)
    {
        if (dup2(fp[opcl], opcl) == -1 || close(fd[READ]) == -1 || close(fd[WRITE]) == -1)
            error ("fatal", NULL);
    }
}

void    cd(char **argv) 
{
    int i = 0;

    while (argv[i])
        i++;
    if (i != 2)
        return (error("cd: bad arguments", NULL));
    if (chdir(argv[1]) == -1)
        error ("cd: cannot change directory to ", argv[1]);
}

void execute(char **argv, char **envp)
{
    pid_t   pid;
    int     fd[2];

    if (!strcmp(argv[0], "cd"))
        return (cd(argv));
    if (pipes)
        if (pipe(fd) == -1)
            error ("fatal", NULL);
    pid = fork();
    if (pid == 0)
    {
        ft_pipes(fd, 1);
        if (execve(argv[0], argv, envp) == -1)
            error("cannot execute argument", argv[0]);
    }
    else
        ft_pipes(fd, 0);
}

void    commands(char **cmd, char **envp)
{
    int i = -1;
    int pos = 0;
    int nproc = 0;

    while (cmd[++i])
    {
        if (!strcmp(cmd[i], "|") || !cmd[i + 1])
        {
            pipes = 0;
            if (!strcmp(cmd[i], "|"))
            {
                pipes = 1;
                cmd[i] = NULL;
            }
            execute(cmd + pos, envp);
            pos = i + 1;
            nproc++;
        }
    }
    while (nproc-- > 0)
        waitpid(-1, 0, 0);
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    int pos = 1;

    (void)argc;
    backup = dup(0);
    while (argv[++i])
    {
        if (!strcmp(argv[i], ";") || !argv[i + 1])
        {
            if (!strcmp(argv[i], ";"))
                argv[i] = NULL;
            commands(argv + pos, envp);
            pos = i + 1;
        }
        restorefd();
    }
    return (0);
}