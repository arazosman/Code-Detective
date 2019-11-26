#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 1e9

int isExist(int*,int, int, int*, int*);
int main(int argc, char ** argv)
{
    int i = 0, N, X;

    printf("Eleman sayisi: ");
    scanf("%d", &N);

    int *A = calloc(N + 1, sizeof(int));
    for (i = N - 1 ; i >= 0; i--)
    {
        printf("#%d. eleman: ", N - i);
        scanf("%d", &A[i]);
    }
	A[N-1] = INT_MAX;
	
    printf("Aranilan toplam: ");
    scanf("%d", &X);

    int num1, num2;
	char buffer[40] = "Uygun sayi cifti bulunamadi.\n" ;
	if(isExist(A, N, X, &num1, &num2))
		sprintf(buffer, "Sayi cifti: (%d, %d)\n" ,  num1, num2 );
    printf("%s", buffer);
    return 0;
}

int isExist(int *A, int N, int X, int *num1, int *num2)
{
	int *p1, *p2;
    p1 = A;
    while(*p1 != INT_MAX){
    	p2 = A;
		while(*p2 != INT_MAX){
			if((*p2 + *p1)  == X){
				*num1 = *p1;
				*num2 = *p2;
				return 1;
			}
			*p2++;	
		}
		*p1++;	
	}
    return 0;
}
