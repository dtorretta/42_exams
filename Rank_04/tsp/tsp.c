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
    int start;
    
    start = 0; //punto de partida
    //si solo hay 2 ciudades, la ditancia total es ida y vuelta
    if(start == len - 1)
    {
        total_distance = distance(x[0], y[0], x[1], y[1]);
        total_distance *= 2;
        return(total_distance);
    }
    else
    {
        j = 0;
        //si tengo 4 ciudades incluido el punto de partida (0 1 2 3) --> len 3 --> j puede llegar hasta 2
        //j=0 (0 1) calcula la distancia entre el punto de partida y la ciudad 1
        //j=1 (1 2) calcula la distancia entre ciudad 1 y la ciudad 2
        //j=2 (2 3) calcula la distancia entre ciudad 2 y la ciudad 3
        while(j < len) 
        {
            min_distance = INFINITY;
            //si tengo 4 ciudades (0 1 2 3) --> len 3 --> (start + i) puede llegar hasta 3
            i = 1; //empieza en 1 para comparar con la siguiente ciudad que no sea el punto de partida
            while ((start + i) <= len)
            {
                temp_distance = distance(x[start], y[start], x[start + i], y[start + i]); //comparo start con cada uno de los elementos
                if(temp_distance < min_distance)
                {
                    min_distance = temp_distance;
                    next = start + i;
                }
                i++;
            }
            total_distance += min_distance; 
            //ordeno las ciudades para que despues de start (start + 1) este la ciudad mas cerca (next)
            swap(&x[start + 1], &x[next], &y[start + 1], &y[next]);
            j++;
            start++; //mi nuevo punto de partida es next;
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
    len = i - 1;
    total_distance = find_shortest (x, y, len);
    printf("total distance: %.2f\n", total_distance);
    return(0);
}

//cc -Wall -Wextra -Werror tsp.c -lm
//add -LM to the cc