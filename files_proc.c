/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_fixed.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:57:28 by ayeganya          #+#    #+#             */
/*   Updated: 2024/05/26 14:10:53 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	my_exit(char *err_msg, size_t *misc, t_cmd *p_cmd);

void	read_to_pipe(int pipe_fd, char *delim)
{
	char	*str;
	char	*delim_n;

	delim_n = ft_strjoin(delim, "\n");
	write(1, "heredoc>", 8);
	str = get_next_line(0);
	while (str && ft_strcmp(delim, str) && ft_strcmp(delim_n, str))
	{
		write(pipe_fd, str, ft_strlen(str));
		free(str);
		write(1, "heredoc>", 8);
		str = get_next_line(0);
	}
}

void	files_to_fd2(size_t *misc, t_cmd *p_cmd)
{
	int	fdp[2];

	if (misc[F2_PTR])
	{
		if (!misc[H_DOC])
			misc[O_FD] = (size_t)open((char *)misc[F2_PTR], O_WRONLY | \
					O_CREAT | O_TRUNC, 0744);
		else
			misc[O_FD] = (size_t)open((char *)misc[F2_PTR], O_WRONLY | \
					O_CREAT | O_APPEND, 0744);
	}
	else
	{
		if (pipe(fdp) == -1)
			my_exit("Initial pipe", misc, p_cmd);
		misc[O_FD] = fdp[1];
	}
}

void	files_to_fd(size_t *misc, t_cmd *p_cmd)
{
	int	fdp[2];

	if (misc[H_DOC])
	{
		if (pipe(fdp) == -1)
			my_exit("Initial pipe", misc, p_cmd);
		read_to_pipe(fdp[1], (char *)misc[F1_PTR]);
		close(fdp[1]);
		misc[I_FD] = fdp[0];
	}
	else
	{
		if (misc[F1_PTR])
			misc[I_FD] = open((char *)misc[F1_PTR], O_RDONLY);
		else
		{
			if (pipe(fdp) == -1)
				my_exit("Initial pipe", misc, p_cmd);
			close(fdp[1]);
			misc[I_FD] = fdp[0];
		}
	}
	files_to_fd2(misc, p_cmd);
}

void	end_trim(size_t *misc, t_cmd *p_cmd)
{
	int		i;
	t_cmd	*iter;

	i = 0;
	iter = p_cmd;
	while (iter->next)
		iter = iter->next;
	while ((iter->argv)[i])
		free((iter->argv)[i++]);
	iter->prev->next = 0;
	free(iter);
	misc[CMD_NBR]--;
	misc[F2_PTR] = 0;
}

void	check_files(size_t *misc, t_cmd **p_p_cmd)
{
	int	i;

	if (!misc[H_DOC] && access((char *)misc[F1_PTR], R_OK) == -1)
	{
		perror("Unable to read input file");
		i = 0;
		while (((*p_p_cmd)->argv)[i])
			free(((*p_p_cmd)->argv)[i++]);
		*p_p_cmd = (*p_p_cmd)->next;
		free((*p_p_cmd)->prev);
		misc[CMD_NBR]--;
		misc[F1_PTR] = 0;
	}
	if (access((char *)misc[F2_PTR], F_OK) == 0 && \
			access((char *)misc[F2_PTR], W_OK) == -1)
	{
		perror("Unable to write to output file");
		end_trim(misc, *p_p_cmd);
	}
}
