/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:31:28 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 12:31:28 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "libft.h"

typedef struct s_cmd
{
	char	**args;/* argv, args[0] = command */
	char	*infile;/* < */
	char	*outfile;/* > or >> */
	int		append;/* 0 truncate, 1 append */
	char	*heredoc;/* << delimiter or NULL */
}	t_cmd;

/* ====== allowed single global (exit status / signals later) ====== */
extern int	g_exit_status;

/* ====== execution ====== */
typedef struct s_child_ctx
{
	int		idx;
	int		n;
	int		prev_read;
	int		pipe_fd[2];
	char	**envp;
	int		heredoc_fd;
}	t_child_ctx;

typedef struct s_exec_ctx
{
	int		i;
	int		n;
	int		prev_read;
	int		pipe_fd[2];
	pid_t	pids[1024];
	int		hd_fds[1024];
	char	**envp;
}	t_exec_ctx;

/* ====== execution ====== */
int		execute_pipeline(t_cmd *cmds, int n, char **envp);
void	child_run(t_cmd *cmd, t_child_ctx *c);
void	exec_external(t_cmd *cmd, char **envp);

/* execute helpers */
int		prepare_heredocs(t_cmd *cmds, int n, int *hd_fds);
int		spawn_cmd(t_cmd *cmds, t_exec_ctx *x);

/* redirections / heredoc */
int		get_cmd_input_fd(t_cmd *cmd);
int		get_cmd_output_fd(t_cmd *cmd);
int		open_heredoc_fd(char *delimiter);
int		is_delim(char *line, char *delim);

char	*find_exec_path(char *cmd, char **envp);

void	close_fd(int fd);
void	minishell_perror(char *name);
void	minishell_cmd_not_found(char *cmd);
void	safe_dup2(int from, int to);

/* parsing bridge */
int		check_valid(char *line);
char	**tokens_func(char *line);
int		parse_line(char *line, t_cmd **cmds, int *count);

#endif
