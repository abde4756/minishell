#include "minishell.h"

int     fonc_cd(char **arg, t_env *env)
{
    char    *tmp = NULL;
    char   *current_getcwd;
    int     nombre_arg;

    if(!arg)
        return(-1);
    nombre_arg = get_nbr_arg(arg);
    if(nombre_arg == 1)
    {
        while (env)
        {
            if(str_cmp("HOME", env->name) == 0)
            {
                tmp = env->value;
                break;
            }
            env = env->next;
        }
        if(tmp != NULL)
        {
            if(chdir(tmp) == -1)
            {
                printf("cd: no such file or directory: %s\n", tmp);
                return(-1);
            }
            current_getcwd = getcwd(NULL, 0);
            update_env(&env, "PWD", current_getcwd);
            free(current_getcwd);
        }
        else
        {
            printf("cd: HOME not set\n");
            return(-1);
        }
    }
    else if(nombre_arg == 2)
    {
        if (chdir(arg[1]) == -1)
        {
            printf("cd: no such file or directory: %s\n", arg[1]);
            return(-1);
        }
        current_getcwd = getcwd(NULL, 0);
        update_env(&env, "PWD", current_getcwd);
        free(current_getcwd);
    }
    else
    {
        printf("cd: too many arguments\n");
        return(-1);
    }
    return(0);
}

void    update_env(t_env **env, const char *name, const char *value)
{
    t_env   *current = *env;

    while (current)
    {
        if (str_cmp(current->name, name) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    add_back_env(env, name, value, (idx_nod(*env) + 1));
}

int     get_nbr_arg(char    **arg)
{
    int i = 0;

    while (arg[i])
        i++;
    return(i);
}