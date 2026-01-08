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

static int	wait_last_status(pid_t *pids, int n)
{
	int		i;
	int		status;
	int		last_status;
	pid_t	last_pid;

	i = 0;
	last_status = 0;
	last_pid = pids[n - 1];
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) > 0)
		{
			if (pids[i] == last_pid)
				last_status = status;
		}
		i++;
	}
	if (WIFEXITED(last_status))
		g_exit_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		g_exit_status = 128 + WTERMSIG(last_status);
	return (g_exit_status);
}

int	execute_pipeline(t_cmd *cmds, int n, char **envp)
{
	t_exec_ctx	x;
	struct sigaction	oldint;
	struct sigaction	oldquit;
	struct sigaction	ign;

	if (!cmds || n <= 0)
		return (0);
	if (n == 1 && is_builtin(&cmds[0]))
	{
		g_exit_status = run_builtin(&cmds[0], envp);
		return (g_exit_status);
	}
	memset(&ign, 0, sizeof(ign));
	ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ign, &oldint);
	sigaction(SIGQUIT, &ign, &oldquit);
	x.n = n;
	x.envp = envp;
	x.prev_read = -1;
	if (prepare_heredocs(cmds, n, x.hd_fds))
		return (sigaction(SIGINT, &oldint, NULL),
			sigaction(SIGQUIT, &oldquit, NULL),
			g_exit_status);
	x.i = 0;
	while (x.i < n)
	{
		if (spawn_cmd(cmds, &x))
			return (sigaction(SIGINT, &oldint, NULL),
				sigaction(SIGQUIT, &oldquit, NULL),
				1);
		x.i++;
	}
	close_fd(x.prev_read);
	x.i = 0;
	while (x.i < n)
		close_fd(x.hd_fds[x.i++]);
	x.i = wait_last_status(x.pids, n);
	sigaction(SIGINT, &oldint, NULL);
	sigaction(SIGQUIT, &oldquit, NULL);
	return (x.i);
}
