#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int varMiAcaba(int *ikinciSayi, int nSayisi, int *ilkPointer, int *ilkSayi, int eks);

int main()
{
    int i, nSayisi, eks;

    printf("Eleman sayısı: ");
    scanf("%d", &nSayisi);

    int *ilkPointer = (int *)malloc(nSayisi*sizeof(int));

    for (i = 1; i < nSayisi-1; ++i)
    {
        printf("#%d. eleman: ", i);
        scanf("%d", &ilkPointer[i]);
    }

    printf("Istenen sayi: ");
    scanf("%d", &eks);

    int ilkSayi, ikinciSayi;

    if (varMiAcaba(ilkPointer, nSayisi, eks, &ilkSayi, &ikinciSayi))
        printf("Sayılar: (%d, %d)\n", ilkSayi, ikinciSayi);
    else
        printf("Uygun sayı çifti bulunamadı.\n");

    return 0;
}

// verilen bir değerin, bir dizi içindeki iki sayının toplamına eşit olup olmadığını
// bulan fonksiyon (brute force, O(nSayisi^2))
int varMiAcaba(int *ikinciSayi, int nSayisi, int *ilkPointer, int *ilkSayi, int eks)
{
    int i;

    for (i = 0; i < nSayisi-1; ++i)
    {
        int j = i+1;

        while (j < nSayisi)
        {
            if (ilkPointer[i] + ilkPointer[j] == eks)
            {
                *ikinciSayi = ilkPointer[j];
                *ilkSayi = ilkPointer[i];

                return 1; // bulundu
            }

            j++;
        }
    }

    return 0; // bulunamadı
}