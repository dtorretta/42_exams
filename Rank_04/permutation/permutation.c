#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ft_swap (char *a, char *b)
{
    char temp;
    
    temp = *a;
    *a = *b;
    *b = temp;
}

void reverse (char *str, int start, int end)
{
    while (end > start)
    {
        ft_swap(&str[start], &str[end]);
        start++;
        end--;
    }
}

int next_permutation (char *str, int len)
{
    int i = len - 1;
    int j = len;
    
    //find the str[i] < str[i + 1]
    while(i >= 0 && str[i] >= str[i + 1])
        i--;

    //no more permutations
    if(i < 0)
        return(0);
        
    //finde the a str[j] bigger than str[i]
    while(str[j] <= str[i])
        j--;
    ft_swap(&str[i], &str[j]);
    reverse(str, i + 1 , len);
    return(1);    
}

void bubble_sort (char *str, int len)
{
    int j;
    int i;
    int temp;
    
    j = 0;
    while (j < len)
    {
        i = 0;
        while(i < len - 1)
        {
            if(str[j] > str[j + 1])
            {
                temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;            
            }        
            i++;
        }
        j++;    
    }
}

int ft_len(char *str)
{
    int i = 0;
    
    while(str[i])
        i++;
    return(i);    
}

int main (int ac, char **av)
{
    if(ac != 2)
        return(1);
    int len =  ft_len(av[1]) - 1;
    bubble_sort(av[1], len);
    printf("%s\n", av[1]);
    while(next_permutation(av[1], len))
        printf("%s\n", av[1]);
    return(0);
}

/*
examples:

./permutations a
a

./permutations ab
ab
ba

./permutations abc
abc
acb
bac
bca
cab
cba
*/