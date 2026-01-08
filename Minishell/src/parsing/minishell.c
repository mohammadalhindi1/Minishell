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

static int	is_op(char *t)
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

static char	*strip_quotes_dup(char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"')
			|| (s[0] == 39 && s[len - 1] == 39)))
		return (ft_substr(s, 1, len - 2));
	return (ft_strdup(s));
}

static void	free_tokens(char **t)
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

static void	free_cmds_partial(t_cmd *cmds, int n)
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

static int	count_cmds(char **t)
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

static int	push_arg(t_cmd *cmd, char *tok)
{
	int		i;
	char	**newv;

	i = 0;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	newv = (char **)malloc(sizeof(char *) * (i + 2));
	if (!newv)
		return (-1);
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			newv[i] = cmd->args[i];
			i++;
		}
		free(cmd->args);
	}
	newv[i] = strip_quotes_dup(tok);
	if (!newv[i])
		return (free(newv), -1);
	newv[i + 1] = NULL;
	cmd->args = newv;
	return (0);
}

static int	apply_redir(t_cmd *cmd, char **t, int *i)
{
	char	*op;
	char	*val;

	op = t[*i];
	if (!t[*i + 1])
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n",
			56);
		g_exit_status = 258;
		return (-1);
	}
	val = strip_quotes_dup(t[*i + 1]);
	if (!val)
		return (-1);
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
		free(cmd->outfile);
		cmd->outfile = val;
		cmd->append = 0;
	}
	else if (!ft_strncmp(op, ">>", 3))
	{
		free(cmd->outfile);
		cmd->outfile = val;
		cmd->append = 1;
	}
	else
		free(val);
	*i += 2;
	return (0);
}

int	parse_line(char *line, t_cmd **cmds, int *count, t_shell *sh)
{
	char	**t;
	char	**expanded;
	int		i;
	int		c;
	int		total;

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
	c = count_cmds(t);
	*cmds = (t_cmd *)ft_calloc(c, sizeof(t_cmd));
	if (!*cmds)
		return (free_tokens(t), -1);
	total = c;
	*count = c;
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
			if (apply_redir(&(*cmds)[c], t, &i) == -1)
			{
				free_tokens(t);
				free_cmds_partial(*cmds, total);
				*cmds = NULL;
				*count = 0;
				return (-1);
			}
			continue ;
		}
		if (push_arg(&(*cmds)[c], t[i]) == -1)
		{
			free_tokens(t);
			free_cmds_partial(*cmds, total);
			*cmds = NULL;
			*count = 0;
			return (-1);
		}
		i++;
	}
	free_tokens(t);
	return (0);
}
