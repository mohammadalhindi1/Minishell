/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:48:23 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/23 13:48:23 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_with_slash(t_cmd *cmd, char **envp)
{
	execve(cmd->args[0], cmd->args, envp);
	minishell_perror(cmd->args[0]);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

// omar add

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_args(cmd->args);
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd->heredoc);
	free(cmd);
}

// omar add

static void	exec_with_path(t_cmd *cmd, char **envp)
{
	char	*path;

	path = find_exec_path(cmd->args[0], envp);
	if (!path)
	{
		minishell_cmd_not_found(cmd->args[0]);
		free_cmd(cmd);
		free_args(envp);
		exit(127);
	}
	execve(path, cmd->args, envp);
	minishell_perror("execve");
	free(path);
	exit(126);
}

void	exec_external(t_cmd *cmd, char **envp)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	if (ft_strchr(cmd->args[0], '/'))
		exec_with_slash(cmd, envp);
	exec_with_path(cmd, envp);
}
