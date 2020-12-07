#include <stdio.h>
#include <stdlib.h>

int main(void){
  int ** T = (int**) malloc(15*sizeof(int*));
  for(int i = 0; i<15; i++){
    T[i]=(int *)malloc(15*sizeof(int));
  }
  for(int i=0;i<15;i++){
    for(int j=0;j<15;j++){
      T[i][j]=0;
      printf("%d", T[i][j]);
    }
    printf("\n");
  }
  int d;
  scanf("%d",&d);
  for(int i=0;i<15;i++){
    free(T[i]);
  }
  free(T);
  return 0;

}
