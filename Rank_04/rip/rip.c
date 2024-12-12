/*
rip
===============================================================================
allowed files: *.c, *.h
allowed functions: puts(), write()
===============================================================================
Write a program that will take as argument a string containing only parenthesis.

If the parenthesis are unbalanced (for example '())'), your program shall remove
the minimum number of parenthesis for the expression to be balanced.

You will then print all the solutions. (can be more than one)

The order of the solutions is not important.
===============================================================================

Examples:

> ./a.out '(()' | cat -e
 ()$
( )$
> ./a.out '((()()())())' | cat -e
((()()())())$
> ./a.out '()())()' | cat -e
()() ()$
()( )()$
( ())()$
> ./a.out '(()(()(' | cat -e 
(()  ) $
( )( ) $
( ) () $
 ()( ) $
 () () $
===============================================================================
*/	


#include <stdio.h>
#include <stdbool.h>

int ft_len(char *str)
{
    int i = 0;
    
    while(str[i])
        i++;
    return(i);    
}

int check_limits(char *str, int len)
{

    int i = 0;    
    while (str[i] == ' ')
        i++;
    while (str[len] == ' ')
        len--;
    if(str[i] == ')' || str[len]== '(')
        return(1);
    return(0);
}   
    
int check_balance(char *str)
{
    int count = 0;
    int i = 0;
    
    while (str[i] == ' ')
        i++;
        
    while (str[i])
    {
        if(str[i] == '(')
            count++;
        if(str[i] == ')')
            count--;
        i++;
    }
    
    if (count < 0)
        count *= -1;
        
    return(count);
}

void remove_parenthesis(char *str, int len, int start, int remove)
{
    int i;

    //delete the ')' from the beggining
    i = 0;
    while(i <= len && str[i] == ')' && remove > 0)
    {
        str[i] = ' ';
        remove--;
        i++;
    }   
    
    //delete the '(' from the end
    i = len;
    while(i >= 0 && str[i] == '(' && remove > 0)
    {
        str[i] = ' ';
        remove--;
        i--;
    }

    if(remove == 0)
    {
        if (!check_balance(str) && !check_limits(str, len))
            printf("%s\n", str);
        return;
    }

    i = start;
    while(i < len && remove > 0)
    {
        char temp = str[i];
        str[i] = ' ';
        remove_parenthesis(str, len, i + 1, remove - 1);
        str[i] = temp;
        i++;
    }
}

int main(int ac, char **av)
{
    if(ac != 2)
        return(1);
    int len = ft_len (av[1]) - 1;
    
    int remove = check_balance(av[1]);
    if (!remove && !check_limits(av[1], len))
    {
        printf("%s\n", av[1]);
        return(0);
    }
    remove_parenthesis(av[1], len, 0, remove);
    return(0);
}
