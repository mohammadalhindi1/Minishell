/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oaletham <oaletham@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/18 17:27:47 by oaletham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_str_array_local(char **a)
{
	int	i;

	if (!a)
		return ;
	i = 0;
	while (a[i])
	{
		free(a[i]);
		i++;
	}
	free(a);
}

static int	split_push_target(char **res, int *k, char **temp, int i)
{
	res[*k] = temp[i];
	temp[i] = NULL;
	(*k)++;
	return (0);
}

static int	split_push_words(char **res, int *k, char *s)
{
	int		idx;
	char	*w;

	idx = 0;
	while (count_words(s + idx))
	{
		w = get_next_w(s, &idx);
		if (!w)
			return (-1);
		res[*k] = w;
		(*k)++;
	}
	return (0);
}

static char	**split_fail(char **res, int k)
{
	res[k] = NULL;
	free_str_array_local(res);
	return (NULL);
}

char	**split_final(char **temp, int *tgt)
{
	char	**res;
	int		i;
	int		k;

	res = alloc_res(temp, tgt);
	if (!res)
		return (NULL);
	i = 0;
	k = 0;
	while (temp[i])
	{
		if (tgt[i])
			split_push_target(res, &k, temp, i);
		else if (split_push_words(res, &k, temp[i]) == -1)
			return (split_fail(res, k));
		i++;
	}
	res[k] = NULL;
	return (res);
}
