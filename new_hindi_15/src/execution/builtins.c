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
