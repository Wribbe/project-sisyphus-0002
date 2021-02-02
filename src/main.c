#include <stdlib.h>
#include <stdio.h>

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
  GLXFBConfig * config = glXChooseFBConfig(display, screen_default, 0, &num_configs);
  if (config == NULL) {
    printf("%s\n", "could not find a config, aborting.\n");
    return EXIT_FAILURE;
  }

  printf("Found %d number of configurations.\n", num_configs);
  XVisualInfo * info = NULL;
  for (int ii=0; ii<num_configs; ii++) {
    info = glXGetVisualFromFBConfig(display, config[ii]);
    if (info != NULL) {
      printf("Found matching config: %d\n", ii);
      break;
    }
  }


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
