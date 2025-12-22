/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:39:41 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:39:41 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delim(char *line, char *delim)
{
	size_t	len;

	if (!line || !delim)
		return (0);
	len = ft_strlen(delim);
	if (ft_strlen(line) != len)
		return (0);
	return (ft_strncmp(line, delim, len) == 0);
}

int	open_heredoc_fd(char *delimiter)
{
	int		fd[2];
	char	*line;

	if (!delimiter)
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (is_delim(line, delimiter))
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close_fd(fd[1]);
	return (fd[0]);
}