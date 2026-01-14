/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slvl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:26:02 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:26:02 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	envp_count(char **envp)
{
	size_t	count;

	if (!envp)
		return (0);
	count = 0;
	while (envp[count])
		count++;
	return (count);
}

int	is_shlvl(const char *entry)
{
	if (!entry)
		return (0);
	return (ft_strncmp(entry, "SHLVL=", 6) == 0);
}

char	*build_shlvl_entry(char *value)
{
	char	*entry;
	char	*joined;

	joined = ft_strjoin("SHLVL=", value);
	if (!joined)
		return (NULL);
	entry = joined;
	return (entry);
}

char	*next_shlvl_value(char *current)
{
	int		i;
	long	lvl;

	i = 0;
	lvl = 0;
	if (!current || !*current)
		return (ft_itoa(1));
	while (current[i] == ' ' || (current[i] >= 9 && current[i] <= 13))
		i++;
	if (current[i] == '+' || current[i] == '-')
	{
		if (current[i] == '-')
			return (ft_itoa(1));
		i++;
	}
	if (current[i] < '0' || current[i] > '9')
		return (ft_itoa(1));
	while (current[i] >= '0' && current[i] <= '9')
	{
		lvl = lvl * 10 + (current[i] - '0');
		i++;
	}
	if (current[i] != '\0')
		lvl = 0;
	return (ft_itoa((int)(lvl + 1)));
}
