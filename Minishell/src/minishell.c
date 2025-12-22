/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 13:41:21 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/21 13:41:21 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_split(char **s)
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

static void	free_cmds(t_cmd *cmds, int n)
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

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_cmd	*cmds;
	int		count;

	(void)ac;
	(void)av;
	while (1)
	{
		line = readline("minishell% ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		cmds = NULL;
		count = 0;
		if (parse_line(line, &cmds, &count) == 0)
		{
			execute_pipeline(cmds, count, envp);
			free_cmds(cmds, count);
		}
		free(line);
	}
	return (0);
}