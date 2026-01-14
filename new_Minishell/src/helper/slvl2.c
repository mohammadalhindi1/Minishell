/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slvl2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:41:21 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:41:21 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_shlvl_entry(char *value)
{
	char	*next_value;
	char	*entry;

	next_value = next_shlvl_value(value);
	entry = NULL;
	if (next_value)
		entry = build_shlvl_entry(next_value);
	free(next_value);
	return (entry);
}

static char	*copy_env_entry(char *src, int *found)
{
	char	*entry;

	if (!*found && is_shlvl(src))
	{
		entry = make_shlvl_entry(src + 6);
		if (entry)
		{
			*found = 1;
			return (entry);
		}
	}
	return (ft_strdup(src));
}

static char	**shlvl_fail(char **copy, char **envp)
{
	free_split(copy);
	return (envp);
}

static size_t	copy_env_with_shlvl(char **copy, char **envp, size_t count)
{
	size_t	i;
	int		found;

	i = 0;
	found = 0;
	while (i < count)
	{
		copy[i] = copy_env_entry(envp[i], &found);
		if (!copy[i])
			return ((size_t) - 1);
		i++;
	}
	if (!found)
	{
		copy[i] = make_shlvl_entry(NULL);
		if (copy[i])
			i++;
	}
	copy[i] = NULL;
	return (i);
}

char	**update_shlvl(char **envp)
{
	size_t	count;
	size_t	i;
	char	**copy;

	count = envp_count(envp);
	copy = malloc(sizeof(char *) * (count + 2));
	if (!copy)
		return (envp);
	i = copy_env_with_shlvl(copy, envp, count);
	if (i == (size_t)-1)
		return (shlvl_fail(copy, envp));
	return (copy);
}
