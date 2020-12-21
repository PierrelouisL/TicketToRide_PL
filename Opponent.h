#ifndef __OPPONENT_H__
#define __OPPONENT_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"


t_return_code Opponent_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T, int** G);

void Opponent_s_move(t_Game* Game, t_move opponent_move, t_Tracksncities** T, int** G);


#endif
