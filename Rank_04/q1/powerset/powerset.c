#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
Backtracking is a technique where the program explores all possibilities to solve a problem 
and backtracks to try different options when it reaches a dead end.
For the example `./powerset 3 1 0 2 4 5 3`, the program attempts to construct all possible 
combinations of numbers (subsets) and checks if their sum equals the target (3). 
If a path does not work, it backtracks and tries another combination.
*/
void findSubsets(int* arr, int size, int target, int* subset, int subsetSize, int start) 
{
    int i = start;
    int sum = 0;
    int j = 0;
      
    while (j < subsetSize) 
    {
        sum += subset[j];
        j++;
    }
    
    // Without '&& subsetSize > 0', while testing `./powerset 0 1 -1`, it will print an empty line before the correct output.
    // I don't remember if we were meant to print this empty line before the correct output in the exam or not.
    if (sum == target && subsetSize > 0) 
    {
        j = 0;
        while (j < subsetSize) 
        {
            printf("%d ", subset[j]);
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
    int size = argc - 2; //not includes the target
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

