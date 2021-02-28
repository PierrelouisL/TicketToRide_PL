#include <stdio.h>
#include <stdlib.h>
#include "API/clientAPI.h"
#include "API/TicketToRideAPI.h"
#include "Opponent.h"
#include "Bot.h"
#include "Game.h"
#include "Dijkstra.h"
#include <string.h>

int take_1st_obj(t_Game* Game);


int main(void){
  /*extern int debug;
  debug = 2;*/
  int serv, is_tournament;
  char* tournament = (char*) calloc(20, sizeof(char));
  
  connectToServer("localhost", 1234, "BOT_PL");
  
  printf("If tournament write tournament Nb else write 00 for normal and 01 for duel: ");
  int c;
  while((c = getchar()) != '\n' && c != EOF); /* Empty buffer */
  gets(tournament);
  t_Game Game;
  do{
    if((tournament[0] != '0')&&(tournament[1] != '0')){
      printf("Tournament mode %s\n", tournament);
      Game = start_game(tournament, 0);
      is_tournament = 1;
    }else if((tournament[1] == '1')&&(tournament[0] == '0')){
      printf("duel mode, waiting for opponent...\n");
      Game = start_game(tournament, 2);
      is_tournament = 0;
    }else{
      printf("normal mode\n");
      Game = start_game(tournament ,1);
      is_tournament = 0;
    }
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
        if(end == LOOSING_MOVE){
          printf("win!\n");
        }
        if(end ==WINNING_MOVE){
          printf("loose!\n");
        }
      }else{
        //end = Bot_s_turn(&Game, &game_over);
        end = Bot_s_turn(&Game, &game_over, T, G);
        if(end == WINNING_MOVE){
          printf("win!\n");
        }
        if(end ==LOOSING_MOVE){
          printf("loose!\n");
        }
      }
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
  }while(is_tournament);

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
  Game->which_player = 1;
  return end;
}
