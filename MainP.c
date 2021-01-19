#include <stdio.h>
#include <stdlib.h>
#include "API/clientAPI.h"
#include "API/TicketToRideAPI.h"
#include "Opponent.h"
#include "Bot.h"
#include "Game.h"
#include "Dijkstra.h"

int take_1st_obj(t_Game* Game);


int main(void){
  int serv;
  printf("Which server do u want to connect to? (1 for 1234 anything else for 5678) : ");
  scanf("%d", &serv);
  if(serv == 1){
    connectToServer("li1417-56.members.linode.com", 1234, "BOT_PL");
  }else{
    connectToServer("li1417-56.members.linode.com", 5678, "BOT_PL");
  }
  t_Game Game = start_game();
  int game_over = 0;
  t_return_code end = NORMAL_MOVE;
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
  if(Game.which_player){
    end = take_1st_obj(&Game);
  }else{
    end = Opponent_s_turn(&Game, &game_over, T, G);
    end = take_1st_obj(&Game);
  }
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
    free(G[i]);
  }
  free(G);
  for(int i = 0; i<Game.Board.Nb_Cities; i++){
    free(T[i]);
  }
  free(T);
  closeConnection();
  return 0;
}

int take_1st_obj(t_Game* Game){
  t_return_code end = NORMAL_MOVE;
  t_objective obj[3];
  int object1[3] = {1,1,1};
  printf("1st drawObjectives\n");
  drawObjectives(obj);
  /* If its the first drawObjectives we can take 3 */
  chooseObjectives(object1);
  for(int i = 0; i<3; i++){
    Game->players[Game->Player_nb].objectives[i] = obj[i];
  }
  Game->players[Game->Player_nb].Nb_objectives = 3;
  printf("Obj = %d %d\n", Game->players[Game->Player_nb].objectives[0].city1, Game->players[Game->Player_nb].objectives[0].city2);
  return end;
}
