/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:39:30 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:39:30 by malhendi         ###   ########.fr       */
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

int	get_cmd_output_fd(t_cmd *cmd)
{
	int	flags;
	int	fd;

	if (!cmd || !cmd->outfile)
		return (-1);
	flags = O_CREAT | O_WRONLY;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->outfile, flags, 0644);
	if (fd < 0)
		minishell_perror(cmd->outfile);
	return (fd);
}
