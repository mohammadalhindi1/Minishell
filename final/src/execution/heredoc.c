/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:39:41 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/19 00:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hd_open_part(char *p, char *nl, t_cmd *cmds, t_exec_ctx x)
{
	int	tmp;

	if (!p || *p == '\0')
		return (-1);
	if (nl)
		*nl = '\0';
	tmp = open_single_heredoc_fd(p, cmds, x);
	if (nl)
		*nl = '\n';
	return (tmp);
}

int	open_heredoc_fd(char *delimiter, t_cmd *cmds, t_exec_ctx x)
{
	char	*p;
	char	*nl;
	int		tmp;

	if (!delimiter)
		return (-1);
	if (!ft_strchr(delimiter, '\n'))
		return (open_single_heredoc_fd(delimiter, cmds, x));
	p = delimiter;
	while (1)
	{
		nl = ft_strchr(p, '\n');
		tmp = hd_open_part(p, nl, cmds, x);
		if (tmp < 0)
			return (-1);
		if (!nl)
			return (tmp);
		close_fd(tmp);
		p = nl + 1;
	}
}
