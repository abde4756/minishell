#include "minishil.h"


int     execute_command(t_cmd *cmd, t_env **env)
{
    int status;
    // bouque general 
    t_cmd   *current = cmd;

    while(current)
    {
        // virifier si c'est une commande (CMD)
        if(current->arg && current->arg[0])
        {
            if(current->next != NULL)
            {
                // virifier si le suivanr et pipe 
                if(current->next && current->next->rdr && current->next->rdr->type == PIPE)
                    status = execute_whith_pipe();
            }
            // redirection 
            else if(current->rdr && (current->rdr->type == RDRIN || 
                         current->rdr->type == RDROUT || 
                         current->rdr->type == APPND || 
                         current->rdr->type == HEREDOC))
                status = execution_with_redirection(current);
            // ya juste des chose simple (text)
            else
            {
                if(is_builin_command(current->arg[0]))
                    status = execute_builtin(current, env);
                else
                    status = exucute_simple_command(current);
            }
        }
        current = current->next;
    }
}

int     execute_with_redirection(t_cmd *current)
{
    int status;
    t_red *red;
    
    red = current->rdr;
    while(red)
    {
        if(open_check_file(red) ==  -1)
        {
            printf("");
            return(-1);
        }
        red = red->next;
    }
    status = execute_simple_command(current);
    return(status);
}

int     open_check_fil(t_red *red)
{
    int fd;

    if(red->type == RDRIN)
    {
        fd = open(red->file, O_RDONLY);
        if(fd == -1)
        {
            perror("open");
            return(-1);
        }
        dup2(fd, 0);
        close(fd);
    }
    else if(red->type == RDROUT)
    {
        fd = open(red->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd == -1)
        {
            perror("open");
            return(-1);
        }
        dup2(fd, 1);
        close(fd);
    }
    else if(red->type == APPND)
    {
        fd = open(red->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd == -1)
        {
            perror("open");
            return(-1);
        }
        dup2(fd, 1);
        close(fd);
    }
    else if(HEREDOC)
    {
        if(red->fd == -1)
            return(-1);
        dup2(red->fd, 0);
        close(red->fd);
    }
    else
        return(-1);
    return(0);
}

int     execute_simple_command(t_cmd *cmd, t_env *env)
{
    pid_t   pid;
    int     status;
    char    *path = NULL;

    if(!env || !cmd)
        return(-1);
    if(is_builin_command(cmd->arg[0]))
        execute_builtin(cmd, &env);
    else
    {
        pid = fork();
        if(pid == -1)
            perror("fork"), return(-1);
        if(pid == 0) // partie d'enfant 
        {
            signal(SIGINT, SIG_DFL);
            signal(SEGQUIT, SIG_DFL);
            if(execvp(cmd->arg[0], cmd->arg) == -1)
                perror("execvp"), exit(127);
        }
        else if(pid > 0) //parent
            wait(&status);
    }
    return(1);
}

int     execute_builtin(t_cmd *cmd, t_env **env )
{
    int     status; // 

    if(cmd->arg[0] == "echo")
        status = echo_fonc(cmd->arg);
    else if(cmd->arg[0] == "cd")
        status = fonc_cd(cmd->arg, *env);
    else if(cmd->arg[0] == "pwd")
        status = fonct_pwd(cmd->arg);
    else if(cmd->arg[0] == "export")
        status = fonc_export(cmd->arg, env);
    else if(cmd->arg[0] == "unset")
        status = unset_fonc(cmd->arg, env);
    else if(cmd->arg[0] == "env")
        status = env_fonc(cmd->arg, *env);
    else if(cmd->arg[0] == "exit")
        status = exit_fonc(cmd->arg, ); // problem de last_code
    return(status);
}

int     is_builin_command(char *cmd)
{
    if(cmd == "echo" || cmd == "cd" || cmd == "pwd"
        cmd == "export" || cmd == "unset"
        cmd == "env" || cmd == "exit")
        return(1);
    return(0);
}