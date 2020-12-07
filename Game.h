#ifndef __GAME_H__
#define __GAME_H__

#include "API/TicketToRideAPI.h" 

typedef struct{
  int Nb_Cities;              /* How many cities there is on the map */
  int Nb_Tracks;              /* How many tracks there is on the map */
  int* Tracks;                /* cf getMap() comment */
} t_Board;

typedef struct{
  t_color Track_color[3];     /* Track_color[0] indicates if the track is double or not */
  short occupied;             /* Indicates if the track has already been claimed by a player, 0 if not 1 if enemy 2 if our bot*/
  short length;               /* 0 if the track doesnt exist */
} t_Tracksncities;

typedef struct{
  int available_wagons;       /* How many available wagons the player has */
  int Nb_cards_in_hand;       /* How many cards the player have in his hand */
  int Nb_objectives;          /* The number of objectives the player has */
  int cards_in_hand[10];      /* PURPLE,WHITE,BLUE,YELLOW,ORANGE,BLACK,RED,GREEN,MULTICOLOR */
  t_objective objectives[20]; /* Stores every of the player's objectives */
} t_Player;

typedef struct{
  short which_player;         /* 0 if our bot playing and 1 if opponent playing */
  short Player_nb;            /* Which player are we, number 1 or 0 */
  t_color faceup[5];          /* Cards shown who can be picked */
  t_Board Board;              /* The actual board on which the game is played */
  t_Player players[2];        /* The two players in the game */
} t_Game;

/* This function Initialize the game */
t_Game start_game();


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Create an bi dimensional array to store infos about cities
Returns T[City1][City2]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
t_Tracksncities** Create_cities_array(t_Game game);

/* Just prints the T array on the console (must be game.Board.Nb_Cities*game.Board.Nb_Cities large)
~~~~ Prettier when its USA MAP~~~~*/
void show_T_array(t_Game game, t_Tracksncities** T);

#endif
