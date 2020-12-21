#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "API/TicketToRideAPI.h"
#include "Game.h"

/*typedef enum {
  NONE = 0,
  VANCOUVER_SEATTLE,
  SEATTLE_PORTLAND,
  VANCOUVER_CALGARY,
  LOSANGELES_LASVEGAS,
  DULUTH_OMAHA,
  OMAHA_KANSASCITY,
  KANSASCITY_OKLAHOMACITY,

}t_objs;*/

int* Dijkstra(int src, int** G, int N, int dest,int* how_many_tracks);

int Minimal_len(int* been_here, int* D,int N);


int** create_G_array(t_Tracksncities **T, t_Game game);

#endif
