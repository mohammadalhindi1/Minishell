/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:58:09 by malhendi          #+#    #+#             */
/*   Updated: 2025/12/22 11:58:09 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_unexpected_newline(void)
{
	write(2,
		"minishell: syntax error near unexpected token `newline'\n",
		sizeof("minishell: syntax error near unexpected token `newline'\n")
		- 1);
	g_exit_status = 2;
}

void	print_unexpected(char c)
{
	write(2, "minishell: syntax error near unexpected token `",
		sizeof("minishell: syntax error near unexpected token `") - 1);
	write(2, &c, 1);
	write(2, "'\n", 2);
	g_exit_status = 2;
}
