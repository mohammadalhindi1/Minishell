/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:01 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_expanded(char **out, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(out[i]);
		i++;
	}
	free(out);
}

static int	count_tokens(char **tokens)
{
	int	count;

	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

char	**expand_tokens(char **tokens, t_shell *sh)
{
	int		i;
	int		count;
	char	**out;

	i = 0;
	count = count_tokens(tokens);
	out = malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (NULL);
	while (i < count)
	{
		out[i] = expand_one(tokens[i], sh);
		if (!out[i])
		{
			free_expanded(out, i);
			return (NULL);
		}
		i++;
	}
	out[count] = NULL;
	return (out);
}
