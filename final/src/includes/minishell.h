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
# include <limits.h>

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

typedef struct s_shell
{
	char	**env;
	int		last_exit_status;
	int		envp_owned;
}	t_shell;

/* ====== execution ====== */
typedef struct s_child_ctx
{
	int		idx;
	int		n;
	int		prev_read;
	int		pipe_fd[2];
	char	**envp;
	int		heredoc_fd;
	t_shell	*sh;

	/* Omar note: for valgrind --trace-children cleanup */
	t_cmd	*cmds;
	int		cmds_n;
	int		envp_owned;
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
	t_shell	*sh;

	/* Omar note */
	int		envp_owned;
}	t_exec_ctx;

typedef struct s_range
{
	int	start;
	int	end;
}	t_range;

/* ====== allowed single global (exit status / signals later) ====== */
extern int	g_exit_status;

// token_handle and helper
typedef struct s_lexer
{
	char	***tokens;
	int		*words;
	char	*line;
}	t_lexer;

/* ====== execution ====== */
int		execute_pipeline(t_cmd *cmds, int n, t_shell *sh, int envp_owned);
void	child_run(t_cmd *cmd, t_child_ctx *c);
void	exec_external(t_cmd *cmd, t_child_ctx *c);

/* execute helpers */
int		prepare_heredocs(t_cmd *cmds, int n, int *hd_fds, t_exec_ctx x);
int		spawn_cmd(t_cmd *cmds, t_exec_ctx *x);

/* redirections / heredoc */
int		get_cmd_input_fd(t_cmd *cmd);
int		get_cmd_output_fd(t_cmd *cmd);
int		open_heredoc_fd(char *delimiter, t_cmd *cmds, t_exec_ctx x);
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
int		run_builtin(t_cmd *cmd, t_shell *sh);
int		builtin_cd(t_cmd *cmd);
void	print_export(char **env);
int		builtin_export(t_cmd *cmd, t_shell *sh);

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

// sig
void	handle_sig(int sig);
void	setup_signals(void);

// build export
int		set_env_manual(char ***env, const char *key, const char *val);

//helper for expand $?
char	*env_get(char **env, const char *key);

//del export
int		unset_env_manual(char ***env, const char *key);

//expand_helper
void	free_expanded(char **out, int size);
void	update_q(char c, int *q);

//parse_syntax
int		redir_syntax_ok(char **t);
int		redir_target_ok(char **t);

//syntax_print
void	print_unexpected_newline(void);
void	print_unexpected(char c);

//handle dollar
int		handle_dollar(char *tok, int i, char **res, t_shell *sh);

//export one
int		export_one(const char *arg, char ***env);

//child run
void	free_str_array(char **a);
void	child_cleanup_and_exit2(t_child_ctx *c, int status);

//exit parse
int		parse_ll_strict(char *s, long long *out);

//heredoc help
void	hd_setup_child_signals(void);
void	hd_child_cleanup(int write_fd, t_cmd *cmds, t_exec_ctx x, int code);

//expand am
int		has_quotes(char *s);
int		is_redir_op(char *s);
void	print_ambiguous(char *tok);
int		count_words(char *str);
char	*get_next_w(char *str, int *i);

//expand split
char	**split_final(char **temp, int *tgt);

//expand
char	**alloc_res(char **temp, int *tgt);

//expand ov
int		count_token_list(char **tokens);
int		*alloc_targets(int cnt);
char	**alloc_temp(int cnt);
char	**expand_fail(char **temp, int cnt, int *tgt);
int		fill_targets(int *tgt, char **tokens, int cnt);

// mult_heredoc
int		open_single_heredoc_fd(char *delimiter, t_cmd *cmds, t_exec_ctx x);

#endif
