#include "minishel.h"

int     exit_fonc(char **arg, int last_code)
{
    int exit_code;
    int i = 0;

    exit_code = last_code;
    printf("exit\n"); // affiche d'abord exit sur shell
    if(arg[1] && arg[2])
    {
        printf("exit: too many arguments\n");
        return(1);
    }
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
        exit_code = ft_atoi(arg[1]) % 256; // 256 a cause de shell car il utilise juste 8bit (0 jusqu'a 255)
    }
    exit(exit_code);
}