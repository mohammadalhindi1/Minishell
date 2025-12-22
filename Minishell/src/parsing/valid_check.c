#include "minishell.h"
#include "parsing.h"

int check_special_chars(char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (line[i] == ';' || line[i] == 92)
            return (1);
        i++;
    }
    return (0);
}

int scan_single_quotes(char *line, int *i)
{
    int j;
    int wrong;

    j = 0;
    wrong = 1;
    while (line[j + *i + 1])
    {
        if (line[j + *i + 1] == 39)
        {
            wrong = 0;
            *i = *i + j + 1;
            break;
        }
        j++;
    }
    return (wrong);
}

int scan_double_quotes(char *line, int *i)
{
    int j;
    int wrong;

    j = 0;
    wrong = 1;
    while (line[j + *i + 1])
    {
        if (line[j + *i + 1] == '"')
        {
            wrong = 0;
            *i = *i + j + 1; 
            break;
        }
        j++;
    }
    return (wrong);
}

int check_quotes(char *line)
{
    int i;
    int wrong;

    wrong = 0;
    i = 0;
    while (line[i])
    {
        if (line[i] == 39)
            wrong = scan_single_quotes(line, &i);
        else if (line[i] == '"')
            wrong = scan_double_quotes(line, &i);
        i++;
    }
    return (wrong);
}

int check_valid(char *line)
{
    int wrong;

    wrong = check_special_chars(line);
    if (wrong == 1)
        return (1);
    return (check_quotes(line));
}



// int check_valid(char *line)
// {
//     int i;
//     int j;
//     int wrong;

//     wrong = 0;
//     i = 0;
//     j = 0;
//     while (line[i])
//     {
//         if (line[i] == ';' || line[i] == 92)
//             return  (1);
//         i++;
//     }
//     i = 0;
//     while (line[i])
//     {
//         if (line[i] == 39)
//         {
//             j = 0;
//             wrong = 1;
//             while (line[j + i + 1])
//             {
//                 if (line[j + i + 1] == 39)
//                 {
//                     wrong = 0;
//                     i = i + j + 1;
//                     break;
//                 }
//                 j++;
//             }
//         }
//         else if (line[i] == '"')
//         {
//             j = 0;
//             wrong = 1;
//             while (line[j + i + 1])
//             {
//                 if (line[j + i + 1] == '"')
//                 {
//                     wrong = 0;
//                     i = i + j + 1; 
//                     break;
//                 }
//                 j++;
//             }
//         }
//         i++;
//     }
//     return (wrong);
// }


