#include <stdlib.h>
#include <stdio.h>

//I SIMPLY DONT REMEMBER IF WE WERE ALLOW TO USE PRINTF :C

void print_value(int **array, int **print)
{
	int i = 0;
	int count = 0;

	while(print[i])
	{
		if(print[i][0] == 1)
			count++;
		i++;
	}
	i = 0;
	while(print[i])
	{
		if(print[i][0] == 1)
		{
			if(count > 1)
				printf("%d ", array[i][0]); //with space
			else
				printf("%d\n", array[i][0]); //with new line
		}
		i++;
		count--;
	}
}

void simple_powerset(int **array, int goal, int size)
{
	int i;
	int j;

	j = 0;
	while(j < size)
	{
		i = 0;
		if(array[j][0] == goal)
			printf("%d\n", array[j][0]);
		while((j + i) < size)
		{
			if(array[j][0] + array[(j + i)][0] == goal)
			{
				printf("%d %d\n", array[j][0], array[(j + i)][0]);
			}
			i++;
		}
		j++;
	}
}

void complex_powerset(int **array, int goal, int size, int **temp)
{
	int i;
	int j;
	int subtotal;

	j = 0;
	while(j < size)
	{
		i = 0;
		subtotal = 0;
		while((j + i) < size)
		{
			subtotal = array[j][0];
			while(subtotal + array[(j + i)][0] < goal && ((j + i) < size))
			{
				temp[j][0] = 1;
				temp[j + i][0] = 1;
				subtotal += array[(j + i)][0];
				i++;
			}
			if(subtotal == goal)
				print_value(array, temp);
			break;
		}
		j++;
	}
}

//just for debugging
// void print_array(int **array)
// {
// 	int i = 0;

// 	while(array[i])
// 	{
// 		printf("%d\n", array[i][0]);
// 		i++;
// 	}
// }

int main (int ac, char **av)
{
	int size = ac - 2;
	int goal = atoi(av[1]);
	int **array = malloc(size * sizeof(int *));
	int **print= malloc(size * sizeof(int *));
	if (!array || !print)
		return(1);
	
	int i = 0;
	while(i < size)
	{
		array[i] = malloc(sizeof(int));
		print[i] = malloc(sizeof(int));
		if(!array[i] || !print[i])
			return(1);
		array[i][0] = atoi(av[i + 2]);
		print[i][0] = 0;
		i++;
	}

	simple_powerset(array, goal, size);
	complex_powerset(array, goal, size, print);

	//I know, frees are incompleted
	free(array);
	free(print);
	return(0);
}