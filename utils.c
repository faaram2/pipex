/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 20:17:44 by ayeganya          #+#    #+#             */
/*   Updated: 2024/05/22 16:33:16 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	struct_free(t_cmd *p_cmd)
{
	int	i;

	while (1)
	{
		i = 0;
		while ((p_cmd->argv)[i] && ++i)
			free ((p_cmd->argv)[i - 1]);
		if (p_cmd->next)
		{
			p_cmd = p_cmd->next;
			free(p_cmd->prev);
		}
		else
		{
			free(p_cmd);
			break ;
		}
	}
}

void	misc_free(size_t *misc)
{
	if (misc[F1_PTR])
		free((void *)misc[F1_PTR]);
	if (misc[F2_PTR])
		free((void *)misc[F2_PTR]);
	free(misc);
}

void	my_exit(char *err_msg, size_t *misc, t_cmd *p_cmd)
{
	if (err_msg)
		perror(err_msg);
	else
		perror(" ");
	if (misc)
		misc_free(misc);
	if (p_cmd)
		struct_free(p_cmd);
	exit(1);
}

void	last_output_redirect(size_t *misc)
{
	char	*str;

	str = get_next_line(misc[LST_PIPE_FD]);
	while (str)
	{
		write(misc[O_FD], str, ft_strlen(str));
		free(str);
		str = get_next_line(misc[LST_PIPE_FD]);
	}
}
