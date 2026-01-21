/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:39:30 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/22 00:00:00 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_input_fd(t_cmd *cmd)
{
	int	fd;

	if (!cmd || !cmd->infile)
		return (-1);
	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
		minishell_perror(cmd->infile);
	return (fd);
}

static int	open_out_one(t_cmd *cmd, char *path)
{
	int	flags;
	int	fd;

	flags = O_CREAT | O_WRONLY;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
		minishell_perror(path);
	return (fd);
}

static int	open_last_out_fd(t_cmd *cmd, char **outs)
{
	int	i;
	int	fd;
	int	tmp;

	i = 0;
	fd = -1;
	while (outs[i])
	{
		tmp = open_out_one(cmd, outs[i]);
		if (tmp < 0)
		{
			if (fd != -1)
				close_fd(fd);
			return (-1);
		}
		if (fd != -1)
			close_fd(fd);
		fd = tmp;
		i++;
	}
	return (fd);
}

int	get_cmd_output_fd(t_cmd *cmd)
{
	char	**outs;
	int		fd;

	if (!cmd || !cmd->outfile)
		return (-1);
	if (!ft_strchr(cmd->outfile, '\n'))
		return (open_out_one(cmd, cmd->outfile));
	outs = ft_split(cmd->outfile, '\n');
	if (!outs)
		return (-1);
	fd = open_last_out_fd(cmd, outs);
	free_split(outs);
	return (fd);
}
