/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 10:00:00 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/24 10:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_name_start(char c)
{
	return ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z') || c == '_');
}

static int	is_name_char(char c)
{
	return (is_name_start(c) || (c >= '0' && c <= '9'));
}

static int	valid_key(const char *s, int len)
{
	int	i;

	if (!s || len <= 0 || !is_name_start(s[0]))
		return (0);
	i = 1;
	while (i < len)
	{
		if (!is_name_char(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	export_error(const char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

int	export_one(const char *arg, char ***env)
{
	char	*eq;
	char	*key;
	int		rc;
	int		len;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		if (!valid_key(arg, (int)ft_strlen(arg)))
			return (export_error(arg));
		return (set_env_manual(env, arg, "") != 0);
	}
	len = (int)(eq - arg);
	if (!valid_key(arg, len))
		return (export_error(arg));
	key = ft_substr(arg, 0, len);
	if (!key)
		return (1);
	rc = set_env_manual(env, key, eq + 1);
	free(key);
	return (rc != 0);
}
