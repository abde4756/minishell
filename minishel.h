/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishel.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:18:54 by aait-laf          #+#    #+#             */
/*   Updated: 2025/06/15 16:18:54 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHEL_H
#define MINISHEL_H

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <fcntl.h>

typedef enum s_type{
    CMD,
    WORD,
    PIPE,
};

typedef enum s_symbol{
    RED_IN,
    RED_OU,
    APPEND,
    HEREDOC,
} t_symbol;

typedef struct s_nfil{
    char *filename;
    enum t_symbol symbol;
} t_nfil;

// typedef struct s_ofil{
//     char *filename;
//     enum s_symbol  symbol;
//     struct s_ofil *next;
// } t_ofil;
typedef struct s_env
{
    int     idx_nod;
    char    *str;
    struct  s_env *next;
} t_env;

typedef struct s_cmd
{
    enum s_type  type; 
    char *path;
    char **arg;
    struct t_nfil  *fil;
    int     infil;
    int     outfil;
    // struct t_ofil  *outfil;
    int pipe[2];
    struct s_cmd *next;
} t_cmd;



char	*ft_strjoin(char *dest, char *src, char c);
int	word_count(char const *s, char c);
char	*alloc_words(char const *s, char c);
char	**f_free(char **p);
char	**ft_split(char const *s, char c);
void    print_str(char *str);
int     ft_isalnum(int c);
int     ft_isalpha(int c);
int     ft_isdigit(int c);
int     get_nbr_arg(char    **arg);

// build-in
int     fonc_cd(char *str, t_env *env);
int     echo_fonc(char *str);
int		fonc_pwd(char *str, t_env *env);
int     fonc_export(char *str, t_env *env);
int     unset_fonc(char *str, t_env *env);
int     env_fonc(char *str , t_env *env);
int     exit_fonc(char *str);

#endif 
