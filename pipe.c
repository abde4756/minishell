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

#include "minishel.h"

int     count_nuber_cmd(t_cmd *cmd)
{
    int     count;

    count = 0;
    while(cmd)
    {
        if(cmd->rdr->type == CMD)
            count++;
        cmd = cmd->next;
    }
    return(count);
}


int     execute_whith_pipe(t_cmd *cmd)
{
    int     nbr_pipe;
    int     i = 0;
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
            signal(SEGQUIT, SIG_DFL);
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
            if(execvp(cmd->arg[0], cmd->arg) == -1)
                    perror("execvp"), free(pipe),exit(1);
        }
        else if(id > 0) 
        {
            if(i < nbr_pipe)
            {
                close(pipe[i][0]);
                close(pipe[i][1]);
            }
            waitpid(id, NULL, 0);
            cmd = cmd->next;
            i++;
        }
    }
    free(pipe);
    return(0);
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

















void    check_pipe(t_cmd *cmd)
{
    while (cmd)
    {
        if(cmd->type == cmd)
        {
            if(cmd->next->type == PIPE)
            {
                if(pipe(cmd->pipe))
                    perror(pipe), exit(1);
                if(cmd->infil == -1)
                {
                    pid_t fid1, fid2;
                    fid1 = fork();
                    if(fid1 == 0)
                    {
                        dep2(cmd->pipe[1], STDOUT_FILENO);
                        close(cmd->pipe[1]);
                        close(cmd->pipe[0]);
                        execvp(cmd->arg[0], cmd->arg);
                    }
                    else
                    {
                        close(cmd->pipe[0]);
                        close(cmd->pipe[1]);
                    }
                    // pour la dexiem commande
                    fid2 = fork();
                    if(fid2 == 0)
                    {
                        dep2(cmd->next->pipe[0], STDIN_FILENO);
                        close(cmd->next->pipe[0]);
                        close(cmd->next->pipe[1]);
                        execvp(cmd->next->arg[0], cmd->arg);
                    }
                    else
                    {
                        close(cmd->next->pipe[0]);
                        close(cmd->next->pipe[1]);
                    }
                }
                
                
            }
        }
        cmd = cmd->next;
    }
    
}