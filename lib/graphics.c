#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lib/graphics.h"

Window window;
Display * display;
bool window_open = true;
GLXContext context = {0};
bool key_is_down[512] = {0};

Atom WM_DELETE_WINDOW;
Atom WM_PROTOCOLS;

bool ui_keyboard = false;

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERIVPROC glGetShaderiv;

void
swapBuffers(Window * window) {
  glXSwapBuffers(display, *window);
}


void
init_methods(void)
{
  glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress(
    (const GLubyte*)"glGenBuffers"
  );

  glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress(
    (const GLubyte*)"glBindBuffer"
  );

  glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress(
    (const GLubyte*)"glBufferData"
  );

  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress(
    (const GLubyte*)"glGenVertexArrays"
  );

  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress(
    (const GLubyte*)"glBindVertexArray"
  );

  glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress(
    (const GLubyte*)"glEnableVertexAttribArray"
  );

  glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress(
    (const GLubyte*)"glVertexAttribPointer"
  );

  glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress(
    (const  GLubyte*)"glCreateShader"
  );

  glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress(
    (const  GLubyte*)"glDeleteShader"
  );

  glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress(
    (const  GLubyte*)"glShaderSource"
  );

  glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress(
    (const  GLubyte*)"glCompileShader"
  );

  glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress(
    (const  GLubyte*)"glCreateProgram"
  );

  glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress(
    (const  GLubyte*)"glAttachShader"
  );

  glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress(
    (const  GLubyte*)"glLinkProgram"
  );

  glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress(
    (const  GLubyte*)"glUseProgram"
  );

  glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress(
    (const  GLubyte*)"glGetUniformLocation"
  );

  glUniform3fv = (PFNGLUNIFORM3FVPROC)glXGetProcAddress(
    (const  GLubyte*)"glUniform3fv"
  );

  glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress(
    (const  GLubyte*)"glGetShaderiv"
  );

  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress(
    (const  GLubyte*)"glGetProgramiv"
  );

}


void
draw_ui_keyboard(GLuint program, GLuint vao)
{
  if (!ui_keyboard) {
    return;
  }

  GLuint loc_color_ui = glGetUniformLocation(program, "u_color_ui");
  float color_blue[] = {0.0f, 1.0f, 0.0f};
  glUniform3fv(loc_color_ui, 1, color_blue);

  glBindBuffer(GL_ARRAY_BUFFER, vao);
  glUseProgram(program);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  for (int i=0; i<512; i++) {
    if (key_is_down[i]) {
      printf("Key: %d is down\n", i);
    }
  }
}


void
draw_ui(GLuint program, GLuint vao)
{
  draw_ui_keyboard(program, vao);
}


void
react_to_events(void)
{

  if (key_is_down[76]) { // F10
    ui_keyboard = !ui_keyboard;
    printf("Ui-keyboard is %s\n", ui_keyboard ? "ON" : "OFF");
    key_is_down[76] = false;
  }

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

  init_methods();

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
