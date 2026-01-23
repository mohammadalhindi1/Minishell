/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alo.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 03:34:43 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/22 03:36:15 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_closing_quote(char *s, size_t i)
{
	char	q;

	q = s[i];
	i++;
	while (s[i] && s[i] != q)
		i++;
	return (s[i] == q);
}

int	alo(char *s, size_t *i, char *q)
{
	if (!*q && (s[*i] == '"' || s[*i] == '\'') && has_closing_quote(s, *i))
	{
		*q = s[(*i)++];
		return (1);
	}
	else if (*q && s[*i] == *q)
	{
		*q = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

char	*strip_quotes_dup(char *s)
{
	size_t	i;
	size_t	j;
	char	q;
	char	*out;

	if (!s)
		return (NULL);
	out = ft_calloc(ft_strlen(s) + 1, 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	q = 0;
	while (s[i])
	{
		if (!alo(s, &i, &q))
			out[j++] = s[i++];
	}
	return (out);
}
