/*-----------------------------------------------------------------

Opponent.c
We'll check opponent's moves, cards, destinations and try
to tell where is he heading to attempt to block him.

Pierre-Louis Lagunegrand EISE3

-----------------------------------------------------------------*/
#include <stdio.h>
#include "API/TicketToRideAPI.h"
#include "Opponent.h"


t_return_code Opponent_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T, int** G){
  t_move opponent_move;
  t_return_code end;
  int replay = 1;
  while(replay){
    end = getMove(&opponent_move, &replay);
    if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
      *game_over = 1;
      return end;
    }
    //printf("getmove : %d opponent_move %d\n", replay, opponent_move.type);
    Opponent_s_move(Game, opponent_move, T, G);
  }
  Game->which_player = 0;
  return end;
}

void Opponent_s_move(t_Game* Game, t_move opponent_move, t_Tracksncities** T, int** G){
  switch(opponent_move.type){
    case CLAIM_ROUTE:
      /* The opponent just claimed a route so we need to update our array */
      T[opponent_move.claimRoute.city1][opponent_move.claimRoute.city2].occupied = 1;
      T[opponent_move.claimRoute.city2][opponent_move.claimRoute.city1].occupied = 1;
      Game->players[1-Game->Player_nb].available_wagons -= T[opponent_move.claimRoute.city1][opponent_move.claimRoute.city2].length;
      Game->players[1-Game->Player_nb].Nb_cards_in_hand -= T[opponent_move.claimRoute.city1][opponent_move.claimRoute.city2].length;
      G[opponent_move.claimRoute.city1][opponent_move.claimRoute.city2] = 99;
      G[opponent_move.claimRoute.city2][opponent_move.claimRoute.city1] = 99;
      Game->players[1-Game->Player_nb].last_track[0] = opponent_move.claimRoute.city1;
      Game->players[1-Game->Player_nb].last_track[1] = opponent_move.claimRoute.city2;
      break;
    case DRAW_CARD:
      /* The opponent just picked a faceup card so we might use it later on our strat */
      for(int i = 0; i<5;i++){
        Game->faceup[i] = opponent_move.drawCard.faceUp[i];
      }
      Game->players[1-Game->Player_nb].Nb_cards_in_hand++;
      break;
    case DRAW_BLIND_CARD:
      Game->players[1-Game->Player_nb].Nb_cards_in_hand++;
      break;
    case CHOOSE_OBJECTIVES:
      Game->players[1-Game->Player_nb].Nb_objectives += opponent_move.chooseObjectives.nbObjectives;
      break;
    case DRAW_OBJECTIVES:
      /* Here the game just sends 3 objective cards to the opponent so nothing to get here */
      break;
  }
  Game->which_player = 0;
}
