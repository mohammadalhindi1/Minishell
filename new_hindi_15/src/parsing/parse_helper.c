/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:58:09 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/22 11:58:09 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_op(char *t)
{
	if (!t)
		return (0);
	if (!ft_strncmp(t, "|", 2))
		return (1);
	if (!ft_strncmp(t, "<", 2) || !ft_strncmp(t, ">", 2))
		return (1);
	if (!ft_strncmp(t, "<<", 3) || !ft_strncmp(t, ">>", 3))
		return (1);
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
	out = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	q = 0;
	while (s[i])
	{
		if (!q && (s[i] == '"' || s[i] == 39))
			q = s[i++];
		else if (q && s[i] == q)
		{
			q = 0;
			i++;
		}
		else
			out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}

void	free_tokens(char **t)
{
	int	i;

	if (!t)
		return ;
	i = 0;
	while (t[i])
	{
		free(t[i]);
		i++;
	}
	free(t);
}

void	free_cmds_partial(t_cmd *cmds, int n)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (i < n)
	{
		free_tokens(cmds[i].args);
		free(cmds[i].infile);
		free(cmds[i].outfile);
		free(cmds[i].heredoc);
		i++;
	}
	free(cmds);
}

int	count_cmds(char **t)
{
	int	i;
	int	c;

	i = 0;
	c = 1;
	while (t && t[i])
	{
		if (!ft_strncmp(t[i], "|", 2))
			c++;
		i++;
	}
	return (c);
}
/*
** Omar note:
** strip_quotes_dup was updated to remove quotes anywhere in the word,
** not only when the whole string is wrapped with quotes.
*/
