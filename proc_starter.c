/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_starter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:55:20 by ayeganya          #+#    #+#             */
/*   Updated: 2024/05/22 16:19:36 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	my_exit(char *err_msg, size_t *misc, t_cmd *p_cmd);

char	*exec_compose2(char *exec, char **paths)
{
	int		i;
	char	*temp;
	char	*result;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		result = ft_strjoin(temp, exec);
		free(temp);
		if (!access(result, X_OK))
			return (result);
		i++;
	}
	return (0);
}

char	*exec_compose(char *exec, char **env)
{
	int		i;
	char	**paths;

	i = 0;
	if (exec == 0)
		return (0);
	if (!ft_strncmp(exec, "/", 1) || !ft_strncmp(exec, "./", 2) || \
			!ft_strncmp(exec, "../", 3))
		return (exec);
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (env[i] == 0)
		return (exec);
	paths = ft_split(&(env[i][5]), ':');
	if (!paths)
		return (0);
	return (exec_compose2(exec, paths));
}

void	proc_starter(size_t *misc, t_cmd *p_cmd)
{
	int	fdp[2];

	fdp[0] = misc[I_FD];
	while (1)
	{
		if (dup2(fdp[0], 0) == -1 || close(fdp[0]) == -1 || pipe(fdp) == -1 \
				|| dup2(fdp[1], 1) == -1 || close(fdp[1]))
			my_exit("Pipe or dup issue", misc, p_cmd);
		misc[PID] = fork();
		if (misc[PID] < 0)
			my_exit("Fork issue", misc, p_cmd);
		else if (misc[PID] == 0)
		{
			close(fdp[0]);
			if (execve(exec_compose((p_cmd->argv)[0], (char **)misc[ENV]), \
						p_cmd->argv, (char **)misc[ENV]) == -1)
				my_exit((p_cmd->argv)[0], misc, p_cmd);
		}
		close(0);
		if (!p_cmd->next && close(1) < 1)
			break ;
		p_cmd = p_cmd->next;
	}
	misc[LST_PIPE_FD] = fdp[0];
}
