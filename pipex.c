/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:46:15 by ayeganya          #+#    #+#             */
/*   Updated: 2024/05/26 15:14:28 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	my_exit(char *err_msg, size_t *misc, t_cmd *p_cmd);
void	misc_free(size_t *misc);
void	struct_free(t_cmd *p_cmd);
void	last_output_redirect(size_t *misc);
void	proc_starter(size_t *misc, t_cmd *p_cmd);
void	check_files(size_t *misc, t_cmd **p_p_cmd);
void	files_to_fd(size_t *misc, t_cmd *p_cmd);

void	cmd_init(t_cmd **p_p_cmd, char **argv, size_t *misc)
{
	t_cmd	*iter;
	size_t	i;

	i = 2 + misc[H_DOC];
	*p_p_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!(*p_p_cmd))
		my_exit("struct malloc", misc, *p_p_cmd);
	iter = *p_p_cmd;
	iter->prev = 0;
	while (1)
	{
		iter->argv = ft_split(argv[i++], ' ');
		if (!iter->argv)
			my_exit("split malloc", misc, *p_p_cmd);
		if (i >= misc[CMD_NBR] + misc[H_DOC] + 2)
			break ;
		iter->next = (t_cmd *)malloc(sizeof(t_cmd));
		if (!(iter->next))
			my_exit("struct malloc", misc, *p_p_cmd);
		iter->next->prev = iter;
		iter = iter->next;
	}
	iter->next = 0;
}

void	init_init(size_t **p_misc, int argc, char **argv, char **env)
{
	if (argc < 5 || (!ft_strcmp("here_doc", argv[1]) && argc < 6))
		my_exit("not enough arguments", 0, 0);
	*p_misc = (size_t *)malloc(MISC_SIZE * sizeof(size_t));
	if (!(*p_misc))
		my_exit("MISC malloc", 0, 0);
	if (!ft_strcmp("here_doc", argv[1]))
		(*p_misc)[H_DOC] = 1;
	else
		(*p_misc)[H_DOC] = 0;
	(*p_misc)[CMD_NBR] = argc - (*p_misc)[H_DOC] - 3;
	(*p_misc)[F1_PTR] = (size_t)malloc((ft_strlen(argv[(*p_misc)[H_DOC] + \
					1]) + 3) * sizeof(char));
	(*p_misc)[F2_PTR] = (size_t)malloc((ft_strlen(argv[argc - 1]) + 3) * \
			sizeof(char));
	if (!(*p_misc)[F1_PTR] || !(*p_misc)[F2_PTR])
		my_exit("F1/F2", *p_misc, 0);
	ft_strlcpy((char *)(*p_misc)[F1_PTR], argv[(*p_misc)[H_DOC] + 1], \
			ft_strlen(argv[(*p_misc)[H_DOC] + 1]) + 1);
	ft_strlcpy((char *)(*p_misc)[F2_PTR], argv[argc -1], \
			ft_strlen(argv[argc - 1]) + 1);
	(*p_misc)[ENV] = (size_t)env;
}

int	main(int argc, char *argv[], char *env[])
{
	size_t	*misc;
	t_cmd	*p_cmd;

	init_init(&misc, argc, argv, env);
	cmd_init(&p_cmd, argv, misc);
	check_files(misc, &p_cmd);
	files_to_fd(misc, p_cmd);
	proc_starter(misc, p_cmd);
	last_output_redirect(misc);
	misc[WAITPID] = wait((int *)&misc[STATUS]);
	while ((long)misc[WAITPID] > 0)
		misc[WAITPID] = wait((int *)&misc[STATUS]);
	misc_free(misc);
	struct_free(p_cmd);
}
