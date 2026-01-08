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

static void	parent_ignore_signals(struct sigaction *oldint,
		struct sigaction *oldquit)
{
	struct sigaction	ign;

	ft_memset(&ign, 0, sizeof(ign));
	ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ign, oldint);
	sigaction(SIGQUIT, &ign, oldquit);
}

static int	restore_signals_ret(struct sigaction *oldint,
		struct sigaction *oldquit, int ret)
{
	sigaction(SIGINT, oldint, NULL);
	sigaction(SIGQUIT, oldquit, NULL);
	return (ret);
}

static int	pipeline_finalize(t_exec_ctx *x, struct sigaction *oldint,
		struct sigaction *oldquit)
{
	int	status;

	close_fd(x->prev_read);
	x->i = 0;
	while (x->i < x->n)
		close_fd(x->hd_fds[x->i++]);
	status = wait_last_status(x->pids, x->n);
	sigaction(SIGINT, oldint, NULL);
	sigaction(SIGQUIT, oldquit, NULL);
	return (status);
}

int	execute_pipeline(t_cmd *cmds, int n, char **envp)
{
	t_exec_ctx			x;
	struct sigaction	oldint;
	struct sigaction	oldquit;

	if (!cmds || n <= 0)
		return (0);
	if (n == 1 && is_builtin(&cmds[0]))
		return (g_exit_status = run_builtin(&cmds[0], envp));
	parent_ignore_signals(&oldint, &oldquit);
	x.n = n;
	x.envp = envp;
	x.prev_read = -1;
	if (prepare_heredocs(cmds, n, x.hd_fds))
		return (restore_signals_ret(&oldint, &oldquit, g_exit_status));
	x.i = 0;
	while (x.i < n)
	{
		if (spawn_cmd(cmds, &x))
			return (restore_signals_ret(&oldint, &oldquit, 1));
		x.i++;
	}
	return (pipeline_finalize(&x, &oldint, &oldquit));
}
