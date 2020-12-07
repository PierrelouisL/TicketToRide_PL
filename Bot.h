#ifndef __BOT_H__
#define __BOT_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"

t_return_code Bot_s_turn(t_Game* Game, int* game_over);
t_return_code User_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T);

#endif
