#include "minishel.h"

int     env_fonc(char **arg, t_env *env)
{
    if(!env || !arg)
        return(-1);
    if(arg[1])
        return(-1);
    while (env != NULL)
    {
        printf("%s\n", env->str);
        env = env->next;
    }
    return(0);
}