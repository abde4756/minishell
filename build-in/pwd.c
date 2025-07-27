#include "minishel.h"

int     fonct_pwd(char **arg)
{
    char cwd[PATH_MAX];

    if(arg[1])
    {
        printf("pwd: too many arguments\n");
        return(1);
    }
    if(getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
        return(0);
    }
    perror("pwd");
    return(1);
}


/// 2éme m2thode par ENV 

int		fonc_pwd(char **arg, t_env *env)
{
    if(!arg || !env)
        return(1);
	if(arg[1])
	{
		printf("pwd: too many arguments");
		return(1);
	}
	print_str_list(env);
	return(0);
}

void	print_str_list(t_env *env)
{
    char cmd[1024];
	int i = 0;
	
	while (env)
	{
		if(str_cmp_n("PWD=", env->str , str_len("PWD=")) == 0)
		{
            printf("%s\n", env->str + 4);
			return;
		}
		env = env->next;
	}
    if(getcwd(cwd, sizeof(cwd))) // si il exit pas pwd
        printf("%s\n", cwd);
}

int     str_cmp_n(char *cmp1, char *cmp2, int size)
{
    int i = 0;

    if(!cmp1 || !cmp2)
        return(1)
    while(i < size)
    {
        if(cmp1[i] != cmp2[i])
            return(1);
        i++;
    }
    if(cmp1[i] == '\0' && cmp2[i] != '=')
        return(0);
    return(1);
}