#include "minishell.h"

int     unset_fonc(char **arg, t_env **env)
{
    int     i;
    int     idex;

	if(arg[1] == NULL)
		return(0);
    i = 1;
    while (arg[i])
    {
        idex = find_idex_var_env(arg[i], *env);
        if(idex != -1)
        {
            delete_var_env(idex, env);
            indx_nods(env);
        }
        i++;
    }
    return(0);
}

int     find_idex_var_env(char *str, t_env *env)
{
    char *name;

    if(!str || !env)
        return(-1);
    name = get_var_name(str);
    if(!name)
        return(-1); 
    while(env)
    {
        if(str_cmp_var(env->name, name))
            return(free(name), env->idx_nod);
        env = env->next;
    }
    free(name);
    return(-1);
}

int     str_cmp_var(char *s1, char *s2)
{
    int     i; 

    i = 0;
    if(!s1 || !s2)
        return(-1);
    while(s1[i] && s1[i] != '=' && s2[i])
    {
        if(s1[i] != s2[i])
            return(-1);
        i++;
    }
    if(s1[i] == '=' && s2[i] == '\0')
        return(1);
    return(-1);
}

void     indx_nods(t_env **env)
{
    t_env *temp;
    int     i = 0;

    if(!env || !*env)
        return;
    temp = *env;
    while (temp != NULL)
    {
        temp->idx_nod = i++;
        temp = temp->next;
    }
}

void	delete_var_env(int idx, t_env **env)
{
    t_env *temp;
    t_env *env_temp;

	indx_nods(env);
    if(idx == 0)
    {
        temp = *env;
        *env = (*env)->next;
        free(temp->name);
        free(temp->value);
        free(temp);
        indx_nods(env);
        return ;
    }
    env_temp = *env;
	while (env_temp != NULL)
	{
		if(idx - env_temp->idx_nod == 1)
		{
            temp = env_temp->next;
			if(env_temp->next->next == NULL)
				env_temp->next = NULL;
			else 
				env_temp->next = env_temp->next->next;
            free(temp->name);
            free(temp->value);
            free(temp);
			break;
		}
		env_temp = env_temp->next;
	}
    indx_nods(env);
}
