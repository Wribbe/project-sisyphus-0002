#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <lib/graphics.h>

Window window;
Display * display;
bool window_open = true;
GLXContext context = {0};
bool key_is_down[512] = {0};

Atom WM_DELETE_WINDOW;
Atom WM_PROTOCOLS;

bool ui_keyboard = false;


void
swapBuffers(Window * window) {
  glXSwapBuffers(display, *window);
}


void
ui_draw_keyboard(void)
{
  if (!ui_keyboard) {
    return;
  }

  for (int i=0; i<512; i++) {
    if (key_is_down[i]) {
      printf("Key: %d is down\n", i);
    }
  }
}


void
react_to_events(void)
{

  if (key_is_down[76]) { // F10
    ui_keyboard = !ui_keyboard;
    printf("Ui-keyboard is %s\n", ui_keyboard ? "ON" : "OFF");
    key_is_down[76] = false;
  }

  ui_draw_keyboard();
}


void
events_process()
{

  XEvent event = {0};
  XClientMessageEvent * message = NULL;
  XKeyEvent * event_key = NULL;

  while (XPending(display)) {

    XFlush(display);
    XNextEvent(display, &event);

    switch(event.type) {
      case DestroyNotify:
        printf("%s\n", "Window got destroyed.");
        break;
      case ClientMessage:
        message = (XClientMessageEvent*)&event;
        if (message->message_type == WM_PROTOCOLS) {
          if (message->data.l[0] == WM_DELETE_WINDOW) {
            window_open = false;
            break;
          }
        }
      case KeyPress:
      case KeyRelease:
        event_key = (XKeyEvent *)&event;
        if (XPending(display)) {
          XEvent event_next = {0};
          XPeekEvent(display, &event_next);
          if (((XKeyEvent *)&event_next)->time == event_key->time) {
            /* Skip repeated events. */
            XNextEvent(display, &event_next);
            continue;
          }
        }
        key_is_down[event_key->keycode] = event.type == KeyPress;
        break;
      default:
        printf("Unknown %d\n", event.type);
        break;
    }
  }
  react_to_events();
}


bool
window_is_open() {
  if (!window_open) {
    glXDestroyContext(display, context);
    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    glFlush();
    XFlush(display);
  }
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

  context = glXCreateContextAttribsARB(
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
  WM_PROTOCOLS = XInternAtom(display, "WM_PROTOCOLS", false);

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
