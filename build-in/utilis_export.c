#include "minishell.h"

int     var_with_equal(char **arg, int i)
{
    char    *name;
    char    *value;

    name = get_var_name(arg[i]);
    value = get_var_value(arg[i]);
    if(!is_valid_name(name))
    {
        export_error(name);
        free(name);
        free(value);
        return(-1);
    } 
    if(check_var_exist_env(*env, name) == -1)
        add_back_env(env, name, value, (idx_nod(*env) + 1));
    else
        updat_env(env, name, value);
    free(name), free(value);
    return(0);
}

int     var_no_value(char **arg, int i)
{
    char *name;
    char *value;

    if(!is_valid_name(arg[i]))
    {
        export_error(arg[i]);
        return(-1);
    }
    name = get_var_name(arg[i]);
    if(check_var_exist_env(*env, name) == -1)
    {
        value = get_var_value(arg[i]);
        add_back_env(env, name, value, (idx_nod(*env) + 1));
        free(value);
    }
    else
        mak_as_export(env, arg[i]);
    free(name);
}