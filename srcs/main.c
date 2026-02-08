#include "headers/dumb.h"
#include <unistd.h>
int main(void) {
  if (isatty(STDIN_FILENO) == 1) {
    // interactive
    dumbshell();
  } else {
    dumbshell();
  }
  return 0;
}
