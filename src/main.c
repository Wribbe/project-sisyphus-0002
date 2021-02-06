#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lib/graphics.h"

int
main(void)
{
  Window * window = init_graphics();

  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

  for (;;) {
    glClear(GL_COLOR_BUFFER_BIT);
    swapBuffers(window);
  }

}
