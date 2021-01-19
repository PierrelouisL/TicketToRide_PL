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

/* Every objective is now done so we're looking to score a maximum amount of point by taking tracks */
t_return_code obj_done_claimroute_or_pass(t_Tracksncities** T, t_Game* Game, int* game_over);

/* track_nb = 1 or 0 depending if we are looking for which color of the double track // always at 0 if single track */
t_return_code claimroute_(t_Tracksncities** T, t_Game* Game, int* pred, int** G,int i, char track_nb, char loco_or_not);
/* Show G array */
void show_G_array(t_Game* game, int** G);

/* Claim new objectives as the name says */
t_return_code claim_new_objs(t_Tracksncities** T, t_Game* Game, int** G);

/* This function goal is to find the shortest track available who is linked to one track or to both
  Track2 = 99 if we're looking to one point other if we look for the closest track linked to this track (Track1 - Track2)
  returns an array [2]which contains array[0]=shortest city and array[1]=edge of the track used */
int* find_shortest_track_linked(int track1, int track2, t_Tracksncities** T, t_Game* Game);

#endif
