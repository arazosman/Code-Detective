/*
*
* this document hacked by fati
* xxx 
* xx
* xx
*/
#include <stdlib.h>
#include <stdio.h>


// ccc
// bbbbb
// aaaaaaaa
// ddd
int findsum( int N, int X, int *num1, int *num2 ,int *A);


// xxxx 
// aaaa
// bbbb
int main()
{
    int  count, target,i;

    printf("Eleman sayısı giriniz");
    scanf("%d", &count);

    int *dizi= (int *)calloc(count , sizeof(int) );

    for (i = 0; i < count; ++i)
    {
        printf("#%d. eleman: ", i+1);
        scanf("%d", &dizi[i]);
    }

    printf("Aranılan toplam: ");
    scanf("%d", &target);

    int num1, num2;
    /*
  	 some lines here....
    */

    if (findsum( count, target, &num1, &num2 , dizi))
    {

        printf("ciftler bulundu\n");
        printf("cift 1: %d\n", num1);
        printf("cift 2: %d\n", num2);
    }
    else
    {
        printf("ciftler bulunamadi :( \n");
    }

    // lutfen bedavalamayi unutmayalim , burasi cok onemli...
    free(dizi);

    return 0;
}

//def def def def def def def
// def def def def def def def
//  def def def def def def def 
int findsum(int N, int X, int *num1, int *num2,int *A)
{
	// burada pek bir sey calamamadım :/
    int i;

    for (i = 0; i < N-1; i+= 1)
    {
        int j = i+1;

        while (j < N)
        {
            if (A[i] + A[j] == X)
            {
                *num1 = A[i];
                *num2 = A[j];

                // olduuuu	......
                return 1; 
            }
            j += 1;
        }
    }

 	// ret 0 ....
    return 0;
}