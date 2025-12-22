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

/* ====== system headers ====== */
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

/* ====== readline ====== */
# include <readline/readline.h>
# include <readline/history.h>

/* ====== libft ====== */
# include "libft.h"

/* Omar, read this :
Your parsing should only transform the input line into t_cmd[] + cmd_count.
Execution logic (pipes, fork, dup2, execve, heredoc, redirections) is handled on my side via:
execute_pipeline(cmds, cmd_count, envp);
No execution assumptions are needed in parsing — just fill this structure correctly.
Once t_cmd[] is ready, integration is a single call.
*/

typedef struct s_cmd
{
	char	**args;      /* argv, args[0] = command */
	char	*infile;     /* < */
	char	*outfile;    /* > or >> */
	int		append;     /* 0 truncate, 1 append */
	char	*heredoc;    /* << delimiter or NULL */
}	t_cmd;

/* ====== allowed single global (exit status / signals later) ====== */
extern int	g_exit_status;

/* ====== execution ====== */
int		execute_pipeline(t_cmd *cmds, int n, char **envp);
void	child_run(t_cmd *cmd, int idx, int n, int prev_read, int pipe_fd[2], char **envp);

/* redirections / heredoc */
int		get_cmd_input_fd(t_cmd *cmd);
int		get_cmd_output_fd(t_cmd *cmd);
int		open_heredoc_fd(char *delimiter);

/* path */
char	*find_exec_path(char *cmd, char **envp);

/* small utils */
void	close_fd(int fd);

int		check_valid(char *line);
char	**tokens_func(char *line);

int parse_line(char *line, t_cmd **cmds, int *count);

#endif
