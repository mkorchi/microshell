/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorch<mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:09:45 by mkorchi           #+#    #+#             */
/*   Updated: 2022/09/16 11:04:09 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i++]);
	return (i);
}

char	*ft_strdup( char *str )
{
	int		i;
	char	*ret;

	ret = malloc(sizeof(char ) * (ft_strlen(str) + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void	error_message(char *msg)
{
	write(2, msg, ft_strlen(msg));
}

void	free_everything(t_cmd *cmd, int length)
{
	int	i;
	int	j;

	i = 0;
	while ( i < length )
	{
		free_args(cmd[i].args);
		i++;
	}
	free(cmd);
}

void	init_all_cmds(t_cmd *cmd, int length)
{
	int	i;

	i = 0;
	while ( i < length )
	{
		cmd[i].args = NULL;
		i++;
	}
}


char	**add_arg(char **args, char *arg)
{
	int		len;
	char	**new_args;
	int		i;
	
	if (args == NULL)
	{
		args = malloc(sizeof(char * ) * 2);
		args[0] = ft_strdup(arg);
		args[1] = NULL;
		return (args);
	}
	len = 0;
	while (args[len++]);
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i++] = ft_strdup(arg);
	new_args[i] = NULL;
	free(args);
	return (new_args);
}

void	check_fd_leaks()
{
	int t = dup(1);
	if (t == 3)
		printf("no file descriptor leak\n");
	else
		printf("file descriptor leak\n");
	close(t);
}

int	main(int argc, char **argv, char **envp) {
	t_cmd	*cmd;
	int		i;
	int		count;
	
	if (argc < 2)
		exit(0);

	count = 0;
	if ( strcmp(argv[0], "cd") == 0 ) {
		if ( argc != 3 ) {
			error_message("error: cd: bad arguments\n");
			exit(1);
		}
		if ( chdir(argv[1]) == -1) {
			error_message("error: cd: cannot change directory to path_to_change\n");
			exit(1);
		}
		exit(0);
	}
	i = 1;
	while ( i < argc )
	{
		if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], ";") == 0)
			count++;
		i++;	
	}
	count++;
	cmd = malloc(sizeof(t_cmd) * (count));
	if (!cmd)
		return (0);
	init_all_cmds(cmd, count);
	int	k = 0;
	i = 1;
	while ( i < argc ) {
		if (i == 1)
			cmd[k].sep_type = START;
		if (strcmp(argv[i], "|") == 0) 
			cmd[++k].sep_type = PIPE;
		else if (strcmp(argv[i], ";") == 0)
			cmd[++k].sep_type = SEMICOLON;
		else
			cmd[k].args = add_arg(cmd[k].args, argv[i]);
		i++;
	}
	int	temp = 0;
	int	fd[2];
	int	pid;
	int	active_pipe = 0;
	i = 0;
	while (i < count) {
		if (i + 1 == count || cmd[i + 1].sep_type == SEMICOLON)
			active_pipe = 0;
		else
		{
			active_pipe = 1;
			pipe(fd);
		}
		if (cmd[i].sep_type == SEMICOLON) {
			waitpid(pid, NULL, 0);
		}
		pid = fork();
		if (pid < 0)
		{
			error_msg("error: fatal\n");
			exit(1);
		}
		if (pid == 0) {
			if (active_pipe)
				close(fd[0]);
			if (temp > 2) {
				dup2(temp, 0);
				close(temp);
			}
			if (active_pipe) {
				dup2(fd[1], 1);
				close(fd[1]);
			}
			execve(cmd[i].args[0], cmd[i].args, envp);
			error_msg("error: cannot execute ");
			error_msg(cmd[i].args[0]);
			error_msg("\n");
			exit(1);
		}
		if (temp > 2)
			close(temp);
		if (active_pipe) {
			close(fd[1]);
			temp = fd[0];
		}
		i++;
	}
	if (temp > 2)
		close(temp);
	while(wait(NULL) > 0);
	free_everything(cmd, count);

	// system("leaks a.out");
	// check_fd_leaks();
	return (0);
}