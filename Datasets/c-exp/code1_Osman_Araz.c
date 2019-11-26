#include <stdio.h>
#include <stdlib.h>

int isExist(int *A, int N, int X, int *num1, int *num2);
int main()
{
    int i, N, X;

    printf("Eleman sayısı: ");
    scanf("%d", &N);

    int *A = (int *)malloc(N*sizeof(int));

    for (i = 0; i < N; ++i)
    {
        printf("#%d. eleman: ", i+1);
        scanf("%d", &A[i]);
    }

    printf("Aranılan toplam: ");
    scanf("%d", &X);

    int num1, num2;

    if (isExist(A, N, X, &num1, &num2))
        printf("Sayı çifti: (%d, %d)\n", num1, num2);
    else
        printf("Uygun sayı çifti bulunamadı.\n");

    return 0;
}

// verilen bir değerin, bir dizi içindeki iki sayının toplamına eşit olup olmadığını
// bulan fonksiyon (brute force, O(N^2))
int isExist(int *A, int N, int X, int *num1, int *num2)
{
    int i;

    for (i = 0; i < N-1; ++i)
    {
        int j = i+1;

        while (j < N)
        {
            if (A[i] + A[j] == X)
            {
                *num1 = A[i];
                *num2 = A[j];

                return 1; // bulundu
            }

            ++j;
        }
    }

    return 0; // bulunamadı
}
