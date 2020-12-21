#ifndef __BOT_H__
#define __BOT_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"

t_return_code Bot_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T, int** G);
t_return_code User_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T);
t_return_code claimroute_or_pass(t_Tracksncities** T, t_Game* Game, int i, int* pred, int** G, int* game_over);

#endif
