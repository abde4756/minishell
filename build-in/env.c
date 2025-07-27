#include "minishell.h"

int     env_fonc(char **arg, t_env *env)
{
    char *name_value;
    char *temp;

    if(!env || !arg)
        return(-1);
    if(arg[1])
        return(-1);
    while (env != NULL)
    {   
        temp = str_join(env->name, "=");
        if(!temp)
            return (-1);
        if(env->value)
        {
            name_value = str_join(temp, env->value);
            free(temp);
            if(!name_value)
                return (-1);
        }
        else
            name_value = temp;
        printf("%s\n", name_value);
        free(name_value);
        env = env->next;
    }
    return(0);
}
