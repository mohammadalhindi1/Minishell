/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:49:33 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/23 13:49:33 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipe_if_needed(int idx, int n, int pipe_fd[2])
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (idx < n - 1)
	{
		if (pipe(pipe_fd) == -1)
			return (minishell_perror("pipe"), 1);
	}
	return (0);
}

int	prepare_heredocs(t_cmd *cmds, int n, int *hd_fds)
{
	int	i;

	i = 0;
	while (i < n)
	{
		hd_fds[i] = -1;
		if (cmds[i].heredoc)
		{
			hd_fds[i] = open_heredoc_fd(cmds[i].heredoc);
			if (hd_fds[i] < 0)
				return (1);
		}
		i++;
	}
	return (0);
}

int	spawn_cmd(t_cmd *cmds, t_exec_ctx *x)
{
	t_child_ctx	c;

	if (pipe_if_needed(x->i, x->n, x->pipe_fd))
		return (1);
	x->pids[x->i] = fork();
	if (x->pids[x->i] == -1)
		return (minishell_perror("fork"), 1);
	if (x->pids[x->i] == 0)
	{
		c.idx = x->i;
		c.n = x->n;
		c.prev_read = x->prev_read;
		c.pipe_fd[0] = x->pipe_fd[0];
		c.pipe_fd[1] = x->pipe_fd[1];
		c.envp = x->envp;
		c.heredoc_fd = x->hd_fds[x->i];
		child_run(&cmds[x->i], &c);
	}
	close_fd(x->prev_read);
	if (x->i < x->n - 1)
	{
		close_fd(x->pipe_fd[1]);
		x->prev_read = x->pipe_fd[0];
	}
	return (0);
}
