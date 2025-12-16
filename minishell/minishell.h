# ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include <string.h> //its for memset , remove it and make memset ft_memset
#include <stdlib.h>
int check_valid(char *line);
char **tokens_func(char *line);
typedef struct s_range {
    int start;
    int end;
}   t_range;

#endif