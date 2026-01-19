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

void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;
	size_t	n;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			n = ft_strlen(env[i]);
			if (ft_strlen(env[j]) > n)
				n = ft_strlen(env[j]);
			if (ft_strncmp(env[i], env[j], n) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(char **env)
{
	int		i;
	char	*eq;

	i = 0;
	sort_env(env);
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

int	builtin_export(t_cmd *cmd, t_shell *sh)
{
	int	i;
	int	status;

	if (!cmd->args[1])
	{
		print_export(sh->env);
		return (0);
	}
	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (export_one(cmd->args[i], &sh->env))
			status = 1;
		i++;
	}
	return (status);
}
