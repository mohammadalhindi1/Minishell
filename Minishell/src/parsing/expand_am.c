/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_am.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oaletham <oaletham@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/18 17:27:47 by oaletham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_quotes(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	is_redir_op(char *s)
{
	if (!s)
		return (0);
	if (!ft_strncmp(s, "<", 2) || !ft_strncmp(s, ">", 2))
		return (1);
	if (!ft_strncmp(s, ">>", 3))
		return (1);
	return (0);
}

void	print_ambiguous(char *tok)
{
	write(2, "minishell: ", 11);
	write(2, tok, ft_strlen(tok));
	write(2, ": ambiguous redirect\n", 21);
	g_exit_status = 1;
}

int	count_words(char *str)
{
	int	i;
	int	cnt;
	int	q;

	i = 0;
	cnt = 0;
	q = 0;
	while (str[i])
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (str[i])
			cnt++;
		while (str[i])
		{
			update_q(str[i], &q);
			if (str[i] == ' ' && q == 0)
				break ;
			i++;
		}
	}
	return (cnt);
}

char	*get_next_w(char *str, int *i)
{
	int		start;
	int		q;
	char	*word;

	q = 0;
	while (str[*i] && str[*i] == ' ')
		(*i)++;
	start = *i;
	while (str[*i])
	{
		update_q(str[*i], &q);
		if (str[*i] == ' ' && q == 0)
			break ;
		(*i)++;
	}
	word = ft_substr(str, start, *i - start);
	return (word);
}
