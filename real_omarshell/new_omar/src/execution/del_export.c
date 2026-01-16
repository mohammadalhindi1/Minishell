/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 10:00:00 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/24 10:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count_after_unset(char **env, const char *key)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(key);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len)
			&& env[i][len] == '=')
			i++;
		else
		{
			j++;
			i++;
		}
	}
	return (j);
}

static void	env_fill_after_unset(char **env, char **newenv, const char *key)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(key);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len)
			&& env[i][len] == '=')
			free(env[i++]);
		else
			newenv[j++] = env[i++];
	}
	newenv[j] = NULL;
}

int	unset_env_manual(char ***env, const char *key)
{
	char	**newenv;
	int		j;

	j = env_count_after_unset(*env, key);
	newenv = malloc(sizeof(char *) * (j + 1));
	if (!newenv)
		return (1);
	env_fill_after_unset(*env, newenv, key);
	free(*env);
	*env = newenv;
	return (0);
}
