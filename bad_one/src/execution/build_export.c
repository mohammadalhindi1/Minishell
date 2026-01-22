/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 10:00:00 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/24 10:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_find(char **env, const char *key)
{
	int	i;
	int	len;

	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	env_replace(char **env, char *entry, const char *key)
{
	int	i;

	i = env_find(env, key);
	if (i < 0)
		return (0);
	free(env[i]);
	env[i] = entry;
	return (1);
}

static int	env_append(char ***env, char *entry)
{
	char	**newenv;
	int		n;

	n = 0;
	while ((*env)[n])
		n++;
	newenv = malloc(sizeof(char *) * (n + 2));
	if (!newenv)
		return (free(entry), 1);
	n = 0;
	while ((*env)[n])
	{
		newenv[n] = (*env)[n];
		n++;
	}
	newenv[n++] = entry;
	newenv[n] = NULL;
	free(*env);
	*env = newenv;
	return (0);
}

static char	*ft_strjoin3(const char *a, const char *b, const char *c)
{
	char	*res;
	size_t	len;

	len = ft_strlen(a) + ft_strlen(b) + ft_strlen(c);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, a, len + 1);
	ft_strlcat(res, b, len + 1);
	ft_strlcat(res, c, len + 1);
	return (res);
}

int	set_env_manual(char ***env, const char *key, const char *val)
{
	char	*entry;

	entry = ft_strjoin3(key, "=", val);
	if (!entry)
		return (1);
	if (env_replace(*env, entry, key))
		return (0);
	return (env_append(env, entry));
}
