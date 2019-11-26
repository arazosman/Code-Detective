#include <stdio.h>
#include <stdlib.h>

int sum   (int *   A  , int    N);
int main() {

int N;
printf  ("Enter the number of elements: ");
scanf("%d", &N);
int *A =    (int *)malloc(N*sizeof(int));

    for (int i      = 0; i < N; ++i){
    printf("Enter the element #%d: ", i+1);
            scanf("%d", &A[i]);
    }

int ans =               sum(A  ,   N)   ;

            if (ans    == 0)
        printf("sum:        zero %d\n", -ans);
    else
            printf( "sum: non-zero %d\n", ans);

return 0;
}

            int sum(int *A, int N)
        {
            int         ans = 0;

            for (int        i = 0; i < N; ++i)
            {
                ans += A[i];
            }

            return ans;
        }