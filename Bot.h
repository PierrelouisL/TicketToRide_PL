#ifndef __BOT_H__
#define __BOT_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"


/* Determining which move should the bot play and do it */
t_return_code Bot_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T, int** G);

/* User interface to debug and test functions */
t_return_code User_s_turn(t_Game* Game, int* game_over, t_Tracksncities** T);

/* Claim route if we have enough cards or do nothing */
t_return_code claimroute_or_pass(t_Tracksncities** T, t_Game* Game, int i, int* pred, int** G, int* game_over);

/* track_nb = 1 or 0 depending if we are looking for which color of the double track // always at 0 if single track*/
t_return_code claimroute_(t_Tracksncities** T, t_Game* Game, int* pred, int** G, int i, char track_nb);

/* Show G array */
void show_G_array(t_Game* game, int** G);
#endif
