/*-----------------------------------------------------------------

Dijkstra.c
Dijkstra algorithm applied to our game

Pierre-Louis Lagunegrand EISE3

-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "API/TicketToRideAPI.h"
#include "Dijkstra.h"

/* Returns the closest non visited city */
int Minimal_len(int** G, int* been_here, int src, int N){
  printf("On entre dans Minimal_len\n");
  int min = 99;
  int indice_min = src;
  for (int i = 0; i<N; i++){
    printf("i  = %d, min = %d\n", i, min);
    if((been_here[i] == 0)&&(G[src][i]<min)){
      min = G[src][i];
      indice_min = i;
    }
  }
  return indice_min;
}

int** create_G_array(t_Tracksncities **T, t_Game game){
  int ** G = (int**) calloc(game.Board.Nb_Cities, sizeof(int*));
  for(int i = 0; i<game.Board.Nb_Cities; i++){
    G[i]= (int*) calloc(game.Board.Nb_Cities, sizeof(int));
  }
  for(int i=0; i<game.Board.Nb_Cities; i++){
    for(int j=0; j<game.Board.Nb_Cities; j++){
      if(T[i][j].length == 0){
        if(i == j){
          G[j][j] = 0;
        }else{
          /* Infinity will be represented as 99 because tracks can not be that long */
          G[i][j] = 99;
          G[j][i] = 99;
        }
      }else{
        G[i][j] = T[i][j].length;
      }
    }
  }
  return G;
}

/* N = Nbcity Returns pred which the way to go to src to */
int* Dijkstra(int src, int** G, int N, int dest){
  int* been_here = (int*) calloc(N, sizeof(int));
  int D[100];
  int* last_cities = (int*) calloc(N, sizeof(int));
  //int len_src_i[N];
  int u = src; /* Actual City */

  for(int i=0;i<N;i++){
    /* 99 represents infinity */
    D[i] = 99;
    been_here[i] = 0;
  }
  //len_src_i[src] = 0;
  while(u != dest){
    u = Minimal_len(G, been_here, u, N);
    for(int v = 0; v<N; v++){
      printf("Dans la boucle %d %d %d\n", v, u, dest);
      if((been_here[v] == 0)&&(G[u][v] < 99)&&(D[u]+G[u][v] < D[v])){
        printf("condition vérifée\n");
        D[v] = D[u]+G[u][v];
        last_cities[v] = u;
      }
    }
    been_here[u] = 1;
  }
  return last_cities;
}
