/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:26:07 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:26:07 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **s)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

void	free_cmds(t_cmd *cmds, int n)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (i < n)
	{
		free_split(cmds[i].args);
		free(cmds[i].infile);
		free(cmds[i].outfile);
		free(cmds[i].heredoc);
		i++;
	}
	free(cmds);
}

int	pipe_syntax_ok(char **t)
{
	int	i;

	if (!t || !t[0])
		return (1);
	if (!ft_strncmp(t[0], "|", 2))
		return (0);
	i = 0;
	while (t[i])
	{
		if (!ft_strncmp(t[i], "|", 2) && !t[i + 1])
			return (0);
		if (!ft_strncmp(t[i], "|", 2) && !ft_strncmp(t[i + 1], "|", 2))
			return (0);
		i++;
	}
	return (1);
}
