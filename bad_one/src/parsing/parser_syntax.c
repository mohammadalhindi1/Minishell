/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:58:09 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/22 11:58:09 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_bad_redir(char *s)
{
	int	i;

	if (!s || (s[0] != '<' && s[0] != '>'))
		return (0);
	i = 0;
	while (s[i] && s[i] == s[0])
		i++;
	if (s[i] != '\0')
		return (1);
	if (i > 2)
		return (1);
	return (0);
}

int	redir_syntax_ok(char **t)
{
	int	i;

	i = 0;
	while (t[i])
	{
		if (is_bad_redir(t[i]))
		{
			print_unexpected(t[i][0]);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	is_redir_tok(char *t)
{
	if (!t)
		return (0);
	if (!ft_strncmp(t, "<", 2) || !ft_strncmp(t, ">", 2))
		return (1);
	if (!ft_strncmp(t, "<<", 3) || !ft_strncmp(t, ">>", 3))
		return (1);
	return (0);
}

int	redir_target_ok(char **t)
{
	int	i;

	i = 0;
	while (t[i])
	{
		if (is_redir_tok(t[i]))
		{
			if (!t[i + 1])
				return (print_unexpected_newline(), 0);
			if (is_op(t[i + 1]))
				return (print_unexpected(t[i + 1][0]), 0);
		}
		i++;
	}
	return (1);
}
