/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 10:00:00 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/24 10:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_cd(t_cmd *cmd)
{
	char	*path;

	path = NULL;
	if (cmd->args[1])
		path = cmd->args[1];
	else
		path = getenv("HOME");
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (minishell_perror("cd"), 1);
	return (0);
}

static int	export_one(const char *arg)
{
	char	*eq;
	char	*key;
	int		rc;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (setenv(arg, "", 1) != 0);
	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return (1);
	rc = setenv(key, eq + 1, 1);
	free(key);
	return (rc != 0);
}

static void	print_export(char **env)
{
	int		i;
	char	*eq;

	i = 0;
	while (env[i])
	{
		write(1, "declare -x ", 11);
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			write(1, env[i], eq - env[i]);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, env[i], ft_strlen(env[i]));
			write(1, "\n", 1);
		}
		i++;
	}
}

static int	builtin_export(t_cmd *cmd, char **envp)
{
	int	i;
	int	status;

	if (!cmd->args[1])
	{
		print_export(envp); 
		return (0);
	}
	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (export_one(cmd->args[i]))
			status = 1;
		i++;
	}
	return (status);
}

static int	builtin_unset(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		unsetenv(cmd->args[i]);
		i++;
	}
	return (0);
}

static void	builtin_exit(t_cmd *cmd)
{
	int	status;

	status = g_exit_status;
	if (cmd->args[1])
		status = ft_atoi(cmd->args[1]);
	g_exit_status = status;
	exit(status);
}

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!ft_strncmp(cmd->args[0], "cd", 3))
		return (1);
	if (!ft_strncmp(cmd->args[0], "export", 7))
		return (1);
	if (!ft_strncmp(cmd->args[0], "unset", 6))
		return (1);
	if (!ft_strncmp(cmd->args[0], "exit", 5))
		return (1);
	return (0);
}

int	run_builtin(t_cmd *cmd, char **envp)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!ft_strncmp(cmd->args[0], "cd", 3))
		return (builtin_cd(cmd));
	if (!ft_strncmp(cmd->args[0], "export", 7))
		return (builtin_export(cmd, envp));
	if (!ft_strncmp(cmd->args[0], "unset", 6))
		return (builtin_unset(cmd));
	if (!ft_strncmp(cmd->args[0], "exit", 5))
		builtin_exit(cmd);
	return (0);

}
