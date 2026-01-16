/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:48:23 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/23 13:48:23 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Omar note:
** Fixed invalid free: cmd is part of cmds array (allocated in init_cmds),
** so we only free cmd members (args/infile/outfile/heredoc) and never free(cmd).
*/
#include "minishell.h"

static void	child_cleanup_and_exit(t_child_ctx *c, int code)
{
	if (c && c->cmds && c->cmds_n > 0)
		free_cmds(c->cmds, c->cmds_n);
	if (c && c->envp_owned && c->envp)
		free_split(c->envp);
	exit(code);
}

static void	exec_with_slash(t_cmd *cmd, t_child_ctx *c)
{
	execve(cmd->args[0], cmd->args, c->envp);
	minishell_perror(cmd->args[0]);
	if (errno == ENOENT)
		child_cleanup_and_exit(c, 127);
	child_cleanup_and_exit(c, 126);
}

static void	exec_with_path(t_cmd *cmd, t_child_ctx *c)
{
	char	*path;

	path = find_exec_path(cmd->args[0], c->envp);
	if (!path)
	{
		minishell_cmd_not_found(cmd->args[0]);
		child_cleanup_and_exit(c, 127);
	}
	execve(path, cmd->args, c->envp);
	minishell_perror("execve");
	free(path);
	child_cleanup_and_exit(c, 126);
}

void	exec_external(t_cmd *cmd, t_child_ctx *c)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		child_cleanup_and_exit(c, 0);
	if (ft_strchr(cmd->args[0], '/'))
		exec_with_slash(cmd, c);
	exec_with_path(cmd, c);
}
