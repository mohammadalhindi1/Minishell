/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:58:09 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/22 11:58:09 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_tokens_loop(char **t, t_cmd *cmds, int total)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (t[i])
	{
		if (!ft_strncmp(t[i], "|", 2))
		{
			c++;
			i++;
			continue ;
		}
		if (is_op(t[i]))
		{
			if (apply_redir(&cmds[c], t, &i) == -1)
				return (-1);
			continue ;
		}
		if (push_arg(&cmds[c], t[i]) == -1)
			return (-1);
		i++;
	}
	(void)total;
	return (0);
}

static int	init_cmds(char **t, t_cmd **cmds, int *count)
{
	int	total;

	total = count_cmds(t);
	*cmds = (t_cmd *)ft_calloc(total, sizeof(t_cmd));
	if (!*cmds)
		return (-1);
	*count = total;
	return (0);
}

static int	parse_error(char **t, t_cmd **cmds, int *count)
{
	free_tokens(t);
	free_cmds_partial(*cmds, *count);
	*cmds = NULL;
	*count = 0;
	return (-1);
}

int	parse_line(char *line, t_cmd **cmds, int *count, t_shell *sh)
{
	char	**t;
	char	**expanded;

	if (!line || !cmds || !count)
		return (-1);
	t = tokens_func(line);
	if (!t)
		return (-1);
	expanded = expand_tokens(t, sh);
	free_tokens(t);
	if (!expanded)
		return (-1);
	t = expanded;
	if (!pipe_syntax_ok(t))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		g_exit_status = 258;
		return (free_tokens(t), -1);
	}
	if (init_cmds(t, cmds, count) == -1)
		return (free_tokens(t), -1);
	if (parse_tokens_loop(t, *cmds, *count) == -1)
		return (parse_error(t, cmds, count));
	free_tokens(t);
	return (0);
}
