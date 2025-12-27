#include "minishell.h"

int g_signal = 0;

void    handle(int sig)
{
    g_signal = sig;
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

int main()
{
    char *line;
    struct sigaction sa;
    char **tokens;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    while (1)
    {
        line = readline("minishell% ");
        if (!line)
        {
            write(1, "exit\n", 5);
            break;
        }
        tokens = tokens_func(line);
        if (!tokens)
        {
            free(line);
            printf("unvalid operator (;)\n");
            return  (1);
        }
        int i = 0;
        while (tokens[i])
        {
            printf("%s\n", tokens[i]);
            free(tokens[i]);
            i++;
        }
        free(tokens);
        //tokens = expand_token(tokens);
        //excution(tokens); // here where hindi will do the work
        add_history(line);
        free(line);
    }

    return 0;
}

