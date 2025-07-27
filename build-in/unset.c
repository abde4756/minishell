#include "minishel.h"

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
            indx_nods(*env);
        }
        i++;
    }
    return(0);
}

int     find_idex_var_env(char *str, t_env *env)
{
    if(!str || !env)
        return(-1);
    while(env)
    {
        if(str_cmp_var(env->str, str)) // il faut comparer justement avand '='
            return(env->idx_nod);
        env = env->next;
    }
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

void     indx_nods(t_env *env)
{
    int     i = 0;

    while (env != NULL)
    {
        env->idx_nod = i++;
        env = env->next;
    }
}

void	delete_var_env(int idx, t_env **env)
{
    t_env *temp;

	indx_nods(*env);
    if(idx == 0)
    {
        temp = *env;
        *env = *env->next;
        free(temp->str);
        free(temp);
        indx_nods(*env);
        return ;
    }
	while (*env != NULL)
	{
		if(idx - (*env)->idx_nod == 1)
		{
            temp = (*env)->next;
			if(*env->next->next == NULL)
				*env->next = NULL;
			else 
				*env->next = *env->next->next;
            free(temp->str);
            free(temp);
			break;
		}
		*env = *env->next;
	}
    indx_nods(*env);
}
