/*-----------------------------------------------------------------

Bot.c
Here we'll define the bot behaviour and store every informations
we got on our cards/remaining trains and so on...

Pierre-Louis Lagunegrand EISE3

-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "API/TicketToRideAPI.h"
#include "Bot.h"
#include "Dijkstra.h"


t_return_code User_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T){
  int move;
  t_return_code end;
  int city1, city2, color, nbLoco;
  t_color card;
  t_objective obj[3];
  int object1[3] = {1,1,1}, object2[3] = {0,1,1};

  printf("It's your turn to play! Pick a move: ");
  scanf("%d",&move);
  switch (move) {
    case 1:
      printf("City1 City2 color nbLoco: ");
      scanf("%d %d %d %d",&city1, &city2, &color, &nbLoco);
      printf("%d %d %d %d",city1, city2, color, nbLoco);
      end = claimRoute(city1,city2,color,nbLoco);
      Game->players[Game->Player_nb].available_wagons -= T[city1][city2].length;
      Game->players[Game->Player_nb].cards_in_hand[color] -= T[city1][city2].length;
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      break;
    case 2:
      printf("drawObjectives\n");
      end = drawObjectives(obj);
      /* We choose to keep every objectives yet */
      if(Game->players[Game->Player_nb].Nb_objectives == 0){
        /* If its the first drawObjectives we can take 3 */
        end = chooseObjectives(object1);
        for(int i = 0; i<3; i++){
          Game->players[Game->Player_nb].objectives[i] = obj[i];
          //printf("%d %d %d\n",obj[i].city1, obj[i].city2, obj[i].score);
        }
        Game->players[Game->Player_nb].Nb_objectives = 3;
      }else{
        /* If its not the first we can only take 2 of them */
        end = chooseObjectives(object2);
        for(int i = Game->players[Game->Player_nb].Nb_objectives ; i<Game->players[Game->Player_nb].Nb_objectives+3 ; i++ ){
          Game->players[Game->Player_nb].objectives[i] = obj[i-Game->players[Game->Player_nb].Nb_objectives+1];
        }
        Game->players[Game->Player_nb].Nb_objectives += 2;
      }

      for(int i = 0;i<Game->players[Game->Player_nb].Nb_objectives; i++){
        printf("%dth obj = %d %d %d \n",i,Game->players[Game->Player_nb].objectives[i].city1, Game->players[Game->Player_nb].objectives[i].city2, Game->players[Game->Player_nb].objectives[i].score);
      }
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }

      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      break;
    case 3:
      printf("drawBlindCard\n");
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      for(int i=1;i<10;i++){
        printf("%d ",Game->players[Game->Player_nb].cards_in_hand[i]);
      }
      printf("\n");
      break;
    case 4:
      printf("drawBlindCard and seelen\n");
      scanf("%d %d",&city1,&city2);
      printf("T[%d][%d] = %d",city1,city2,T[city1][city2].length);
      printf("drawBlindCard\n");
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      for(int i=1;i<10;i++){
        printf("%d ",Game->players[Game->Player_nb].cards_in_hand[i]);
      }
      printf("\n");
      break;
    default:
      break;
  }
  Game->which_player = 1;
  return end;
}

