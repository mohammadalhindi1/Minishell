/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oaletham <oaletham@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 10:00:00 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/18 17:33:04 by oaletham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_missing(char *arg)
{
	if (arg && arg[0] == '-' && arg[1] == '\0')
		write(2, "minishell: cd: OLDPWD not set\n", 30);
	else
		write(2, "minishell: cd: HOME not set\n", 28);
	return (1);
}

static char	*cd_expand_tilde(char *arg)
{
	char	*home;
	char	*path;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	if (arg[1] == '\0')
		return (ft_strdup(home));
	path = ft_strjoin(home, arg + 1);
	return (path);
}

static char	*cd_resolve(char *arg, int *alloc)
{
	char	*val;

	*alloc = 0;
	if (!arg)
		return (getenv("HOME"));
	if (arg[0] == '~')
	{
		*alloc = 1;
		return (cd_expand_tilde(arg));
	}
	if (arg[0] == '-' && arg[1] == '\0')
	{
		val = getenv("OLDPWD");
		if (!val)
			return (NULL);
		*alloc = 1;
		return (ft_strdup(val));
	}
	return (arg);
}

static int	cd_too_many(void)
{
	write(2, "minishell: cd: too many arguments\n", 34);
	return (1);
}

int	builtin_cd(t_cmd *cmd)
{
	char	*path;
	int		alloc;

	if (cmd->args[1] && cmd->args[2])
		return (cd_too_many());
	path = cd_resolve(cmd->args[1], &alloc);
	if (!path)
		return (cd_missing(cmd->args[1]));
	if (chdir(path) != 0)
	{
		if (alloc)
			free(path);
		minishell_perror("cd");
		return (1);
	}
	if (cmd->args[1] && cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
	{
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	if (alloc)
		free(path);
	return (0);
}
