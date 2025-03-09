/*
Assignment name			: tsp
Expected files			: *.c *.h
Allowed functions   : write, sqrtf, getline, fseek, fscanf, ferror, feof, fabsf, memcpy, fprintf, fclose, malloc, calloc, realloc, free, fopen, errno, stderr, stdin, stdout
-------------------------------------------------------

The first publication referring to this problem as the "traveling salesman 
problem" is found in the 1949 RAND Corporation report by Julia Robinson, 
"On the Hamiltonian game (a traveling salesman problem)."

Here is how she defines the problem:

"The purpose of this note is to give a method for solving a problem related
to the traveling salesman problem. It seems worthwhile to give a description
of the original problem. One formulation is to find the shortest route for a
salesman starting from Washington, visiting all the state capitals and then
returning to Washington.

More generally, to find the shortest CLOSED CURVE containing n given points 
in the plane."

for example with the following set of cities:
0, 0
1, 0
2, 0
0, 1
1, 1
2, 1
1, 2
2, 2
which can be presented as follows:
+ + +
+ + +
  + +
the shortest path is:
 _____
|__   |
   |__|

so you should print the length of this path that is:
8.00

Write a program that will read a set of city coordinates in the form 
'%f, %f\n' from the standard input and will print the length of the shortest
possible path containing all these cities under the form '%.2f'.

Your program will not be tested with more than 11 cities.

You will find in this directory a file tsp.c containing all the boring parts of
this exercise and example files to help you test your program.

hint: in order to use sqrtf, add -lm at the end of your compilation command.

For example this should work:
$> cat square.txt
1, 1
0, 1
1, 0
0, 0
$> ./tsp < square.txt | cat -e
4.00$
*/

#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

float distance (float x0, float y0, float x1, float y1)
{
    float diff[2] = { x1 - x0,
                    y1 - y0};
    return (sqrtf(diff[0] * diff[0] + diff[1] * diff [1]));
}

void swap(float *x0, float *x1, float *y0, float *y1)
{
    float tempx;
    float tempy;
    
    tempx = *x0;
    tempy = *y0;
    *x0 = *x1;
    *y0 = *y1;
    *x1 = tempx;
    *y1 = tempy;
}
float find_shortest (float *x, float *y, int len)
{
    int i;
    int j;
    int next;
    float min_distance;
    float temp_distance;
    float total_distance;
    
    j = 0; //punto de partida
    
    if(j == len - 1) //si solo hay 2 ciudades, la ditancia total es ida y vuelta
    {
        total_distance = distance(x[0], y[0], x[1], y[1]);
        total_distance *= 2;
        return(total_distance);
    }
    else
    {
        //si len es 3 --> max j posible es 2
        //j=0 (0 1) calcula la distancia entre el punto de partida y la ciudad 1
        //j=1 (1 2) calcula la distancia entre ciudad 1 y la ciudad 2
        //j=2 (2 3) calcula la distancia entre ciudad 2 y la ciudad 3
        while(j < len) 
        {
            min_distance = INFINITY;
            //si len es 3 --> i puede llegar hasta 3
            i = j + 1; //empieza en 1 para comparar con la siguiente ciudad que no sea el punto de partida
            while (i <= len)
            {
                temp_distance = distance(x[j], y[j], x[i], y[i]); //comparo start(j) con cada uno de los elementos
                if(temp_distance < min_distance)
                {
                    min_distance = temp_distance;
                    next = i;
                }
                i++;
            }
            total_distance += min_distance; 
            //al principio 'j' es el punto de partida
            //next es la ciudad mas cercana a 'j' que debe reordenarse y quedar consecutiva a esta
            //para ello hago swap de 'j + 1' y 'next'
            swap(&x[j + 1], &x[next], &y[j + 1], &y[next]);
            j++; //mi nuevo punto de partida es 'next' que ahora esta posicionado en 'j + 1'
        }
        total_distance += distance(x[len], y[len], x[0], y[0]); //vuelta al punto de partida
        return(total_distance);
    }
}

int main (int ac, char **av)
{
    (void) ac;
    (void) av;
    float x[11];
    float y[11];
    int i;
    int len;
    float total_distance;
    
    i = 0;    
    //dentro de 'x' e 'y' se van a almacenar todas las coordenadas
    while (fscanf(stdin, "%f, %f", &x[i], &y[i]) == 2)
    {
        i++;    
    }
    len = i - 1; //len es la cantidad de ciudades SIN contar el punto de partida
    total_distance = find_shortest (x, y, len);
    printf("total distance: %.2f\n", total_distance);
    return(0);
}

//cc -Wall -Wextra -Werror tsp.c -lm
//add -LM to the cc