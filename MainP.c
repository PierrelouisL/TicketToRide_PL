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
  int** D = create_D_array(T, Game);
  //printMap();
  while(!game_over){
    printMap();
    if(Game.which_player){
      end = Opponent_s_turn(&Game, &game_over,T);
    }else{
      //end = Bot_s_turn(&Game, &game_over);
      end = User_s_turn(&Game, &game_over, T);
    }
  }
  if(end == WINNING_MOVE){
    printf("win!\n");
  }
  if(end ==LOOSING_MOVE){
    printf("loose!\n");
  }
  free(Game.Board.Tracks);
  closeConnection();
  return 0;
}
