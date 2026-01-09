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

static int	handle_exit_status(t_shell *sh, char **res)
{
	char	*val;
	char	*tmp;

	val = ft_itoa(sh->last_exit_status);
	tmp = ft_strjoin(*res, val);
	free(val);
	free(*res);
	*res = tmp;
	return (2);
}

static int	handle_env_var(char *tok, int i, char **res)
{
	int		len;
	char	*name;
	char	*env;
	char	*tmp;

	len = 0;
	while (tok[i + 1 + len]
		&& (ft_isalnum(tok[i + 1 + len]) || tok[i + 1 + len] == '_'))
		len++;
	name = ft_substr(tok, i + 1, len);
	env = getenv(name);
	if (env)
		tmp = ft_strjoin(*res, env);
	else
		tmp = ft_strjoin(*res, "");
	free(name);
	free(*res);
	*res = tmp;
	return (len + 1);
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
		{
			if (tok[i + 1] == '?')
				i += handle_exit_status(sh, &res);
			else
				i += handle_env_var(tok, i, &res);
		}
		else
		{
			handle_char(tok[i], &res);
			i++;
		}
	}
	return (res);
}
