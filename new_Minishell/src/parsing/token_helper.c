/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:43 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:43 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_if_word(t_lexer *lx, int start, int end)
{
	t_range	r;

	if (end > start)
	{
		r.start = start;
		r.end = end;
		add_word_to_token(lx->tokens, lx->words, lx->line, r);
	}
}

void	scan_word(char *line, int *i)
{
	while (line[*i] != '|' && line[*i] != '<'
		&& line[*i] != '>' && line[*i] != ' '
		&& line[*i] != '"' && line[*i] != 39
		&& line[*i] != '\0')
		(*i)++;
}

void	skip_spaces(char *line, int *i, int *start)
{
	while (line[*i] == ' ')
	{
		(*i)++;
		*start = *i;
	}
}

void	handle_quotes(t_lexer *lx, int *i, int *start)
{
	int		j;
	t_range	r;

	j = 1;
	while (lx->line[*i + j] != lx->line[*i])
		j++;
	r.start = *i;
	r.end = *i + j + 1;
	add_word_to_token(lx->tokens, lx->words, lx->line, r);
	*i += j + 1;
	*start = *i;
}

char	**build_final_tokens(char **tokens, int words)
{
	int		i;
	char	**final;

	final = malloc(sizeof(char *) * (words + 1));
	if (!final)
		return (NULL);
	i = 0;
	while (i < words)
	{
		final[i] = tokens[i];
		i++;
	}
	final[words] = NULL;
	free(tokens);
	return (final);
}
