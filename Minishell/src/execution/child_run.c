/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:48:23 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/23 13:48:23 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_array(char **a)
{
	int	i;

	if (!a)
		return ;
	i = 0;
	while (a[i])
	{
		free(a[i]);
		i++;
	}
	free(a);
}

void	child_cleanup_and_exit2(t_child_ctx *c, int status)
{
	close_fd(c->prev_read);
	close_fd(c->pipe_fd[0]);
	close_fd(c->pipe_fd[1]);
	close_fd(c->heredoc_fd);
	if (c->envp_owned)
		free_str_array(c->envp);
	free_cmds_partial(c->cmds, c->cmds_n);
	exit(status);
}
