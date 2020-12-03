#ifndef __GAME_H__
#define __GAME_H__

typedef struct{
  int Nb_Cities;
  int Nb_Tracks;
  int* Tracks;
} t_Board;

typedef struct{
  int available_wagons;
  int Nb_cards_in_hand;
  int Nb_objectives;
  int cards_in_hand[9]; //	PURPLE,WHITE,BLUE,YELLOW,ORANGE,BLACK,RED,GREEN,MULTICOLOR
  t_objective objectives[20];
} t_Player;

typedef struct{
  short which_player; //0 if our bot playing and 1 if opponent playing
  short Player_nb; //Which player are we, number 1 or 0
  t_color faceup; // cards shown who can be picked
  t_Board Board;
  t_Player players[2];
} t_Game;

#endif
