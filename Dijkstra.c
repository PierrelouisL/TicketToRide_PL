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

void Minimal_len(int** D, int* been_here, int* closest_city, int src){
  float min = INFINITY;
  for (int i = 0; N-1; i++0){
    if(been_here[i] == 0)(T[src][i]<min){
      min = T[src][i];
      indice_min =i;
    }
  }

}

int** create_D_array(t_Tracksncities **T, t_Game game){
  int ** D = (int**) calloc(game.Board.Nb_Cities, sizeof(int*));
  for(int i = 0; i<game.Board.Nb_Cities; i++){
    D[i]= (int*) calloc(game.Board.Nb_Cities, sizeof(int));
  }
  for(int i=0; i<game.Board.Nb_Cities; i++){
    for(int j=0; j<game.Board.Nb_Cities; j++){
      if(T[i][j].length == 0){
        if(i == j){
          D[j][j] = 0;
        }else{
          /* Infinity will be represented as 99 because tracks can not be that long */
          D[i][j] = 99;
          D[j][i] = 99;
        }
      }else{
        D[i][j] = T[i][j].length;
      }
    }
  }
  return D;
}