t_return_code Bot_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T, int** G){
  t_return_code end;
  t_objective obj[3];
  int object1[3] = {1,1,1}, object2[3] = {1,1,0};
  int how_many_tracks = 0, obj_done = 0, all_obj_done = 1;
  int* pred;
  t_color card;

  if(Game->players[Game->Player_nb].Nb_objectives == 0){
    printf("drawObjectives\n");
    end = drawObjectives(obj);
    /* If its the first drawObjectives we can take 3 */
    end = chooseObjectives(object1);
    for(int i = 0; i<3; i++){
      Game->players[Game->Player_nb].objectives[i] = obj[i];
      //printf("%d %d %d\n",obj[i].city1, obj[i].city2, obj[i].score);
    }
    Game->players[Game->Player_nb].Nb_objectives = 3;
    printf("Obj = %d %d\n", Game->players[Game->Player_nb].objectives[0].city1, Game->players[Game->Player_nb].objectives[0].city2);
    Game->which_player = 1;
  }else if((Game->players[Game->Player_nb].Nb_objectives != 0) && (Game->which_player == 0)){
    /* We shouldnt go here if we have done every obj */
    for(int objective = 0 ; objective < Game->players[Game->Player_nb].Nb_objectives ; objective++){
      pred = Dijkstra(Game->players[Game->Player_nb].objectives[objective].city1, G, Game->Board.Nb_Cities, Game->players[Game->Player_nb].objectives[objective].city2, &how_many_tracks);
      //pred = Dijkstra(0, G, Game->Board.Nb_Cities, 19, &how_many_tracks);
      printf("Dijkstra says there is %d tracks to destination.\n", how_many_tracks);
      for(int j=0;j<=how_many_tracks;j++){
        printf("%d ", pred[j]);
      }
      for(int j=0; j<how_many_tracks;j++){
        if(G[pred[j]][pred[j+1]] == 0){
          obj_done = 1;
        }else{
          obj_done = 0;
        }
      }
      printf("\n");
      if(obj_done){
        printf("Obj done\n");
        Game->players[Game->Player_nb].objectives_done[objective] = 1;
      }
      /* We multiply 1 per each objdone so we get 1 after this loop only if every obj done is equal to one */
      for(int i =0;i<Game->players[Game->Player_nb].Nb_objectives;i++){
        printf("%d ",Game->players[Game->Player_nb].objectives_done[i]);
        all_obj_done *= Game->players[Game->Player_nb].objectives_done[i];
      }
      printf("\n");
      printf("all obj = %d \n",all_obj_done);
      if(all_obj_done){
        /* We pick an other set of objectives */
        /*end = drawObjectives(obj);
        end = chooseObjectives(object2);
        for(int i = Game->players[Game->Player_nb].Nb_objectives; i < (Game->players[Game->Player_nb].Nb_objectives + 2); i++ ){
          Game->players[Game->Player_nb].objectives[i] = obj[i - Game->players[Game->Player_nb].Nb_objectives];
        }
        printf("New objectives \n");
        Game->players[Game->Player_nb].Nb_objectives += 2;
        Game->which_player = 1;*/
      }else{
        for(int i = 0 ; i < (how_many_tracks) ; i++){
          end = claimroute_or_pass(T, Game, i, pred, G, game_over);
          if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
            *game_over = 1;
            return end;
          }
        }
      }
    }
    if(Game->which_player == 0){
      /* If we still didnt played (don't have enough cards to own a track)
        we pick cards, depending on which cards we want */
      printf("drawBlindCard\n");
      //show_G_array(Game, G);
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      Game->which_player = 1;
    }
  }
  free(pred);
  return end;
}

/* track_nb = 1 or 0 */
t_return_code claimroute_(t_Tracksncities** T, t_Game* Game, int* pred, int** G,int i, char track_nb, char loco_or_not){
  t_return_code end;
  t_color color;
  int nb_loco,nb_tr;
  if(loco_or_not){
    /* We're using a loco so we need to check how much loco do we need and complete it in the claimroute */
    /* Track length - nb of the appropriate track =  nbloco */
    nb_loco = T[pred[i]][pred[i+1]].length - Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]];
  }else{
    nb_loco=0;
  }
  if(track_nb){
    nb_tr = 2;
  }else{
    nb_tr = 1;
  }
  if((G[pred[i]][pred[i+1]] != 0)&&(G[pred[i]][pred[i+1]] < 99)){
    /* We claim the appropriate track */
    color = T[pred[i]][pred[i+1]].Track_color[nb_tr];
    printf("claimroute1 : %d %d %d %d\n", pred[i], pred[i+1],color, nb_loco);
    if((G[pred[i]][pred[i+1]] != 0)&&(G[pred[i+1]][pred[i]] != 0)){
      end = claimRoute(pred[i],pred[i+1],color, nb_loco);
      T[pred[i+1]][pred[i]].occupied = 2;
      T[pred[i]][pred[i+1]].occupied = 2;
      G[pred[i]][pred[i+1]] = 0;
      G[pred[i+1]][pred[i]] = 0;
      Game->players[Game->Player_nb].available_wagons -= T[pred[i]][pred[i+1]].length;
      Game->players[Game->Player_nb].cards_in_hand[color] -= (T[pred[i]][pred[i+1]].length - nb_loco);
      Game->which_player = 1;
      if(loco_or_not){
        Game->players[Game->Player_nb].cards_in_hand[MULTICOLOR] -= nb_loco;
      }
    }
  }
  return end;
}

