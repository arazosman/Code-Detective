#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool search(int *, int, int, int *, int *);

int main()
{
    int i, size, sum, x, y;
    int *A = malloc(size*sizeof(int));

    printf("Boyut: ");
    scanf("%d", &size);

    for (i = 0; i < size; ++i)
    {
        printf("A[%d] giriniz: ", i+1);
        scanf("%d", &A[i]);
    }

    printf("Istenilen toplam: ");
    scanf("%d", &sum);

    if (search(A, size, sum, &x, &y))
        printf("Sayi cifti: (%d, %d)\n", x, y);
    else
        printf("Sayi cifti yok.\n");

    return 0;
}

bool search(int *A, int size, int sum, int *x, int *y)
{
    int i, j;
    for (i = 0; i < size-1; ++i)
        for (j=i+1 ; j<size ; j++)
            if (A[i] + A[j] == sum)
            {
                *x = A[i];
                *y = A[j];

                return 1;
            }
    return 0;
}