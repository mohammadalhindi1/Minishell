/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:43 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:43 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_old_tokens(char **new_arr, char **old, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		new_arr[i] = old[i];
		i++;
	}
}

static void	copy_token(char *dst, char *line, t_range r)
{
	int	i;

	i = 0;
	while (i < r.end - r.start)
	{
		dst[i] = line[r.start + i];
		i++;
	}
	dst[i] = '\0';
}

void	add_word_to_token(char ***tokens, int *words,
		char *line, t_range r)
{
	char	**old;
	char	**new_arr;
	int		len;

	old = *tokens;
	len = r.end - r.start;
	new_arr = malloc(sizeof(char *) * (*words + 1));
	if (!new_arr)
		return ;
	copy_old_tokens(new_arr, old, *words);
	new_arr[*words] = malloc(len + 1);
	if (!new_arr[*words])
	{
		free(new_arr);
		return ;
	}
	copy_token(new_arr[*words], line, r);
	free(old);
	*tokens = new_arr;
	(*words)++;
}

static void	process_token_loop(t_lexer *lx, int *i, int *start)
{
	skip_spaces(lx->line, i, start);
	scan_word(lx->line, i);
	add_if_word(lx, *start, *i);
	if (lx->line[*i] == 39 || lx->line[*i] == '"')
		handle_quotes(lx, i, start);
	else if (lx->line[*i] == '|'
		|| lx->line[*i] == '<'
		|| lx->line[*i] == '>')
		handle_operator(lx, i, start);
}

char	**tokens_func(char *line)
{
	int		i;
	int		start;
	int		words;
	char	**tokens;
	t_lexer	lx;

	i = 0;
	start = 0;
	words = 0;
	tokens = NULL;
	if (check_valid(line) == 1)
		return (NULL);
	lx.line = line;
	lx.tokens = &tokens;
	lx.words = &words;
	while (line[i])
		process_token_loop(&lx, &i, &start);
	return (build_final_tokens(tokens, words));
}
