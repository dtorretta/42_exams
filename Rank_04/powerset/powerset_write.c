#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//this version is with write instead of printf
//IN PROGRESS

void write_int(int num)
{
	char buffer[12];  // enough to store an int in ASCII form
	int i = 0;
	if (num == 0) {
		write(1, "0", 1);
		return;
	}
	// Handle negative numbers
	if (num < 0) {
		write(1, "-", 1);
		num = -num;
	}
	// Convert number to string
	while (num > 0) {
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	// Write in reverse order
	while (--i >= 0) {
		write(1, &buffer[i], 1);
	}
}

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
			write_int(array[i][0]);  // Print the number
			if(count > 1)
				write(1, " ", 1);  // Space
				//printf("%d ", array[i][0]);
			else
				write(1, "\n", 1);  // New line
				//printf("%d\n", array[i][0]);
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
				write_int(array[j][0]);
				write(1, " ", 1);  // Space
				write_int(array[(j + i)][0]);
				write(1, "\n", 1);  // New line
				//printf("%d %d\n", array[j][0], array[(j + i)][0]);
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