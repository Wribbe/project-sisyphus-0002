#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <X11/Xlib.h>

#include <GL/glx.h>
#include <GL/gl.h>

int
main(void)
{

  Display * display = XOpenDisplay(NULL);
  if (display == NULL) {
    printf("%s\n", "Could not open display, aborting.");
    return EXIT_FAILURE;
  }


  int screen_default = DefaultScreen(display);

  Window window = XCreateSimpleWindow(
      display
      ,DefaultRootWindow(display)
      ,0
      ,0
      ,800
      ,600
      ,0
      ,BlackPixel(display, screen_default)
      ,WhitePixel(display, screen_default)
  );

  int num_configs = 0;
  GLXFBConfig * configs = glXChooseFBConfig(display, screen_default, 0, &num_configs);
  if (configs == NULL) {
    printf("%s\n", "could not find a config, aborting.\n");
    return EXIT_FAILURE;
  }

  printf("Found %d number of configurations.\n", num_configs);
  XVisualInfo * visual = NULL;
  for (int ii=0; ii<num_configs; ii++) {
    visual = glXGetVisualFromFBConfig(display, configs[ii]);
    if (visual != NULL) {
      printf("Found matching config: %d\n", ii);
      break;
    }
    if (ii+1 == num_configs) {
      printf("%s\n", "Found no matching configuration.");
      return EXIT_FAILURE;
    }
  }

  GLXContext context = glXCreateContext(display, visual, 0, true);
  bool ok_current = glXMakeCurrent(display, window, context);
  if (!ok_current) {
    printf("%s\n", "Setting context did not work, aborting.\n");
    return EXIT_FAILURE;
  }

  printf("Running the following OpenGL context: %s\n", glGetString(GL_VERSION));

  XEvent event = {0};
  XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);

  XMapWindow(display, window);

  for (;;) {
    XNextEvent(display, &event);
    switch (event.type) {

      case Expose:
        printf("%s\n", "Window was exposed!");
        break;

      case KeyPress:
        printf("%s\n", "Key was pressed.");
        break;

      case KeyRelease:
        printf("%s\n", "Key was released.");
        break;

    }
  }

}
