#include <stdio.h>
#include <stdlib.h>

/*
Backtracking es una técnica donde el programa explora todas las posibilidades 
para resolver un problema y retrocede para probar opciones diferentes cuando llega a un callejón sin salida.

para el ejemplo ()./powerset 3 1 0 2 4 5 3) el programa intenta construir todas las combinaciones posibles de números (subsets) 
y verificar si su suma es igual al objetivo (3). Si un camino no sirve, retrocede y prueba otra combinación.
*/
void findSubsets(int* arr, int size, int target, int* subset, int subsetSize, int start) {
    int i = start;
    int sum = 0;
    int j = 0;
    
    
    int x = -1;
    printf("subset: ");
     while (++x < size) 
        printf("%d ", subset[x]);
    printf(" subsetSize: %d ", subsetSize);
    printf(" start: %d ", start);
    
    
    while (j < subsetSize) 
    {
        sum += subset[j];
        j++;
    }
    
    printf(" SUM: %d ", sum);
    printf("\n");
    
    if (sum == target) 
    {
        j = 0;
        while (j < subsetSize) 
        {
            printf("final: %d ", subset[j]);
            j++;
        }
        printf("\n");
    }

    while (i < size) 
    {
        subset[subsetSize] = arr[i];
        findSubsets(arr, size, target, subset, subsetSize + 1, i + 1);
        i++;
    }
}

int main(int argc, char **argv) 
{
    int target = atoi(argv[1]);
    int size = argc - 2; //not including the target
    int i = -1;
    
    int *subset = malloc(size * sizeof(int));
    if (!subset)
        return(1);

    int *arr = malloc(size * sizeof(int));
    if (!arr)
        return(1);
        
    while (++i < size) 
        arr[i] = atoi(argv[i + 2]);
        
    findSubsets(arr, size, target, subset, 0, 0);

    free(arr);
    free(subset);
    return 0;
}

