/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:39:36 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:39:36 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_split(char **s)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
}

static char	*get_env_value(char **envp, const char *name)
{
	size_t	nlen;
	int		i;

	if (!envp || !name)
		return (NULL);
	nlen = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, nlen) && envp[i][nlen] == '=')
			return (envp[i] + nlen + 1);
		i++;
	}
	return (NULL);
}

static char	*join_path(const char *a, const char *b)
{
	char	*tmp;
	char	*out;

	tmp = ft_strjoin(a, "/");
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, b);
	free(tmp);
	return (out);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full;
	int		i;

	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (full && access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_exec_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*full;

	if (!cmd || !cmd[0])
		return (NULL);
	path_env = get_env_value(envp, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full = search_in_paths(paths, cmd);
	free_split(paths);
	return (full);
}
