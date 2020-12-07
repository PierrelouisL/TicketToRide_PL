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


t_return_code User_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T){
  int move;
  t_return_code end;
  int city1, city2, color, nbLoco;
  t_color card;
  t_objective obj[3];
  int object[3] = {1,1,1};

  printf("It's your turn to play! Pick a move: ");
  scanf("%d",&move);
  switch (move) {
    case 1:
      printf("City1 City2 color nbLoco: ");
      scanf("%d %d %d %d",&city1, &city2, &color, &nbLoco);
      printf("%d %d %d %d",city1, city2, color, nbLoco);
      end = claimRoute(city1,city2,color,nbLoco);
      Game->players[Game->Player_nb].available_wagons -= nbLoco;
      Game->players[Game->Player_nb].cards_in_hand[color] -= nbLoco;
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      break;
    case 2:
      printf("drawObjectives\n");
      end = drawObjectives(obj);
      /* We choose to keep every objectives yet */
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      end = chooseObjectives(object);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      for(int i=0; i<3; i++){
        Game->players[Game->Player_nb].objectives[i] = obj[i];
        printf("%dth obj = %d %d %d ",i,obj[i].city1, obj[i].city2, obj[i].score);
      }
      Game->players[Game->Player_nb].Nb_objectives = 3;

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

t_return_code Bot_s_turn(t_Game* Game, int* game_over){
  t_color card_color;
  t_return_code end;
  end = drawBlindCard(&card_color);
  if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
    *game_over = 1;
    return end;
  }
  end = drawBlindCard(&card_color);
  if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
    *game_over = 1;
    return end;
  }
  Game->which_player = 1;
  return end;
}
