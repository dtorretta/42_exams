#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input");
}


int solve(char *s, int *i)
{
    int nums[500];
    int current = 0;
    int count = 0;
    char op = '+';
    
    
    if(s[*i] == ')')
    {
        unexpected(')');
        return 1;
    }
    
    while(s[*i] && s[*i] != ')')
    {
        if(isdigit(s[*i]))
            current = s[*i] - '0';
        else if(s[*i] == '(')
        {
            (*i)++;
            current = solve(s, i);
        }
        
        if(op == '+')
            nums[count++] = current;
        else if (op == '*')
            nums[count - 1] *= current;
        
        op = s[*i];
        
        if(s[*i] && s[*i] != ')')
            (*i)++;    
    }
    if(s[*i] == ')')
            (*i)++;  
    
    int k = 0;
    int res = 0;
    
    while(k < count)
    {
        res += nums[k];
        k++;
    }
    return (res);

}




int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	int i = 0;
	int res = solve(argv[1], &i);
	if (res == 1)
	    return 1;
	printf("%d\n", solve(argv[1], &i));
	return (0);
}