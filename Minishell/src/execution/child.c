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

static void	safe_dup2(int from, int to)
{
	if (from >= 0 && from != to)
	{
		if (dup2(from, to) < 0)
			exit(1);
	}
}

static void	close_if_needed(int fd, int keep1, int keep2)
{
	if (fd >= 0 && fd != keep1 && fd != keep2)
		close_fd(fd);
}

static void	exec_external(t_cmd *cmd, char **envp)
{
	char	*path;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	path = find_exec_path(cmd->args[0], envp);
	if (!path)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, "\n", 1);
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror("execve");
	free(path);
	exit(126);
}

void	child_run(t_cmd *cmd, int idx, int n, int prev_read, int pipe_fd[2], char **envp)
{
	int	in_fd;
	int	out_fd;

	in_fd = get_cmd_input_fd(cmd);
	out_fd = get_cmd_output_fd(cmd);
	if (in_fd < 0 && idx > 0)
		in_fd = prev_read;
	if (out_fd < 0 && idx < n - 1)
		out_fd = pipe_fd[1];
	close_if_needed(pipe_fd[0], in_fd, out_fd);
	safe_dup2(in_fd, STDIN_FILENO);
	safe_dup2(out_fd, STDOUT_FILENO);
	close_if_needed(in_fd, STDIN_FILENO, -1);
	close_if_needed(out_fd, STDOUT_FILENO, -1);
	close_if_needed(prev_read, -1, -1);
	close_if_needed(pipe_fd[1], -1, -1);
	exec_external(cmd, envp);
}
