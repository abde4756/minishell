/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:28:10 by aait-laf          #+#    #+#             */
/*   Updated: 2025/06/22 16:31:59 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




int     fonc_export(char **arg,  t_env **env)
{
    int     i;
    int     j;
    char    *name;
    char    *value;

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
        {
            name = get_var_name(arg[i]);
            value = get_var_value(arg[i]);
            if(!is_valid_name(name))
            {
                export_error(name);
                free(name);
                free(value);
                return(-1);
            } 
            if(check_var_exist_env(*env, arg[i]) == -1)
                add_back_env(env, arg[i]);
            else
                updat_env(env, arg[i]);
            free(name), free(value);
        }
        else
        {
            if(!is_valid_name(arg[i]))
            {
                export_error(arg[i]);
                return(-1);
            }    
            if(check_var_exist_env(env, arg[i]) == -1)
                add_back_env(env, arg[i]);
            else
                mak_as_export(env, arg[i]);
        }
        i++;
    }
    return(0);
}

int     mak_as_export(t_env **env, char *str)
{
    
}

int     is_valid_name(char *str)
{
    int i; 

    i = 0;
    while(str[i] && str[i] != '=')
    {
        if(!ft_isalnum(str[i]) && str[i] != '-')
            return(0);
    }
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

void    updat_env(t_env *env, char *str)
{
    int     i;
    char    *tmp;
    
    while (str[i] && str[i] != '=')
    {
        tmp[i] = str[i];
        i++;
    }
    while (env)
    {
        if(str_cmp(tmp, env->str) == 0)
            env->str = str;
        env = env->next;
    }
}

void    print_env(t_env *env)
{
    while (env)
    {
        printf("%s\n", env->str);
        env = env->next;
    }
}