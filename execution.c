

#include "minishell.h"


int     execute_command(t_cmd *cmd, t_env **env, int status)
{
    t_cmd   *current;

    if(!cmd || !env)
        return(-1);
    current = cmd;
    while(current)
    {
        // vérifier si c'est une commande (CMD)
        if(current->arg && current->arg[0])
        {
            // Gérer les pipes (à implémenter plus tard)
            if(current->next)
                status = execute_with_pipe(current, env, status);
            // Gérer les redirections
            else if(current->rdr && (current->rdr->type == RDRIN || 
                current->rdr->type == RDROUT || 
                current->rdr->type == APPND || 
                current->rdr->type == HEREDOC))
                {
                    status = execute_with_redirection(current, env, status);
                }
            else
            {
                if(is_builin_command(current->arg[0]))
                    status = execute_builtin(current, env, status);
                else
                    status = execute_simple_command(current, env, status);
            }
        }
        current = current->next;
    }
    return(status);
}

int     execute_with_redirection(t_cmd *current, t_env **env, int status)
{
    int result;
    t_rdr *red;
    
    red = current->rdr;
    while(red)
    {
        if(open_check_file(red) ==  -1)
        {
            return(-1);
        }
        red = red->next;
    }
    result = execute_simple_command(current, env, status);
    return(result);
}

int     open_check_file(t_rdr *red)
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
    else if(red->type == HEREDOC)
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

int     execute_simple_command(t_cmd *cmd, t_env **env, int status)
{
    pid_t   pid;
    char    *path = NULL;

    if(!env || !cmd)
        return(-1);
    if(is_builin_command(cmd->arg[0]))
        return (execute_builtin(cmd, env, status));
    else
    {
        pid = fork();
        if(pid == -1)
            return(perror("fork"), -1);
        if(pid == 0) // partie d'enfant 
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            path = get_path_cmd(cmd->arg[0]);
            if(!path)
            {
                fprintf(stderr, "Command not found: %s\n", cmd->arg[0]);
                exit(127);
            }
            if(execve(path, cmd->arg, env) == -1)
                perror("execve"), free(path), exit(127);
        }
        else if(pid > 0) //parent
            waitpid(pid, &status, 0);
    }
    if(path)
        free(path);
    return(status);
}

int     execute_builtin(t_cmd *cmd, t_env **env , int last_code)
{
    int     status; //

    status = last_code; // initialiser le status avec le dernier code de sortie
    if(!cmd || !cmd->arg || !env)
        return(-1);
    if(ft_strcmp(cmd->arg[0], "echo") == 0)
        status = echo_fonc(cmd->arg);
    else if(ft_strcmp(cmd->arg[0], "cd") == 0)
        status = fonc_cd(cmd->arg, *env);
    else if(ft_strcmp(cmd->arg[0], "pwd") == 0)
        status = fonc_pwd(cmd->arg);
    else if(ft_strcmp(cmd->arg[0], "export") == 0)
        status = fonc_export(cmd->arg, env);
    else if(ft_strcmp(cmd->arg[0], "unset") == 0)
        status = unset_fonc(cmd->arg, env);
    else if(ft_strcmp(cmd->arg[0], "env") == 0)
        status = env_fonc(cmd->arg, *env);
    else if(ft_strcmp(cmd->arg[0], "exit") == 0)
        status = exit_fonc(cmd->arg, last_code);
    return(status);
}

int     is_builin_command(char *cmd)
{
    if(ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 || 
       ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 || 
       ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 || 
       ft_strcmp(cmd, "exit") == 0)
        return(1);
    return(0);
}