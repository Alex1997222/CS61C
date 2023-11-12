#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  // allocate memory on heap
  game_state_t* defaultState = (game_state_t*)malloc(sizeof(game_state_t));  

  defaultState->num_rows = 18;
  defaultState->num_snakes = 1;

  //allocate the board
  unsigned int defaultCols = 20;
  defaultState->board = (char**)malloc(defaultState->num_rows*sizeof(char*));
  for(int i = 0; i < defaultState->num_rows; ++i){
      defaultState->board[i] = (char*)malloc((defaultCols+1)*sizeof(char));
  }

  //create board
  char* lineBorader = "####################";
  char* lineArea = "#                  #";
  strcpy(defaultState->board[0],lineBorader);
  for(int i = 1; i < defaultState->num_rows-1; ++i){
      strcpy(defaultState->board[i],lineArea);
  }
  strcpy(defaultState->board[defaultState->num_rows-1],lineBorader);

  //create default snake
  snake_t snake;
  snake.live = true;
  snake.head_row = 2;
  snake.head_col = 4;
  snake.tail_row = 2;
  snake.tail_col = 2;
  defaultState->snakes = (snake_t*)malloc(defaultState->num_snakes*sizeof(snake_t));
  defaultState->snakes[0] = snake;

  //draw snake onboard
  defaultState->board[defaultState->snakes[0].head_row][defaultState->snakes[0].head_col] = 'D';
  defaultState->board[defaultState->snakes[0].tail_row][defaultState->snakes[0].tail_col] = 'd';
  defaultState->board[defaultState->snakes[0].head_row][defaultState->snakes[0].head_col-defaultState->snakes[0].tail_col+1] = '>';
  defaultState->board[2][9] = '*';

  //return the result
  return defaultState;
}

/* Task 2 */
void free_state(game_state_t* state) {
  free(state->snakes);
  for(int i = 0; i < state->num_rows;++i){
      free(state->board[i]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  if(state == NULL || fp == NULL){
     fprintf(stderr, "print_board: NULL argument provided\n");
     return;
  }
  for(int i = 0; i < state->num_rows; ++i){
     fprintf(fp, "%s\n", state->board[i]);
  }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
   return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
   return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
   return (is_tail(c) || is_head(c) || c == '^' || c == '<' || c == 'v' || c == '>');
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch (c) {
        case '^': return 'w';
        case '<': return 'a';
        case 'v': return 's';
        case '>': return 'd';
        default: return '?'; // Return '?' if the character is not part of the snake's body
    }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  switch (c) {
        case 'W': return '^';
        case 'A': return '<';
        case 'S': return 'v';
        case 'D': return '>';
        default: return '?'; // Return '?' if the character is not part of the snake's head
    }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  switch (c) {
        case 'v':
        case 's':
        case 'S':
            return cur_row + 1;
        case '^':
        case 'w':
        case 'W':
            return cur_row - 1;
        default:
            return cur_row;
    }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  switch (c) {
        case '>':
        case 'd':
        case 'D':
            return cur_col + 1;
        case '<':
        case 'a':
        case 'A':
            return cur_col - 1;
        default:
            return cur_col;
    }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
    if (state == NULL) {
        return '?'; // Error handling: return '?' if state is NULL
    }
    unsigned int chrow = state->snakes[snum].head_row;
    unsigned int chcol = state->snakes[snum].head_col;

    char head = get_board_at(state,chrow,chcol);

    // Calculate the next row and column based on the direction of the head
    unsigned int next_row = chrow;
    unsigned int next_col = chcol;
    
    switch (head) {
        case 'W': // Moving up
        case 'S': // Moving down
            next_row = get_next_row(chrow,head);
            break;
        case 'A': // Moving left
        case 'D': // Moving right
            next_col = get_next_col(chcol,head);
            break;
        default:
            return '?'; // Return '?' if the head character is unrecognized
    }

    return state->board[next_row][next_col];
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
    if (state == NULL) {
        return; // Error handling: return '?' if state is NULL
    }
    unsigned int chrow = state->snakes[snum].head_row;
    unsigned int chcol = state->snakes[snum].head_col;

    char head = get_board_at(state,chrow,chcol);

    // Calculate the next row and column based on the direction of the head
    unsigned int next_row = chrow;
    unsigned int next_col = chcol;
    
    switch (head) {
        case 'W': // Moving up
        case 'S': // Moving down
            next_row = get_next_row(chrow,head);
            break;
        case 'A': // Moving left
        case 'D': // Moving right
            next_col = get_next_col(chcol,head);
            break;
        default:
            return;
    }
    // Update the board
    set_board_at(state,next_row,next_col,head);
    set_board_at(state,chrow,chcol,head_to_body(head));

    // Update the snake
    state->snakes[snum].head_row = next_row;
    state->snakes[snum].head_col = next_col;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  if (state == NULL) {
        return; // Error handling: return '?' if state is NULL
    }
    unsigned int chrow = state->snakes[snum].tail_row;
    unsigned int chcol = state->snakes[snum].tail_col;

    char tail = get_board_at(state,chrow,chcol);

    // Calculate the next row and column based on the direction of the head
    unsigned int next_row = chrow;
    unsigned int next_col = chcol;
    
    switch (tail) {
        case 'w': // Moving up
        case 's': // Moving down
            next_row = get_next_row(chrow,tail);
            break;
        case 'a': // Moving left
        case 'd': // Moving right
            next_col = get_next_col(chcol,tail);
            break;
        default:
            return;
    }
    // Update the board
    char nextBody = get_board_at(state,next_row,next_col);
    set_board_at(state,next_row,next_col,body_to_tail(nextBody));
    set_board_at(state,chrow,chcol,' ');

    // Update the snake
    state->snakes[snum].tail_row = next_row;
    state->snakes[snum].tail_col = next_col;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  for(unsigned int i = 0; i < state->num_snakes; ++i){
      // get next square
      char nq = next_square(state,i);
      if(nq == '#' || is_snake(nq)){
         unsigned int chrow = state->snakes[i].head_row;
         unsigned int chcol = state->snakes[i].head_col; 
         set_board_at(state,chrow,chcol,'x');
         state->snakes[i].live = false;
      }else if(nq == '*'){
         update_head(state,i);
         // generate new frut
         add_food(state);
      }else{
         update_head(state,i);
         update_tail(state,i);
      }
  }
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  return NULL;
}
