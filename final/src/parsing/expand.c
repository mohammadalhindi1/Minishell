/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oaletham <oaletham@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/18 17:27:47 by oaletham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**alloc_res(char **temp, int *tgt)
{
	int		i;
	int		count;
	char	**res;

	i = 0;
	count = 0;
	while (temp[i])
	{
		if (tgt[i])
			count += 1;
		else
			count += count_words(temp[i]);
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	return (res);
}

static char	**expand_to_temp(char **tokens, t_shell *sh, char **temp, int cnt)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		temp[i] = expand_one(tokens[i], sh);
		if (!temp[i])
			return (NULL);
		i++;
	}
	temp[i] = NULL;
	return (temp);
}

static int	check_ambiguous(char **tokens, char **temp, int *tgt, int cnt)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		if (tgt[i] && !has_quotes(tokens[i]) && count_words(temp[i]) != 1)
		{
			print_ambiguous(tokens[i]);
			return (-1);
		}
		i++;
	}
	return (0);
}

char	**expand_tokens(char **tokens, t_shell *sh)
{
	int		cnt;
	char	**temp;
	char	**final;
	int		*tgt;

	cnt = count_token_list(tokens);
	temp = alloc_temp(cnt);
	tgt = alloc_targets(cnt);
	if (!temp || !tgt)
	{
		free(temp);
		free(tgt);
		return (NULL);
	}
	fill_targets(tgt, tokens, cnt);
	if (!expand_to_temp(tokens, sh, temp, cnt))
		return (expand_fail(temp, cnt, tgt));
	if (check_ambiguous(tokens, temp, tgt, cnt) == -1)
		return (expand_fail(temp, cnt, tgt));
	final = split_final(temp, tgt);
	free_expanded(temp, cnt);
	free(tgt);
	return (final);
}
