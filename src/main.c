#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <X11/Xlib.h>

#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/gl.h>
#include <GL/glext.h>

//typedef GLXContext (*glXCreateContextAttribsARBProc)
//    (Display*, GLXFBConfig, GLXContext, Bool, const int*);

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

//  int num_configs = 0;
//  GLXFBConfig * configs = glXChooseFBConfig(display, screen_default, 0, &num_configs);
//  if (configs == NULL) {
//    printf("%s\n", "could not find a config, aborting.\n");
//    return EXIT_FAILURE;
//  }
//
//  printf("Found %d number of configurations.\n", num_configs);
//  XVisualInfo * visual = NULL;
//  for (int ii=0; ii<num_configs; ii++) {
//    visual = glXGetVisualFromFBConfig(display, configs[ii]);
//    if (visual != NULL) {
//      printf("Found matching config: %d\n", ii);
//      break;
//    }
//    if (ii+1 == num_configs) {
//      printf("%s\n", "Found no matching configuration.");
//      return EXIT_FAILURE;
//    }
//  }

  int num_configs = 0;
  GLXFBConfig * configs = glXGetFBConfigs(display, 0, &num_configs);

  struct gl_values {
    int id_conf;
    int buffer_size;
    int samples;
    int type_drawable;
    int alpha_size;
    int red_size;
    int green_size;
    int blue_size;
    int double_buffer;
    int renderable;
  };

  struct gl_values values = {0};

  for (int ii=0; ii<num_configs; ii++) {

    GLXFBConfig config = configs[ii];

    glXGetFBConfigAttrib(display, config, GLX_FBCONFIG_ID, &values.id_conf);
    glXGetFBConfigAttrib(display, config, GLX_BUFFER_SIZE, &values.buffer_size);
    glXGetFBConfigAttrib(display, config, GLX_SAMPLES, &values.samples);
    glXGetFBConfigAttrib(display, config, GLX_DRAWABLE_TYPE, &values.type_drawable);
    glXGetFBConfigAttrib(display, config, GLX_ALPHA_SIZE, &values.alpha_size);
    glXGetFBConfigAttrib(display, config, GLX_RED_SIZE, &values.red_size);
    glXGetFBConfigAttrib(display, config, GLX_BLUE_SIZE, &values.blue_size);
    glXGetFBConfigAttrib(display, config, GLX_GREEN_SIZE, &values.green_size);
    glXGetFBConfigAttrib(display, config, GLX_DOUBLEBUFFER, &values.double_buffer);
    glXGetFBConfigAttrib(display, config, GLX_X_RENDERABLE, &values.renderable);

//    if (!values.double_buffer) {
//      continue;
//    }
//
//    if (values.buffer_size < 32) {
//      continue;
//    }
//
//    if (values.samples == 0) {
//      continue;
//    }
//
//    if (!(values.type_drawable | GLX_WINDOW_BIT)) {
//      continue;
//    }
//
//    if (!values.renderable) {
//      continue;
//    }

    printf("Processing config %d (ID: %d)\n", ii, values.id_conf);

    printf("  buffer_size: %d\n", values.buffer_size);
    printf("  samples: %d\n", values.samples);
//    printf("  type_drawable includes window: %s\n", values.type_drawable | GLX_WINDOW_BIT ? "True" : "False");
    printf("  color_sizes: %d, %d, %d\n", values.red_size, values.blue_size, values.green_size);
    printf("  alpha_size: %d\n", values.alpha_size);
//    printf("  double buffer: %s\n", values.double_buffer ? "True" : "False");
//    printf("  renderable: %s\n", values.renderable ? "True" : "False");
    printf("\n");

  }

//  int attribs[] = {
//    GLX_BUFFER_SIZE, 32,
//    GLX_ALPHA_SIZE, 8,
//    GLX_SAMPLES, 8,
//    GLX_DOUBLEBUFFER, true,
//    GLX_RENDER_TYPE, GLX_RGBA_BIT,
//    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
//    None
//  };

  static int attribs[] = {
      GLX_RENDER_TYPE, GLX_RGBA_BIT,
      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_DOUBLEBUFFER, true,
      GLX_RED_SIZE, 1,
      GLX_GREEN_SIZE, 1,
      GLX_BLUE_SIZE, 1,
      None
  };

  GLXFBConfig * configs_choosen = glXChooseFBConfig(
    display
    ,DefaultScreen(display)
    ,attribs
    ,&num_configs
  );

  printf("Got %d configurations back.\n", num_configs);
  for (int ii=0; ii<num_configs; ii++) {
    int id_conf = 0;
    glXGetFBConfigAttrib(display, configs_choosen[ii], GLX_FBCONFIG_ID, &id_conf);
    printf("  %d\n", id_conf);
  }

//  printf("%s\n", "Picking first config of the lot.");
//
//  XVisualInfo * visual = glXGetVisualFromFBConfig(display, configs_choosen[0]);
//
//  if (visual == NULL) {
//    printf("%s\n", "There was not matching visual, aborting.");
//    return EXIT_FAILURE;
//  }

  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
  glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress(
      (const GLubyte*)"glXCreateContextAttribsARB"
  );

  const int attrib_config[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
    GLX_CONTEXT_MINOR_VERSION_ARB, 2,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };

//  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
//  glXCreateContextAttribsARB =
//      (glXCreateContextAttribsARBProc)
//      glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

//  GLXContext context = glXCreateContext(display, visual, 0, true);
//  GLXContext context = glXCreateContextAttribARB(display, configs_choosen[0], 0, true, attrib_config);
  GLXContext context = glXCreateContextAttribsARB(display, configs_choosen[0], 0, true, attrib_config);

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

  printf("Running the following OpenGL context: %s\n", glGetString(GL_VERSION));

//  XEvent event = {0};
//  XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);


  for (;;) {
//    XNextEvent(display, &event);
//    switch (event.type) {
//
//      case Expose:
//        printf("%s\n", "Window was exposed!");
//        break;
//
//      case KeyPress:
//        printf("%s\n", "Key was pressed.");
//        break;
//
//      case KeyRelease:
//        printf("%s\n", "Key was released.");
//        break;
//
//    }
  }

}
