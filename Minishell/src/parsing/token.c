#include "minishell.h"
#include "parsing.h"

void add_word_to_token(char ***tokens, int *words, char *line, t_range r)
{
    char **old = *tokens;
    char **new_arr;
    int len = r.end - r.start;

    new_arr = malloc(sizeof(char *) * (*words + 1));

    for (int i = 0; i < *words; i++)
        new_arr[i] = old[i];

    new_arr[*words] = malloc(len + 1);

    for (int i = 0; i < len; i++)
        new_arr[*words][i] = line[r.start + i];

    new_arr[*words][len] = '\0';

    free(old);
    *tokens = new_arr;
    (*words)++;
}

char **tokens_func(char *line)
{
    int i;
    int j;
    int start;
    int words;
    char **tokens;
    t_range r;

    i = 0;
    start = 0;
    words = 0;
    tokens = NULL;

    if (check_valid(line) == 1)
        return NULL;
    while (line[i])
    {
        while (line[i] == ' ')
        {
            i++;
            start = i;
        }

        while (line[i] != '|' && line[i] != '<' &&
               line[i] != '>' && line[i] != ' ' &&
               line[i] != '"' && line[i] != 39 &&
               line[i] != '\0')
        {
            i++;
        }

        if (i > start)
        {
            r.start = start;
            r.end = i;
            add_word_to_token(&tokens, &words, line, r);
        }
        if (line[i] == 39) // '
        {
            j = 1;
            while (line[i + j] != 39)
                j++;
            r.start = i;
            r.end = i + j + 1;
            add_word_to_token(&tokens, &words, line, r);
            i += j + 1;
            start = i;
        }
        else if (line[i] == '"') // "
        {
            j = 1;
            while (line[i + j] != '"')
                j++;
            r.start = i;
            r.end = i + j + 1;
            add_word_to_token(&tokens, &words, line, r);
            i += j + 1;
            start = i;
        }
        else if (line[i] == ' ')
        {
            i++;
            start = i;
        }
        else if (line[i] == '|')
        {
            r.start = i;
            r.end = i + 1;
            add_word_to_token(&tokens, &words, line, r);
            i++;
            start = i;
        }
        else if (line[i] == '<')
        {
            if (line[i + 1] == '<')
            {
                r.start = i;
                r.end = i + 2;
                add_word_to_token(&tokens, &words, line, r);
                i += 2;
            }
            else
            {
                r.start = i;
                r.end = i + 1;
                add_word_to_token(&tokens, &words, line, r);
                i++;
            }
            start = i;
        }
        else if (line[i] == '>')
        {
            if (line[i + 1] == '>')
            {
                r.start = i;
                r.end = i + 2;
                add_word_to_token(&tokens, &words, line, r);
                i += 2;
            }
            else
            {
                r.start = i;
                r.end = i + 1;
                add_word_to_token(&tokens, &words, line, r);
                i++;
            }
            start = i;
        }
    }
    char **final = malloc(sizeof(char *) * (words + 1));
    for (int k = 0; k < words; k++)
        final[k] = tokens[k];
    final[words] = NULL;
    free(tokens);
    return final;
}
