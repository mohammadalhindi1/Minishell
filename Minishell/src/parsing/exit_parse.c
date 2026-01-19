/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_prase.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:52:01 by malhendi          #+#    #+#             */
/*   Updated: 2026/01/08 19:52:01 by malhendi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_is_sp(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static void	ms_skip_sp(char *s, int *i)
{
	while (s[*i] && ms_is_sp(s[*i]))
		(*i)++;
}

static void	ms_read_sign(char *s, int *i, int *sign)
{
	*sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

static int	ms_parse_digits(char *s, int *i, int sign, long long *out)
{
	unsigned long long	n;
	unsigned long long	lim;

	n = 0;
	lim = (unsigned long long)LLONG_MAX;
	if (sign < 0)
		lim++;
	while (s[*i] >= '0' && s[*i] <= '9')
	{
		if (n > lim / 10ULL)
			return (0);
		if (n == lim / 10ULL && (unsigned long long)(s[*i] - '0') > lim % 10ULL)
			return (0);
		n = n * 10ULL + (unsigned long long)(s[*i] - '0');
		(*i)++;
	}
	ms_skip_sp(s, i);
	if (s[*i] != '\0')
		return (0);
	if (sign < 0 && n == (unsigned long long)LLONG_MAX + 1ULL)
		return (*out = LLONG_MIN, 1);
	*out = (long long)n;
	if (sign < 0)
		*out = -(*out);
	return (1);
}

int	parse_ll_strict(char *s, long long *out)
{
	int	i;
	int	sign;

	i = 0;
	ms_skip_sp(s, &i);
	ms_read_sign(s, &i, &sign);
	if (!(s[i] >= '0' && s[i] <= '9'))
		return (0);
	return (ms_parse_digits(s, &i, sign, out));
}
