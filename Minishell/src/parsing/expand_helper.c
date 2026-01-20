/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oaletham <oaletham@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/18 17:29:13 by oaletham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_expanded(char **out, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(out[i]);
		i++;
	}
	free(out);
}

void	update_q(char c, int *q)
{
	if (c == '\'' && *q != 2)
	{
		if (*q == 1)
			*q = 0;
		else
			*q = 1;
	}
	else if (c == '\"' && *q != 1)
	{
		if (*q == 2)
			*q = 0;
		else
			*q = 2;
	}
}
