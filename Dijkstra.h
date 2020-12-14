#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"

int* Dijkstra(int src, int** G, int N, int dest);

int Minimal_len(int** G, int* been_here, int src, int N);


int** create_G_array(t_Tracksncities **T, t_Game game);

#endif
