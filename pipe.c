/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:19:45 by aait-laf          #+#    #+#             */
/*   Updated: 2025/06/26 16:21:44 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     count_nuber_cmd(t_cmd *cmd)
{
    int     count;

    if(!cmd)
        return(0);
    if(!cmd->next)
        return(1);
    count = 0;
    while(cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return(count);
}


int     execute_whith_pipe(t_cmd *cmd, t_env **env, int status)
{
    int     nbr_pipe;
    int     i = 0;
    char    *path;
    int     (*pipe)[2];
    pid_t   id;

    if(!cmd)
        return(-1);
    nbr_pipe = count_nuber_cmd(cmd) - 1;

    pipe = malloc(sizeof(int[2]) * nbr_pipe);
    if(!pipe)
        return(-1);
    // premierment je vais creer les pipes pour avoir les fil_descrepter
    while(i < nbr_pipe)
    {
        if(pipe(pipe[i]) == -1)
            return (perror("pipe"), free(pipe), 1);
        i++;
    }
    // on vas creer  chaque proces avec fork()
    i = 0;
    while(i <= nbr_pipe && cmd)
    {
        id = fork();
        if(id == -1)
            return (perror("fork"), free(pipe), -1);
        if(id == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            if(i == 0)
            {
                dup2(pipe[i][1], STDOUT_FILENO);
                close(pipe[i][0]);
                close_other_fil(pipe, nbr_pipe, pipe[i][1], -1);
            }
            else if(i > 0 && i < nbr_pipe)
            {
                dup2(pipe[i - 1][0], STDIN_FILENO);
                dup2(pipe[i][1], STDOUT_FILENO);
                close(pipe[i - 1][1]);
                close(pipe[i][0]);
                close_other_fil(pipe, nbr_pipe, pipe[i-1][0], pipe[i][1]);
            }
            else
            {
                dup2(pipe[i -1][0], 0);
                close(pipe[i - 1][1]);
                close_other_fil(pipe, nbr_pipe, pipe[i - 1][0], -1);
            }
            // gerrer les redirection
            if(cmd->rdr)
            {
                t_rdr *red = cmd->rdr;
                while(red)
                {
                    if(open_check_file(red) == -1)
                        return (free(pipe), -1);
                    red = red->next;
                }
            }
            // on vas executer la commande
            if(is_builin_command(cmd->arg[0]))
            {
                free(pipe);
                return execute_builtin(cmd, env, status);
            }
            else
            {
                path = get_path_cmd(cmd->arg[0]);
                if(!path)
                {
                    fprintf(stderr, "Command not found: %s\n", cmd->arg[0]);
                    free(pipe);
                    exit(1);
                }
                if(execve(path, cmd->arg, env) == -1)
                        perror("execve"), free(path), free(pipe), exit(1);
            }
        }
        else if(id > 0) 
        {
            if(i < nbr_pipe)
            {
                close(pipe[i][0]);
                close(pipe[i][1]);
            }
            waitpid(id, &status, 0);
            if(cmd->next == NULL)
            {
                if(WIFEXITED(status))
                    status = WEXITSTATUS(status);
                else
                    status = 1; // erreur si le processus n'est pas terminé normalement
            }
            cmd = cmd->next;
            i++;
        }
    }
    if(path)
        free(path);
    free(pipe);
    return(status);
}

void    close_other_fil(int pipe[][2], int nbr_pipe, int fd1, int fd2)
{
    int     i = 0;
    while(i < nbr_pipe)
    {
        if(pipe[i][0] != fd1 && pipe[i][0] != fd2)
            close(pipe[i][0]);
        if(pipe[i][1] != fd1 && pipe[i][1] != fd2)
            close(pipe[i][1]);
        i++;
    }
}

// pour pipe mais la methode de cree chaque pipe 



char    *get_path(t_env **env)
{
    t_env   *tmp;
    
    tmp = *env;
    while(tmp)
    {
        if(ft_strncmp(tmp->name, "PATH", 4) == 0)
        return (tmp->value);
        tmp = tmp->next;
    }
    return (NULL);
}

char   *get_path_cmd(char *cmd)
{
    int i;
    char *path;
    char *directory = NULL;
    char **arg;
    
    path = get_path(env);
    if(!path)
        return (printf("PATH not found\n"), NULL);
    arg = ft_split(path, ':');
    if(!arg)
        return (NULL);
    i = 0;
    while (arg[i])
    {
        if(access(arg[i], X_OK) == 0)
        {
            directory = arg[i];
            return (f_free(arg) ,directory);
        }
        i++;
    }
    f_free(arg);
    return (NULL);
}


// Pourquoi le status est géré dans le parent ?
// Le processus enfant exécute la commande (avec execve).
// Quand il termine, il quitte avec un code de sortie (exit(n)).

// Le processus parent (celui qui a fait le fork) utilise waitpid pour attendre la fin de l’enfant et récupérer son code de sortie dans la variable status.

// Pourquoi ?

// Le parent doit savoir si la commande s’est bien exécutée ou non (pour afficher le bon code de retour, gérer les erreurs, etc).
// Le shell (le parent) doit retourner le code de la dernière commande exécutée à l’utilisateur.
// En résumé :
// L’enfant : exécute la commande et termine avec un code de sortie.
// Le parent : récupère ce code avec waitpid et le met dans status.
// C’est le comportement standard d’un shell Unix :
// Le shell (parent) affiche ou utilise le code de sortie des commandes (enfants).














// int    execute_whith_pipe(t_cmd *cmd, t_env **env, int status)
// {
//     int pipe_fd[2];
//     pid_t pid;
//     char *path;

//     if(pipe(pipe_fd) == -1)
//     {
//         perror("pipe");
//         return (-1);
//     }
//     id = fork();
//     if (id == -1)
//     {
//         perror("fork");
//         return (-1);
//     }
//     else if(id == 0)
//     {
//         // pour premier pipe , infil rénisialise à -1
//         if(cmd->infil == -1)
//         {
//             dup2(pipe_fd[1], STDOUT_FILENO);
//             cmd->infil = pipe_fd[0];
//             cmd->outfil = pipe_fd[1];
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//         }
//         // pour pipe milieu
//         else if(cmd->infil != -1 && cmd->next)
//         {
//             dup2(cmd->infil, STDIN_FILENO);
//             dup2(cmd->outfil, STDOUT_FILENO);
//             cmd->infil = pipe_fd[0];
//             cmd->outfil = pipe_fd[1];
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//         }
//         // pour pipe dernier    
//         else
//         {
//             dup2(cmd->infil, STDIN_FILENO);
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//         }
//         path = get_path_cmd(cmd->arg[0]);
//         if(!path)
//         {            
//             fprintf(stderr, "Command not found: %s\n", cmd->arg[0]);
//             exit(1);
//         }
        // if(execve(path, cmd->arg, env) == -1)
        // {
        //     perror("execve");
        //     exit(1);
        // }
//     }
//     else
//     {
//         if(i < nbr_pipe)
//         {
//             close(pipe[0]);
//             close(pipe[1]);
//         }
//         waitpid(id, NULL, 0);
//         cmd = cmd->next;
//         i++;
//     }
    
// }