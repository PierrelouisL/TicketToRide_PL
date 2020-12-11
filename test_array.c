#include <stdio.h>
#include <stdlib.h>

void aff(int** d1){
  for(int i=0;i<15;i++){
    printf("%d ",d1[i]);
  }
}

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
  printf("\n");
  aff(T);
  scanf("%d",&d);
  for(int i=0;i<15;i++){
    free(T[i]);
  }
  free(T);
  return 0;

}
