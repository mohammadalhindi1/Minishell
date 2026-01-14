/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:41:21 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:41:21 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_sig(int sig)
{
	g_exit_status = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sig;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static void	run_line(char *line, char **shell_envp, t_shell *sh)
{
	t_cmd	*cmds;
	int		count;

	if (*line)
		add_history(line);
	cmds = NULL;
	count = 0;
	sh->last_exit_status = g_exit_status;
	if (parse_line(line, &cmds, &count, sh) == 0)
	{
		execute_pipeline(cmds, count, shell_envp);
		free_cmds(cmds, count);
	}
}

static int	shell_loop(char **shell_envp, t_shell *sh)
{
	char	*line;

	line = readline("minishell% ");
	if (!line)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	if (!*line)
	{
		free(line);
		return (1);
	}
	run_line(line, shell_envp, sh);
	free(line);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	**shell_envp;
	t_shell	sh;

	(void)ac;
	(void)av;
	setup_signals();
	shell_envp = update_shlvl(envp);
	sh.last_exit_status = 0;
	while (shell_loop(shell_envp, &sh))
		;
	if (shell_envp && shell_envp != envp)
		free_split(shell_envp);
	return (0);
}
