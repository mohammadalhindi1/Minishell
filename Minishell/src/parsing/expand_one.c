/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:01 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_single_quote(char *tok, int i, char **res)
{
	int		j;
	char	*part;
	char	*tmp;

	j = i + 1;
	while (tok[j] && tok[j] != '\'')
		j++;
	part = ft_substr(tok, i, j - i + 1);
	tmp = ft_strjoin(*res, part);
	free(*res);
	free(part);
	*res = tmp;
	return (j + 1);
}

static void	handle_char(char c, char **res)
{
	char	tmp[2];
	char	*joined;

	tmp[0] = c;
	tmp[1] = 0;
	joined = ft_strjoin(*res, tmp);
	free(*res);
	*res = joined;
}

char	*expand_one(char *tok, t_shell *sh)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	while (tok[i])
	{
		if (tok[i] == '\'')
			i = handle_single_quote(tok, i, &res);
		else if (tok[i] == '$')
			i += handle_dollar(tok, i, &res, sh);
		else
		{
			handle_char(tok[i], &res);
			i++;
		}
	}
	return (res);
}
