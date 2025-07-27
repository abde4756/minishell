#include "minishel.h"

int     fonc_cd(char *str, t_env *env)
{
    char    **arg;
    char    *tmp;
    int     nombre_arg , i;

    arg = ft_split(str, ' ');
    if(!arg)
        return(-1);
    if(str_cmp("cd", arg[0]) == -1)
    {
        printf("command not found: %s\n", arg[0]);
        return(-1);
    }
    nombre_arg = get_nbr_arg(arg);
    if(nombre_arg == 0)
    {
        while (env)
        {
            if(str_cmp("HOME=", env->str) == 0)
            {
                while (env->str[i + 5])
                {
                    tmp[i] = env->str[i + 5];
                    i++;
                }
                tmp[i] = '\0';
            }
            env = env->next;
        }
        if(tmp != NULL)
        {
            if(chdir(tmp) == -1)
                printf("cd: no such file or directory: %s\n", tmp);
        }
        else
            printf("cd: HOME not set");
    }
    else if(nombre_arg == 1)
    {
        if (chdir(arg[1]) == -1)
            printf("cd: no such file or directory: %s\n", arg[1]);
    }
    else
        printf("cd: too many arguments");
    return(0);
}