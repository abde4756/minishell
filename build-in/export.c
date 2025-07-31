#include "minishell.h"

int     fonc_export(char **arg,  t_env **env)
{
    int     i;
    int     j;
    int     status;

    if(!arg)
        return(-1);
    if(!arg[1])
    {
        print_env(*env);
        return(0);
    }
    i = 1;
    while (arg[i])
    {   
        if(has_equal_sign(arg[i])) // si on '=' (var=value)
            status = var_with_equal(arg, i);
        else
            var_no_value(arg, i);
        i++;
    }
    return(status);
}

char    *get_var_name(char *str)
{
    char    *name;
    int     i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    name = ft_substr(str, 0, i);
    return (name);
}
 
int     check_var_exist_env(t_env *env, char *name)
{
    while (env)
    {
        if (str_cmp(env->name, name) == 0)
            return (1);
        env = env->next;
    }
    return (-1);
}

char    *get_var_value(char *str)
{
    char    *value;
    int     i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    if (str[i] == '=')
        value = ft_strdup(str + i + 1);
    else
        value = ft_strdup("");
    return (value);
}

int     mak_as_export(t_env **env, char *str)
{
    char    *name;
    char    *value;

    name = get_var_name(str);
    if (!name)
        return (-1);
    value = get_var_value(str);
    if (!value)
    {
        free(name);
        return (-1);
    }
    if (check_var_exist_env(*env, name) == -1)
        add_back_env(env, name, value, (idx_nod(*env) + 1));
    else
        updat_env(env, name, value);
    free(name);
    free(value);
    return (0);
}

void    export_error(char *str)
{
    if (!str)
        return;
    fprintf(stderr, "export: `%s': not a valid identifier\n", str);
}

int     is_valid_name(char *str)
{
    int i;

    if(!str || !str[0])
        return (0);
    if (!ft_isalpha(str[0]) && str[0] != '_') // premier caractere doit etre une lettre
        return (0);
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int     has_equal_sign(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == '=')
            return(1);
        i++;
    }
    return(0);
}

void    updat_env(t_env *env, char *name, char *value)
{
    int i;

    if (!env || !name || !value)
        return;
    if(value[0] != '"') // si pas de guillemets
        add_double_quotes(value, name, env);
    else
    {
        while (env)
        {
            if(str_cmp(name, env->name) == 0)
                free(env->value), env->value = value;
            env = env->next;
        }
    }
}

void    add_double_quotes(char *value, char *name, t_env *env)
{
    char    *new_str;
    int     i, j;
    
    if (!value || !name || !env)
        return;
    new_str = malloc(ft_strlen(value) + 3);
    if (!new_str)
        return;
    new_str[0] = '"';
    (1) &&(i = 1, j = 0);
    while (value[j])
        new_str[i++] = value[j++];
    new_str[i++] = '"';
    new_str[i] = '\0';
    while (env)
    {
        if (str_cmp(name, env->name) == 0)
        {
            free(env->value);
            env->value = new_str;
            return;
        }
        env = env->next;
    }
    free(new_str);
}

void    print_env(t_env *env)
{
    while (env)
    {
        printf("%s\n", env->name);
        env = env->next;
    }
}