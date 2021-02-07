#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <lib/graphics.h>

Window window;
Display * display;
bool window_open = true;

Atom WM_DELETE_WINDOW;


void
swapBuffers(Window * window) {
  glXSwapBuffers(display, *window);
}



void
events_process()
{
  XEvent event = {0};

  while (XPending(display)) {
    XFlush(display);
    XNextEvent(display, &event);
    switch(event.type) {
      case DestroyNotify:
        printf("%s\n", "Window got destroyed.");
        break;
      case ClientMessage:
        printf("%s\n", "Client Message.");
        XDestroyWindow(display, window);
        window_open = false;
        printf("%s\n", "Window open set to false.");
        break;
      default:
        printf("Unknown %d\n", event.type);
        break;
    }
    printf("Got event of type %d\n", event.type);
  }
  printf("%s\n", "Processing events done.");

}


bool
window_is_open() {
  printf("Window open: %s\n", window_open ? "True" : "False");
  return window_open;
}


Window *
init_graphics()
{

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    printf("%s\n", "Could not open display, aborting.");
    exit(EXIT_FAILURE);
  }

  int screen_default = DefaultScreen(display);

  window = XCreateSimpleWindow(
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

  int attribs[] = {
    GLX_BUFFER_SIZE, 32,
    GLX_ALPHA_SIZE, 8,
    GLX_SAMPLES, 8,
    GLX_DOUBLEBUFFER, true,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    None
  };

  int num_configs = 0;
  GLXFBConfig * configs_choosen = glXChooseFBConfig(
    display
    ,DefaultScreen(display)
    ,attribs
    ,&num_configs
  );

  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
  glXCreateContextAttribsARB =
    (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress(
      (const GLubyte*)"glXCreateContextAttribsARB"
  );

  const int attrib_config[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
    GLX_CONTEXT_MINOR_VERSION_ARB, 2,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };

  GLXContext context = glXCreateContextAttribsARB(
    display
    ,configs_choosen[0]
    ,0
    ,true
    ,attrib_config
  );

  if (context == NULL) {
    printf("%s\n", "No context was returned, aborting.");
    return NULL;
  }

  XMapWindow(display, window);

  WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", false);

  Atom protocols[] = {
    WM_DELETE_WINDOW
  };

  XSetWMProtocols(display, window, protocols, sizeof(protocols)/sizeof(Atom));

  if (!glXMakeCurrent(display, window, context)) {
    printf("%s\n", "Setting context did not work, aborting.\n");
    exit(EXIT_FAILURE);
  }

  printf(
    "Running the following OpenGL context: %s\n"
    ,glGetString(GL_VERSION)
  );

  XSelectInput(display, window, KeyPressMask | KeyReleaseMask | ExposureMask );

  return &window;
}
