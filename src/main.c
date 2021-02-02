#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>

int
main(void)
{

  Display * display = XOpenDisplay(NULL);
  if (display == NULL) {
    printf("%s\n", "Could not open display, aborting.");
    return EXIT_FAILURE;
  }

  Window window = XCreateSimpleWindow(
      display
      ,DefaultRootWindow(display)
      ,0
      ,0
      ,800
      ,600
      ,0
      ,BlackPixel(display, DefaultScreen(display))
      ,WhitePixel(display, DefaultScreen(display))
  );

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
