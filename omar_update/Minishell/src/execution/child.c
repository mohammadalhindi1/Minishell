/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:35:44 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:35:44 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_if_needed(int fd, int keep1, int keep2)
{
	if (fd >= 0 && fd != keep1 && fd != keep2)
		close_fd(fd);
}

static int	resolve_in_fd(t_cmd *cmd, t_child_ctx *c)
{
	int	in_fd;

	in_fd = -1;
	if (c->heredoc_fd >= 0)
		in_fd = c->heredoc_fd;
	else if (cmd && cmd->infile)
		in_fd = get_cmd_input_fd(cmd);
	else if (c->idx > 0)
		in_fd = c->prev_read;
	if (cmd && cmd->infile && in_fd < 0)
		exit(1);
	return (in_fd);
}

static int	resolve_out_fd(t_cmd *cmd, t_child_ctx *c)
{
	int	out_fd;

	out_fd = -1;
	if (cmd && cmd->outfile)
		out_fd = get_cmd_output_fd(cmd);
	else if (c->idx < c->n - 1)
		out_fd = c->pipe_fd[1];
	if (cmd && cmd->outfile && out_fd < 0)
		exit(1);
	return (out_fd);
}

void	child_run(t_cmd *cmd, t_child_ctx *c)
{
	int	in_fd;
	int	out_fd;

	in_fd = resolve_in_fd(cmd, c);
	out_fd = resolve_out_fd(cmd, c);
	close_if_needed(c->pipe_fd[0], in_fd, out_fd);
	safe_dup2(in_fd, STDIN_FILENO);
	safe_dup2(out_fd, STDOUT_FILENO);
	close_if_needed(in_fd, STDIN_FILENO, -1);
	close_if_needed(out_fd, STDOUT_FILENO, -1);
	close_if_needed(c->prev_read, -1, -1);
	close_if_needed(c->pipe_fd[1], -1, -1);
	exec_external(cmd, c->envp);
}
