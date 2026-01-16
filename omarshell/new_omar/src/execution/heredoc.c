/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:39:41 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:39:41 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	hd_child_loop(int write_fd, char *delimiter,
	t_cmd *cmds, t_exec_ctx x)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (is_delim(line, delimiter))
		{
			free(line);
			free_cmds(cmds, x.n);
			free_split(x.envp);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close_fd(write_fd);
	exit(0);
}

static void	parent_ignore_sigint(struct sigaction *oldint)
{
	struct sigaction	ign;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, oldint);
}

static pid_t	hd_fork_child(int fd[2], char *delimiter,
	t_cmd *cmds, t_exec_ctx x)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close_fd(fd[0]);
		hd_child_loop(fd[1], delimiter, cmds, x);
	}
	return (pid);
}

static int	hd_parent_wait(pid_t pid, int read_fd, struct sigaction *oldint)
{
	int	status;

	waitpid(pid, &status, 0);
	sigaction(SIGINT, oldint, NULL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close_fd(read_fd);
		g_exit_status = 130;
		return (-1);
	}
	return (read_fd);
}

int	open_heredoc_fd(char *delimiter, t_cmd *cmds, t_exec_ctx x)
{
	int					fd[2];
	pid_t				pid;
	struct sigaction	oldint;
	int					ret;

	if (!delimiter)
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	parent_ignore_sigint(&oldint);
	pid = hd_fork_child(fd, delimiter, cmds, x);
	if (pid == -1)
		return (sigaction(SIGINT, &oldint, NULL),
			close_fd(fd[0]), close_fd(fd[1]), -1);
	close_fd(fd[1]);
	ret = hd_parent_wait(pid, fd[0], &oldint);
	return (ret);
}
