/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:58:42 by ayeganya          #+#    #+#             */
/*   Updated: 2024/05/21 16:04:34 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_cmd
{
	char			**argv;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

# define MISC_SIZE 12

# define H_DOC 0
# define CMD_NBR 1
# define F1_PTR 2
# define F2_PTR 3
# define I_FD 4
# define O_FD 5
# define WAITPID 6
# define ENV 7
# define LST_PIPE_FD 8
# define STATUS 9
# define PID 10

#endif
