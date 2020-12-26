#include <stdio.h>
#include <stdlib.h>
#include "API/clientAPI.h"
#include "API/TicketToRideAPI.h"
#include "Opponent.h"
#include "Bot.h"
#include "Game.h"
#include "Dijkstra.h"



int main(void){
  connectToServer("li1417-56.members.linode.com", 5678, "BOT_PL");
  t_Game Game = start_game();
  int game_over=0;
  t_return_code end;
  t_Tracksncities** T = Create_cities_array(Game);
  int** G = create_G_array(T, Game);
  /*int how_many_cities;
  int* pred = Dijkstra(2, G, Game.Board.Nb_Cities, 26, &how_many_cities);
  int u = 0;
  printf("pred : ");
  while(u<15){
    printf("%d ", pred[u]);
    u++;
  }
  printf("\n");*/
  //printMap();
  while(!game_over){
    if(Game.which_player){
      printMap();
      end = Opponent_s_turn(&Game, &game_over, T, G);
    }else{
      //end = Bot_s_turn(&Game, &game_over);
      end = Bot_s_turn(&Game, &game_over, T, G);
    }
  }
  if(end == WINNING_MOVE){
    printf("win!\n");
  }
  if(end ==LOOSING_MOVE){
    printf("loose!\n");
  }
  free(Game.Board.Tracks);
  for(int i = 0; i<Game.Board.Nb_Cities; i++){
    free(T[i]);
  }
  free(T);
  closeConnection();
  return 0;
}
