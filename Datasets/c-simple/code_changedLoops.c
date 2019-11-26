#include <stdio.h>
#include <stdlib.h>

int sum(int *A, int N);
int main()
{
    int N, i = 0;
    printf("Enter the number of elements: ");
    scanf("%d", &N);
    int *A = (int *)malloc(N*sizeof(int));
    
    while (i < N)
    {
        printf("Enter the element #%d: ", i+1);
        scanf("%d", &A[i]);
        ++i;
    }

    int ans = sum(A, N);

    if (ans == 0)
        printf("sum: zero %d\n", -ans);
    else
        printf("sum: non-zero %d\n", ans);

    return 0;
}

int sum(int *A, int N)
{
    int ans = 0, i = 0;

    while (i < N)
        ans += A[++i];

    return ans;
}