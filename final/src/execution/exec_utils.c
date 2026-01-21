/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:47:41 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/23 13:47:41 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(int fd)
{
	if (fd >= 0)
		close(fd);
}

void	minishell_perror(char *name)
{
	write(2, "minishell: ", 11);
	perror(name);
}

void	minishell_cmd_not_found(char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
}

void	safe_dup2(int from, int to)
{
	if (from >= 0 && from != to)
	{
		if (dup2(from, to) < 0)
			exit(1);
	}
}

int	is_delim(char *line, char *delim)
{
	size_t	len;

	if (!line || !delim)
		return (0);
	len = ft_strlen(delim);
	if (ft_strlen(line) != len)
		return (0);
	if (ft_strncmp(line, delim, len) == 0)
		return (1);
	return (0);
}
