#ifndef PARSING_H
# define PARSING_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

int		check_valid(char *line);
char	**tokens_func(char *line);

typedef struct s_range
{
	int	start;
	int	end;
}	t_range;

#endif
