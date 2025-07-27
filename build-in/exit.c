
#include "minishell.h"

int     exit_fonc(char **arg, int last_code)
{
    int exit_code;
    int i;

    exit_code = last_code;
    printf("exit\n");
    if(arg[1] && arg[2])
    {
        printf("exit: too many arguments\n");
        return(1);
    }
    i = 0;
    if(arg[1])
    {
        if(arg[1][i] == '-' || arg[1][i] == '+')
            i++;
        if(!arg[1][i])
        {
            printf("exit: %s: numeric argument required\n", arg[1]);
            exit(1);
        }
        while(arg[1][i])
        {
            if(!ft_isdigit(arg[1][i]))
            {
                printf("exit: %s: numeric argument required\n", arg[1]);
                exit(1);
            }
            i++;
        }
        exit_code = ft_atoi(arg[1]);
        // Limiter le code de sortie entre 0 et 255
        if (exit_code < 0)
            exit_code = 256 + (exit_code % 256);
        else
            exit_code = exit_code % 256;
    }
    exit(exit_code);
}