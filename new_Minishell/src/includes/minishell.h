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

typedef struct s_shell
{
	int	last_exit_status;
}	t_shell;

typedef struct s_cmd
{
	char	**args;/* argv, args[0] = command */
	char	*infile;/* < */
	char	*outfile;/* > or >> */
	int		append;/* 0 truncate, 1 append */
	char	*heredoc;/* << delimiter or NULL */
}	t_cmd;

typedef struct s_range
{
	int	start;
	int	end;
}	t_range;

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

// token_handle and helper
typedef struct s_lexer
{
	char	***tokens;
	int		*words;
	char	*line;
}	t_lexer;

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

//this too it was : int		parse_line(char *line, t_cmd **cmds, int *count);
int		parse_line(char *line, t_cmd **cmds, int *count, t_shell *sh);

// omar added this 
char	**expand_tokens(char **tokens, t_shell *sh);
char	*expand_one(char *tok, t_shell *sh);

int		is_builtin(t_cmd *cmd);
int		run_builtin(t_cmd *cmd, char **envp);
int		builtin_cd(t_cmd *cmd);
int		export_one(const char *arg);
void	print_export(char **env);
int		builtin_export(t_cmd *cmd, char **envp);

//free
void	free_split(char **s);
void	free_cmds(t_cmd *cmds, int n);

//slvl
size_t	envp_count(char **envp);
int		is_shlvl(const char *entry);
char	*build_shlvl_entry(char *value);
char	*next_shlvl_value(char *current);

//slvl2
char	**update_shlvl(char **envp);

//expand one
char	*expand_one(char *tok, t_shell *sh);

//token helper
void	add_if_word(t_lexer *lx, int start, int end);
void	scan_word(char *line, int *i);
void	skip_spaces(char *line, int *i, int *start);
void	handle_quotes(t_lexer *lx, int *i, int *start);
char	**build_final_tokens(char **tokens, int words);

// token handle
void	handle_operator(t_lexer *lx, int *i, int *start);

// token
void	add_word_to_token(char ***tokens, int *words, char *line, t_range r);

//parse args
int		push_arg(t_cmd *cmd, char *tok);

//parse helper
int		is_op(char *t);
char	*strip_quotes_dup(char *s);
void	free_tokens(char **t);
void	free_cmds_partial(t_cmd *cmds, int n);
int		count_cmds(char **t);

//parse redir
int		apply_redir(t_cmd *cmd, char **t, int *i);

//pipe handle
int		pipe_syntax_ok(char **t);


#endif
