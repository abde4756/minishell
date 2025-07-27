#include "minishell.h"

int		fonc_pwd(char **arg, t_env *env)
{
    if(!arg || !env)
        return(1);
	if(arg[1])
	{
		printf("pwd: too many arguments");
		return(1);
	}
	print_repert(env);
	return(0);
}

void	print_repert(t_env *env)
{
    if(!env)
    {
        printf("No environment variables set.\n");
        return;
    }
	while (env)
	{
		if(str_cmp("PWD", env->name) == 0)
		{
            printf("%s\n", env->value);
			return;
		}
		env = env->next;
	}
    printf("PWD not found in environment variables.\n");
}