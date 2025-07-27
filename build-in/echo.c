#include "minishell.h"

int     echo_fonc(char **arg)
{
    int j;
    int flag_valid;
    int i = 1;
    int    flag_newline = 1;

    if(!arg || !arg[0])
        return(-1);
    // virifier si il y'a fflag -n ou plusieur -nn ou -n -n 
    while(arg[i] && arg[i][0] == '-' && arg[i][1] && arg[i][1] == 'n')
    {
        j = 1;
        flag_valid = 1;
        while(arg[i][j])
        {
            if(arg[i][j] != 'n')
            {
                flag_valid = 0;
                break;
            }
            j++;
        }
        if(flag_valid && j > 1) // on a plusieur n et aussi que n 
        {
            flag_newline = 0;
            i++;
        }
        else
            break; // on va traiter comme un argement normale;
    }
    while (arg[i])
    {
        printf("%s", arg[i]);
        if(arg[i + 1])
            printf(" "); // si il y'a un autre arg
        i++;
    }
    if(flag_newline)
        printf("\n");
    return(0);
}