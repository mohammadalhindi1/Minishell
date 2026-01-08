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

static size_t	envp_count(char **envp)
{
	size_t	count;

	if (!envp)
		return (0);
	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static int	is_shlvl(const char *entry)
{
	if (!entry)
		return (0);
	return (ft_strncmp(entry, "SHLVL=", 6) == 0);
}

static char	*build_shlvl_entry(char *value)
{
	char	*entry;
	char	*joined;

	joined = ft_strjoin("SHLVL=", value);
	if (!joined)
		return (NULL);
	entry = joined;
	return (entry);
}

static char	*next_shlvl_value(char *current)
{
	char	*end;
	long	lvl;

	lvl = 0;
	if (current && *current)
	{
		lvl = strtol(current, &end, 10);
		if (*end != '\0' || lvl < 0)
			lvl = 0;
	}
	return (ft_itoa((int)(lvl + 1)));
}

static char	**update_shlvl(char **envp)
{
	size_t	count;
	size_t	i;
	char	*value;
	char	*next_value;
	char	*entry;
	char	**copy;
	int		found;

	count = envp_count(envp);
	copy = malloc(sizeof(char *) * (count + 2));
	if (!copy)
		return (envp);
	found = 0;
	i = 0;
	while (i < count)
	{
		if (!found && is_shlvl(envp[i]))
		{
			value = envp[i] + 6;
			next_value = next_shlvl_value(value);
			entry = next_value ? build_shlvl_entry(next_value) : NULL;
			free(next_value);
			if (entry)
			{
				copy[i] = entry;
				found = 1;
			}
			else
				copy[i] = ft_strdup(envp[i]);
		}
		else
			copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			free_split(copy);
			return (envp);
		}
		i++;
	}
	if (!found)
	{
		next_value = next_shlvl_value(NULL);
		entry = next_value ? build_shlvl_entry(next_value) : NULL;
		free(next_value);
		if (entry)
			copy[i++] = entry;
	}
	copy[i] = NULL;
	return (copy);
}

static void	handle(int sig)
{
    g_exit_status = sig;
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_cmd	*cmds;
	int		count;
	char	**shell_envp;
	t_shell sh;
	struct sigaction sa;

	shell_envp = update_shlvl(envp);

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle;

    sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
    sh.last_exit_status = 0;
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
		sh.last_exit_status = g_exit_status;
		if (parse_line(line, &cmds, &count, &sh) == 0)
		{
			execute_pipeline(cmds, count, shell_envp);
			free_cmds(cmds, count);
		}
		free(line);
	}
	if (shell_envp && shell_envp != envp)
		free_split(shell_envp);
	return (0);
}