#include "minishell.h"

void    handler_ctrl_c(int sig)
{
    if(sig == SIGINT)
    {
        printf("\n");
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

void    handler_ctrl_d()
{
    exit(0);
}

void    handler_ctrl_slash(int sig)
{
    (void)sig;
}

int     main()
{
    char *line;

    signal(SIGINT, handler_ctrl_c);
    signal(SEGQUIT, handler_ctrl_slash);
    while(1)
    {
        line = readline("minishell> ");
        if(line == NULL)
        {
            printf("exit\n")
            exit(0);
        }
        if(*line)
            add_history(line);
        free(line);
    }
    return(0);
}