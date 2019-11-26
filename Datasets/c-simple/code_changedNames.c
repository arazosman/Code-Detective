#include <stdio.h>
#include <stdlib.h>

int sumOfElements(int *arr, int num_of_elements);
int main()
{
    int num_of_elements;
    printf("# of elements: ");
    scanf("%d", &num_of_elements);
    int *A = (int *)malloc(num_of_elements*sizeof(int));
    
    for (int i = 0; i < num_of_elements; ++i)
    {
        printf("element #%d: ", i+1);
        scanf("%d", &A[i]);
    }

    int answer = sum(A, num_of_elements);

    if (answer == 0)
        printf("sum: zero %d\n", -answer);
    else
        printf("sum: non-zero %d\n", answer);

    return 0;
}

int sumOfElements(int *arr, int num_of_elements)
{
    int answer = 0;

    for (int ind = 0; ind < num_of_elements; ++ind)
        answer += arr[ind];

    return answer;
}