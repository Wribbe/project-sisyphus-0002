#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <X11/Xlib.h>

#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/gl.h>
#include <GL/glext.h>


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
  GLXFBConfig * configs = glXGetFBConfigs(
    display
    ,DefaultScreen(display)
    ,&num_configs
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
    return EXIT_FAILURE;
  }

  XMapWindow(display, window);

  bool ok_current = glXMakeCurrent(display, window, context);
  if (!ok_current) {
    printf("%s\n", "Setting context did not work, aborting.\n");
    return EXIT_FAILURE;
  }

  printf(
    "Running the following OpenGL context: %s\n"
    ,glGetString(GL_VERSION)
  );

  for (;;) {
  }

}
