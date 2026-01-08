/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 10:00:00 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/24 10:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_cmd *cmd)
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
	{
		minishell_perror("cd");
		return (1);
	}
	return (0);
}

int	export_one(const char *arg)
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

void	print_export(char **env)
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

int	builtin_export(t_cmd *cmd, char **envp)
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
