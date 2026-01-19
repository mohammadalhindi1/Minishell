/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:43 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:43 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_single_op(char *line, int *i,
		char ***tokens, int *words)
{
	t_range	r;

	r.start = *i;
	r.end = *i + 1;
	add_word_to_token(tokens, words, line, r);
	(*i)++;
}

static void	add_double_op(char *line, int *i,
		char ***tokens, int *words)
{
	t_range	r;

	r.start = *i;
	r.end = *i + 2;
	add_word_to_token(tokens, words, line, r);
	*i += 2;
}

static void	handle_less(char *line, int *i,
		char ***tokens, int *words)
{
	if (line[*i + 1] == '<')
		add_double_op(line, i, tokens, words);
	else
		add_single_op(line, i, tokens, words);
}

static void	handle_greater(char *line, int *i,
		char ***tokens, int *words)
{
	if (line[*i + 1] == '>')
		add_double_op(line, i, tokens, words);
	else
		add_single_op(line, i, tokens, words);
}

void	handle_operator(t_lexer *lx, int *i, int *start)
{
	if (lx->line[*i] == '|')
		add_single_op(lx->line, i, lx->tokens, lx->words);
	else if (lx->line[*i] == '<')
		handle_less(lx->line, i, lx->tokens, lx->words);
	else if (lx->line[*i] == '>')
		handle_greater(lx->line, i, lx->tokens, lx->words);
	*start = *i;
}
