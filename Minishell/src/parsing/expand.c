#include "minishell.h"

char *expand_one(char *tok, t_shell *sh)
{
    int  i = 0;
    char *res = ft_strdup("");

    while (tok[i])
    {
        if (tok[i] == '\'')  
        {
            int j = i + 1;
            while (tok[j] && tok[j] != '\'')
                j++;
            char *part = ft_substr(tok, i, j - i + 1);
            char *tmp = ft_strjoin(res, part);
            free(res);
            free(part);
            res = tmp;
            i = j + 1;
        }
        else if (tok[i] == '$')   
        {
            if (tok[i + 1] == '?')
            {
                char *val = ft_itoa(sh->last_exit_status);
                char *tmp = ft_strjoin(res, val);
                free(val);
                free(res);
                res = tmp;
                i += 2;
            }
            else
            {
                int len = 0;
                while (tok[i + 1 + len] && (ft_isalnum(tok[i + 1 + len]) || tok[i + 1 + len] == '_'))
                    len++;

                char *name = ft_substr(tok, i + 1, len);
                char *env = getenv(name);
                char *tmp = ft_strjoin(res, env ? env : "");
                free(name);
                free(res);
                res = tmp;
                i += len + 1;
            }
        }
        else
        {
            char tmp[2] = {tok[i], 0};
            char *joined = ft_strjoin(res, tmp);
            free(res);
            res = joined;
            i++;
        }
    }
    return res;
}

char **expand_tokens(char **tokens, t_shell *sh)
{
    int count;
    int i;
    int k;

    i = 0;
    count = 0;
    while (tokens[count])
        count++;

    char **out = malloc(sizeof(char *) * (count + 1));
    if (!out)
        return NULL;
    while (i < count)
    {
        out[i] = expand_one(tokens[i], sh);
        if (!out[i])
        {
            k = 0;
            while (k < i)
            {
                free(out[k]);
                k++;
            }
            free(out);
            return NULL;
        }
        i++;
    }
    out[count] = NULL;
    return out;
}
