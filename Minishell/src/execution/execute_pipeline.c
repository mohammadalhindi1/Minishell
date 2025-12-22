/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:17:34 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:17:34 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(int fd)
{
	if (fd >= 0)
		close(fd);
}

static int	spawn_one_cmd(t_cmd *cmds, int i, int n, int *prev_read, pid_t *pids, char **envp)
{
	int	pipe_fd[2];

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (i < n - 1 && pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	pids[i] = fork();
	if (pids[i] == -1)
		return (perror("fork"), 1);
	if (pids[i] == 0)
		child_run(&cmds[i], i, n, *prev_read, pipe_fd, envp);
	close_fd(*prev_read);
	if (i < n - 1)
	{
		close_fd(pipe_fd[1]);
		*prev_read = pipe_fd[0];
	}
	return (0);
}

static int	wait_all_children(pid_t *pids, int n)
{
	int	i;
	int	status;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		i++;
	}
	return (g_exit_status);
}

int	execute_pipeline(t_cmd *cmds, int n, char **envp)
{
	int		i;
	int		prev_read;
	pid_t	pids[1024];

	prev_read = -1;
	i = 0;
	while (i < n)
	{
		if (spawn_one_cmd(cmds, i, n, &prev_read, pids, envp))
			return (1);
		i++;
	}
	close_fd(prev_read);
	return (wait_all_children(pids, n));
}
