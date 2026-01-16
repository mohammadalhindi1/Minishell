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

static int	is_str_numeric(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_exit_cmd(t_cmd *cmd, t_shell *sh)
{
	long	status;

	status = sh->last_exit_status;
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		g_exit_status = 1;
		return (1);
	}
	if (cmd->args[1])
	{
		if (!is_str_numeric(cmd->args[1]))
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			g_exit_status = 2;
			return (0);
		}
		status = (long)ft_atoi(cmd->args[1]);
		g_exit_status = (unsigned char)status;
	}
	write(1, "exit\n", 5);
	rl_clear_history();
	return (0);
}

static int	run_line(char *line, t_shell *sh)
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
		if (count == 1 && cmds && cmds[0].args && cmds[0].args[0]
			&& !ft_strncmp(cmds[0].args[0], "exit", 5))
		{
			if (handle_exit_cmd(&cmds[0], sh) == 0)
			{
				free_cmds(cmds, count);
				return (0);
			}
			free_cmds(cmds, count);
			return (1);
		}
		execute_pipeline(cmds, count, sh, sh->envp_owned);
		free_cmds(cmds, count);
	}
	return (1);
}

static int	shell_loop(t_shell *sh)
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
	if (run_line(line, sh) == 0)
	{
		free(line);
		return (0);
	}
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
	sh.envp_owned = 1;
	if (!shell_envp)
	{
		shell_envp = envp;
		sh.envp_owned = 0;
	}
	sh.last_exit_status = 0;
	sh.env = shell_envp;
	while (shell_loop(&sh))
		;
	if (sh.envp_owned)
		free_split(sh.env);
	return (g_exit_status);
}