t_return_code claimroute_or_pass(t_Tracksncities** T, t_Game* Game, int i, int* pred, int** G, int* game_over){
  t_return_code end;
  if((T[pred[i]][pred[i+1]].Track_color[0] == 0)&&(Game->which_player == 0)){
    /* Track_color[0] == 0 so it means that it's not a double track  */
    if(T[pred[i]][pred[i+1]].Track_color[1] == MULTICOLOR){
      /* We can use any track color so we'll try to use a color we don't need (yet at least) */
      int len = T[pred[i]][pred[i+1]].length;
      for(int j = 1; j < 9; j++){
        /* j represents the color of our card */
        if((G[pred[i]][pred[i+1]] != 0)&&(Game->players[Game->Player_nb].cards_in_hand[j] >= len)){
          /* We enough cards of j color to claim the route */
          /* We'll need to check if this is an appropriate color to use (if we dont need that color to claim an other track) */
          printf("We claim %d %d with %d color\n", pred[i], pred[i+1], j);
          claimRoute(pred[i], pred[i+1], j, 0);
          T[pred[i]][pred[i+1]].occupied = 2;
          T[pred[i+1]][pred[i]].occupied = 2;
          G[pred[i]][pred[i+1]] = 0;
          G[pred[i+1]][pred[i]] = 0;
          Game->players[Game->Player_nb].available_wagons -= T[pred[i]][pred[i+1]].length;
          Game->players[Game->Player_nb].cards_in_hand[j] -= T[pred[i]][pred[i+1]].length;
          Game->which_player = 1;
        }
      }
    }else{
      /* If the track is longer than 4 we use Locomotives otherwise we don't */
      if(T[pred[i]][pred[i+1]].length > 3){
        if(T[pred[i]][pred[i+1]].length <= (Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]]+Game->players[Game->Player_nb].cards_in_hand[MULTICOLOR])){
          printf("Condition1 verifiee pour %d %d tc = %d mult = %d\n", pred[i], pred[i+1], Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]],
                                                                                          Game->players[Game->Player_nb].cards_in_hand[MULTICOLOR]);
          end = claimroute_(T, Game, pred, G, i, 0, 1);
        }
      }else{
        if(T[pred[i]][pred[i+1]].length <= Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]]){
          printf("Condition1 verifiee pour %d %d tc = %d\n", pred[i], pred[i+1], Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]]);
          end = claimroute_(T, Game, pred, G, i, 0, 0);
        }
      }
    }
  }else{


    /* TODO: do the same thing for double track!!! */

    /* It is a double track so we need to check either if we have enough cards for each color */
    if(T[pred[i]][pred[i+1]].Track_color[1] == MULTICOLOR){
      /* We can use any track color so we'll try to use a color we don't need (yet at least) */
      int len = T[pred[i]][pred[i+1]].length;
      for(int j = 1; j < 9; j++){
        /* j represents the color of our card */
        if((G[pred[i]][pred[i+1]] != 0)&&(Game->players[Game->Player_nb].cards_in_hand[j] >= len)&&(Game->which_player == 0)){
          /* We enough cards of j color to claim the route */
          /* We'll need to check if this is an appropriate color to use (if we dont need that color to claim an other track) */
          printf("We claim2 %d %d with %d color\n", pred[i], pred[i+1], j);
          claimRoute(pred[i], pred[i+1], j, 0);
          T[pred[i]][pred[i+1]].occupied = 2;
          T[pred[i+1]][pred[i]].occupied = 2;
          G[pred[i]][pred[i+1]] = 0;
          G[pred[i+1]][pred[i]] = 0;
          Game->players[Game->Player_nb].available_wagons -= T[pred[i]][pred[i+1]].length;
          Game->players[Game->Player_nb].cards_in_hand[j] -= T[pred[i]][pred[i+1]].length;
          Game->which_player = 1;
        }
      }
    }else{
      if((T[pred[i]][pred[i+1]].length <= Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]])&&(Game->which_player == 0)){
        /* We claim the appropriate track */
        printf("Condition2 verifiee pour %d %d tc = %d tc2= %d  %d  %d\n", pred[i], pred[i+1],
                                                        Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[1]],
                                                        Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[2]],
                                                        T[pred[i]][pred[i+1]].Track_color[1], T[pred[i]][pred[i+1]].Track_color[2]);
        end = claimroute_(T, Game, pred, G, i, 0, 0);
      }else if((T[pred[i]][pred[i+1]].length <= Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[2]])&&(Game->which_player == 0)){
        /* We don't have enough cards of the first color so we check the other track color */
        /* We claim the appropriate track */
        end = claimroute_(T, Game, pred, G, i, 1, 0);
        printf("Condition3 verifiee pour %d %d tc = %d\n", pred[i], pred[i+1], Game->players[Game->Player_nb].cards_in_hand[T[pred[i]][pred[i+1]].Track_color[2]]);
      }
    }
  }
  return end;
}

/* Checks if we need to be claiming essentials_routes or not

*/
//int essentials_routes(){

//}

void show_G_array(t_Game* game, int** G){
  printf("| ");
  for(int i = 0;i<game->Board.Nb_Cities;i++){
    for(int j = 0;j<game->Board.Nb_Cities;j++){
      printf("%d | ",G[i][j]);
    }
    printf("\n---------------------------------------------------------------------------------------");
    printf("-----------------------------------------------------------------------------------------\n| ");
  }
}
