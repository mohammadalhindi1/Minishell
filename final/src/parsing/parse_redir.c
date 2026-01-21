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

static int	append_nl(char **dst, char *val)
{
	char	*tmp;
	char	*res;

	if (!val)
		return (0);
	if (!*dst)
	{
		*dst = val;
		return (1);
	}
	tmp = ft_strjoin(*dst, "\n");
	if (!tmp)
		return (free(val), 0);
	res = ft_strjoin(tmp, val);
	free(tmp);
	free(val);
	if (!res)
		return (0);
	free(*dst);
	*dst = res;
	return (1);
}

static void	apply_redir_value(t_cmd *cmd, char *op, char *val)
{
	if (!ft_strncmp(op, "<", 2))
	{
		free(cmd->infile);
		cmd->infile = val;
	}
	else if (!ft_strncmp(op, "<<", 3))
	{
		free(cmd->heredoc);
		cmd->heredoc = val;
	}
	else if (!ft_strncmp(op, ">", 2))
	{
		cmd->append = 0;
		if (!append_nl(&cmd->outfile, val))
			minishell_perror("malloc");
	}
	else if (!ft_strncmp(op, ">>", 3))
	{
		cmd->append = 1;
		if (!append_nl(&cmd->outfile, val))
			minishell_perror("malloc");
	}
	else
		free(val);
}

int	apply_redir(t_cmd *cmd, char **t, int *i)
{
	char	*op;
	char	*val;

	op = t[*i];
	if (!t[*i + 1])
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n",
			56);
		g_exit_status = 2;
		return (-1);
	}
	val = strip_quotes_dup(t[*i + 1]);
	if (!val)
		return (-1);
	apply_redir_value(cmd, op, val);
	*i += 2;
	return (0);
}
