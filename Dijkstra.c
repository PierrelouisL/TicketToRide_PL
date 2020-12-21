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
int Minimal_len(int* been_here, int* D, int N){
  //printf("On entre dans Minimal_len\n");
  int min = 99;
  int indice_min = 0;

  /*printf("D:\n");
  for(int j = 0; j<N; j++){
    printf("%d ",D[j]);
  }
  printf("\n");
  printf("been_here:\n");
  for(int j = 0; j<N; j++){
    printf("%d ",been_here[j]);
  }
  printf("\n"); */
  for (int i = 0; i<N; i++){
    //printf("i  = %d, min = %d\n", i, min);
    if((been_here[i] == 0)&&(D[i]<min)){
      min = D[i];
      indice_min = i;
    }
  }
  //printf("Indice_min = %d\n", indice_min);
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
int* Dijkstra(int src, int** G, int N, int dest, int* how_many_tracks){
  //printf("on entre\n");
  int* been_here = (int*) calloc(N, sizeof(int));
  int D[100]; /* D[u] = dist between u and src */
  int last_cities_before[N];
  int* last_cities = (int*) calloc(N, sizeof(int));
  //int len_src_i[N];
  int u = src; /* Actual City */
  int cpt; /* temp cpt */
  //printf("src = %d N =%d dest = %d\n",src, N, dest);
  for(int i=0;i<N;i++){
    /* 99 represents infinity */
    D[i] = 99;
    been_here[i] = 0;
  }
  D[src] = 0;
  //len_src_i[src] = 0;
  while((u != dest)&&(cpt<100)){
    u = Minimal_len(been_here,D, N);
    been_here[u] = 1;
    for(int v = 0; v<N; v++){
      //printf("Dans la boucle %d %d %d\n", v, u, dest);
      if((been_here[v] == 0)&&(G[u][v] < 99)&&(D[u]+G[u][v] < D[v])){
        //printf("condition vérifée: D[u]= %d / G[u][v]= %d/ u = %d v =%d\n", D[u],G[u][v],u,v);
        D[v] = D[u]+G[u][v];
        last_cities_before[v] = u;
      }
    }
    cpt++;
  }
  /*for(int i = 0; i<N; i++){
    printf("%d: %d \n", i, D[i]);
  }*/
  cpt = 0;
  //printf("\n");
  /* Creating last_cities array because it wasn't exploitable before. */
  while(u != src){
    last_cities[cpt] = u;
    //printf("last_cities_before = %d ", last_cities_before[u]);
    u = last_cities_before[u];
    cpt++;
  }
  //printf("\nCPT = %d\n",cpt);
  *how_many_tracks = cpt;
  /* cpt >= 100 if the loop is called infinite so we want to indicate it in the returned value */
  if(cpt<100){
    return last_cities;
  }else{
    return NULL;
  }

}
