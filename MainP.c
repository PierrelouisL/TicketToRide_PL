#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "API/clientAPI.h"
#include "API/TicketToRideAPI.h"


#if defined(__WIN32)
#define CLEAR(); system("cls");
#else
#define CLEAR(); system("clear");
#endif


typedef struct{
  int Nb_Cities;              /* How many cities there is on the map */
  int Nb_Tracks;              /* How many tracks there is on the map */
  int* Tracks;                /* cf getMap() comment */
} t_Board;

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

t_Game start_game();
t_return_code Opponent_s_turn(t_Game* Game, int* game_over);
t_return_code Bot_s_turn(t_Game* Game, int* game_over);
t_return_code User_s_turn(t_Game* Game, int* game_over);


int main(void){
  connectToServer("li1417-56.members.linode.com", 5678, "BOT_PL");
  t_Game Game = start_game();
  int game_over=0;
  t_return_code end;
  //printMap();
  while(!game_over){
    printMap();
    if(Game.which_player){
      end = Opponent_s_turn(&Game, &game_over);
      //CLEAR();
    }else{
      //end = Bot_s_turn(&Game, &game_over);
      end = User_s_turn(&Game, &game_over);
      //CLEAR();
    }
  }
  if(end == WINNING_MOVE){
    printf("win!\n");
  }
  if(end ==LOOSING_MOVE){
    printf("loose!\n");
  }
  free(Game.Board.Tracks);
  closeConnection();
  return 0;
}


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

t_return_code Opponent_s_turn(t_Game* Game, int* game_over){
  t_move opponent_move;
  t_return_code end;
  int replay = 0;
  end = getMove(&opponent_move, &replay);
  if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
    *game_over = 1;
    return end;
  }
  if(replay){
    end = getMove(&opponent_move, &replay);
    if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
      *game_over = 1;
      return end;
    }
  }
  printf("replay = %d\n", replay);
  Game->which_player = 0;
  return end;
}

t_return_code Bot_s_turn(t_Game* Game, int* game_over){
  t_color card_color;
  t_return_code end;
  end = drawBlindCard(&card_color);
  if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
    *game_over = 1;
    return end;
  }
  end = drawBlindCard(&card_color);
  if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
    *game_over = 1;
    return end;
  }
  Game->which_player = 1;
  return end;
}

t_return_code User_s_turn(t_Game* Game, int* game_over){
  int move;
  t_return_code end;
  int city1, city2, color, nbLoco;
  t_color card;
  t_objective obj[3];
  int object[3] = {1,1,1};

  printf("It's your turn to play! Pick a move: ");
  scanf("%d",&move);
  switch (move) {
    case 1:
      printf("City1 City2 color nbLoco: ");
      scanf("%d %d %d %d",&city1, &city2, &color, &nbLoco);
      printf("%d %d %d %d",city1, city2, color, nbLoco);
      end = claimRoute(city1,city2,color,nbLoco);
      Game->players[Game->Player_nb].available_wagons -= nbLoco;
      Game->players[Game->Player_nb].cards_in_hand[color] -= nbLoco;
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      break;
    case 2:
      printf("drawObjectives\n");
      end = drawObjectives(obj);
      /* We choose to keep every objectives yet */
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      end = chooseObjectives(object);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      for(int i=0; i<3; i++){
        Game->players[Game->Player_nb].objectives[i] = obj[i];
        printf("%dth obj = %d %d %d ",i,obj[i].city1, obj[i].city2, obj[i].score);
      }
      Game->players[Game->Player_nb].Nb_objectives = 3;

      break;
    case 3:
      printf("drawBlindCard\n");
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      end = drawBlindCard(&card);
      if((end == WINNING_MOVE) || (end == LOOSING_MOVE)){
        *game_over = 1;
        return end;
      }
      Game->players[Game->Player_nb].cards_in_hand[card]++;
      for(int i=1;i<10;i++){
        printf("%d ",Game->players[Game->Player_nb].cards_in_hand[i]);
      }
      printf("\n");
      break;
    case 4:
      break;
    default:
      break;
  }
  Game->which_player = 1;
  return end;
}
