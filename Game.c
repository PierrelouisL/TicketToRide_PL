/*-----------------------------------------------------------------

Game.c
Here is every informations we have on the actual game such as
who's playing, how many cards left...

Pierre-Louis Lagunegrand EISE3

-----------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "API/TicketToRideAPI.h"
#include "Game.h"

t_Game start_game(){
  t_Game Game;
  t_color cards[4];
  char* gameName = (char*) malloc(15*sizeof(char));
  strcpy(gameName, "Partie PL");

  waitForT2RGame("TRAINING PLAY_RANDOM timeout=100 start=0 map=USA", gameName, &Game.Board.Nb_Cities, &Game.Board.Nb_Tracks);
  Game.Board.Tracks = (int*) malloc(5*Game.Board.Nb_Tracks*sizeof(int));
  Game.which_player = getMap(Game.Board.Tracks, Game.faceup, cards);
  Game.Player_nb = Game.which_player;
  printf("Player Number = %d NbTracks=%d NbCities=%d\n", Game.Player_nb, Game.Board.Nb_Tracks, Game.Board.Nb_Cities);


  for(int i=0;i<10;i++){
    Game.players[Game.Player_nb].cards_in_hand[i] = 0;
    printf("%d ",Game.players[Game.Player_nb].cards_in_hand[i]);
  }
  printf("\n");
  for(int i = 0 ; i<4 ; i++){
    Game.players[Game.Player_nb].cards_in_hand[cards[i]]++;
    /*
      It deserves a small explanation :
      The card array is filled with cards colors which goes from 1 to 9,
      we want to increment each value corresponding to the card color we got.
    */
  }

  free(gameName);
  return Game;
}

t_Tracksncities** Create_cities_array(t_Game game){
  t_Tracksncities ** T = (t_Tracksncities**) calloc(game.Board.Nb_Cities, sizeof(t_Tracksncities*));
  char counter = 0;
  int First_city, Scnd_city;
  t_color first_color;

  for(int i = 0; i<game.Board.Nb_Cities; i++){
    T[i]= (t_Tracksncities*) calloc(game.Board.Nb_Cities, sizeof(t_Tracksncities));
  }

  for(int i=0;i<game.Board.Nb_Tracks*5;i++){
    switch(counter){
      case 0:
        First_city = game.Board.Tracks[i];
        counter++;
        break;
      case 1:
        Scnd_city = game.Board.Tracks[i];
        counter++;
        break;
      case 2:
        T[First_city][Scnd_city].length = game.Board.Tracks[i];
        T[Scnd_city][First_city].length = game.Board.Tracks[i];
        counter++;
        break;
      case 3:
        first_color = game.Board.Tracks[i];
        counter++;
        break;
      case 4:
        if(game.Board.Tracks[i] == NONE){
          T[First_city][Scnd_city].Track_color[0] = 0;
          T[Scnd_city][First_city].Track_color[0] = 0;
          T[First_city][Scnd_city].Track_color[1] = first_color;
          T[Scnd_city][First_city].Track_color[1] = first_color;
        }else{
          T[First_city][Scnd_city].Track_color[0] = 1;
          T[Scnd_city][First_city].Track_color[0] = 1;
          T[First_city][Scnd_city].Track_color[1] = first_color;
          T[Scnd_city][First_city].Track_color[1] = first_color;
          T[First_city][Scnd_city].Track_color[2] = game.Board.Tracks[i];
          T[Scnd_city][First_city].Track_color[2] = game.Board.Tracks[i];
        }
        counter = 0;
        break;
      default:
        printf("Error while filling Track array\n");
        exit(EXIT_FAILURE);
    }
  }

  show_T_array(game, T);
  return T;
}

void show_T_array(t_Game game, t_Tracksncities** T){
  printf("| ");
  for(int i = 0;i<game.Board.Nb_Cities;i++){
    for(int j = 0;j<game.Board.Nb_Cities;j++){
      printf("%d | ",T[i][j].length);
    }
    printf("\n----------------------------------------------------------------------");
    printf("---------------------------------------------------------------------------\n| ");
  }
}
