#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"

void Dijkstra();

void Minimal_len(int** D, int* been_here, int* closest_city, int src);


int** create_D_array(t_Tracksncities **T, t_Game game);

#endif
