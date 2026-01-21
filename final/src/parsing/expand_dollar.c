/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:01 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	handle_env_var(char *tok, int i, char **res, t_shell *sh)
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
	env = env_get(sh->env, name);
	if (env)
		tmp = ft_strjoin(*res, env);
	else
		tmp = ft_strjoin(*res, "");
	free(name);
	free(*res);
	*res = tmp;
	return (len + 1);
}

static int	handle_positional(char c, char **res)
{
	char	*tmp;

	if (c != '0')
		return (2);
	tmp = ft_strjoin(*res, "minishell");
	if (!tmp)
		return (2);
	free(*res);
	*res = tmp;
	return (2);
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

int	handle_dollar(char *tok, int i, char **res, t_shell *sh)
{
	char	c;

	c = tok[i + 1];
	if (!c)
		return (handle_char('$', res), 1);
	if (c == '?')
		return (handle_exit_status(sh, res));
	if (c >= '0' && c <= '9')
		return (handle_positional(c, res));
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
		return (handle_env_var(tok, i, res, sh));
	return (handle_char('$', res), 1);
}
