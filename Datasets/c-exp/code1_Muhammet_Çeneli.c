#include <stdio.h>
#include <stdlib.h>

int isExist(int *A,int N,int X,int num1,int num2){
    int i,j;

    for(i=0;i<N-1;i++){
        j=i+1;
       while (j<N){
            if (A[i]+A[j]==X){
                num1=A[i];
                num2=A[j];
		   return 1;
            }
		j++;
        }
    }
    return 0;
}

int main(){
    int i,N,X,A[50],num1,num2;

    printf("Eleman sayısı: ");
    scanf("%d",&N);
    
    for(i=0;i<N;i++){
        printf("#%d. eleman: ",i+1);
        scanf("%d",&A[i]);
    }

    printf("Aranılan toplam: ");
    scanf("%d",&X);

    if(isExist(A,N,X,num1,num2)){
        printf("Sayı çifti: (%d, %d)\n",num1,num2);
    }else{
        printf("Uygun sayı çifti bulunamadı.\n");
    }
    return 0;
}