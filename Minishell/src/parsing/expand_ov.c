/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ov.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oaletham <oaletham@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/18 17:27:47 by oaletham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_token_list(char **tokens)
{
	int	cnt;

	cnt = 0;
	while (tokens[cnt])
		cnt++;
	return (cnt);
}

int	*alloc_targets(int cnt)
{
	return ((int *)ft_calloc(cnt, sizeof(int)));
}

char	**alloc_temp(int cnt)
{
	return ((char **)ft_calloc(cnt + 1, sizeof(char *)));
}

char	**expand_fail(char **temp, int cnt, int *tgt)
{
	free_expanded(temp, cnt);
	free(tgt);
	return (NULL);
}

int	fill_targets(int *tgt, char **tokens, int cnt)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		tgt[i] = 0;
		if (i > 0 && is_redir_op(tokens[i - 1]))
			tgt[i] = 1;
		i++;
	}
	return (0);
}
