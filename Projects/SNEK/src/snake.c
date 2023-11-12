#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char* argv[]) {
  bool io_stdin = false;
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  // Read board from file, or create default board
  if (in_filename != NULL) {
    FILE* file = fopen(in_filename, "r");
    if (file == NULL) {
      fprintf(stderr, "Error opening file %s\n", in_filename);
      return -1;
    }
    state = load_board(file);
    fclose(file);
    initialize_snakes(state);
  } else if (io_stdin) {
    state = load_board(stdin);
    initialize_snakes(state);
  } else {
    state = create_default_state(); // Assuming this function exists
  }

  // Update state (example: moving snakes, adding food)
  update_state(state, deterministic_food); // Assuming deterministic_food is a function

  // Write updated board to file or stdout
  if (out_filename != NULL) {
    save_board(state, out_filename); // Assuming save_board is implemented
  } else {
    print_board(state, stdout); // Assuming print_board is implemented
  }

  // Free the state
  free_state(state);

  return 0;
}
