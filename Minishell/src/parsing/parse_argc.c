/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:58:09 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/22 11:58:09 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

static void	copy_args(char **dst, char **src)
{
	int	i;

	i = 0;
	if (!src)
		return ;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
}

int	push_arg(t_cmd *cmd, char *tok)
{
	int		i;
	char	**newv;

	i = count_args(cmd->args);
	newv = (char **)malloc(sizeof(char *) * (i + 2));
	if (!newv)
		return (-1);
	copy_args(newv, cmd->args);
	free(cmd->args);
	newv[i] = strip_quotes_dup(tok);
	if (!newv[i])
	{
		free(newv);
		return (-1);
	}
	newv[i + 1] = NULL;
	cmd->args = newv;
	return (0);
}
