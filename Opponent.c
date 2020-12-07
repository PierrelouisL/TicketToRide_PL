/*-----------------------------------------------------------------

Opponent.c
We'll check opponent's moves, cards, destinations and try
to tell where is he heading to attempt to block him.
Mostly advanced functions

Pierre-Louis Lagunegrand EISE3

-----------------------------------------------------------------*/
#include <stdio.h>
#include "API/TicketToRideAPI.h"
#include "Opponent.h"


t_return_code Opponent_s_turn(t_Game* Game, int* game_over){
  t_move opponent_move;
  t_return_code end;
  int replay = 0;
  end = getMove(&opponent_move, &replay);
  if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
    *game_over = 1;
    return end;
  }
  if(replay){
    end = getMove(&opponent_move, &replay);
    if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
      *game_over = 1;
      return end;
    }
  }
  printf("replay = %d\n", replay);
  Game->which_player = 0;
  return end;
}
