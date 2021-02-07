#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <lib/graphics.h>


EGLDisplay * display;
EGLSurface surface;
Window window;
Display * x11_display;


const char *
eglStrError(void)
{
  int error = eglGetError();
  switch(error) {
    case EGL_BAD_MATCH:
      return "EGL_BAD_MATCH";
      break;
    case EGL_BAD_CONFIG:
      return "EGL_BAD_CONFIG";
      break;
    case EGL_BAD_ALLOC:
      return "EGL_BAD_ALLOC";
      break;
    case EGL_BAD_NATIVE_WINDOW:
      return "EGL_BAD_NATIVE_WINDOW";
      break;
  }
  printf("Got: %d\n", error);
  return "Unknown";
}

void
swapBuffers(EGLSurface * surface) {
  eglSwapBuffers(display, *surface);
}


EGLSurface *
init_graphics()
{

  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (display == NULL) {
    printf("%s\n", "Could not open display, aborting.");
    exit(EXIT_FAILURE);
  }

  eglInitialize(display, NULL, NULL);
  eglBindAPI(EGL_OPENGL_API);

  EGLint attribs[] = {
    EGL_BUFFER_SIZE, 32,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
  };

  EGLConfig configs[5] = {0};
  EGLint num_configs = 0;
  if (!eglChooseConfig(display, attribs, configs, 5, &num_configs)) {
    printf("%s\n", "Failed to retrieve config, aborting\n");
    exit(EXIT_FAILURE);
  }

  const int attrib_config[] = {
    EGL_CONTEXT_MAJOR_VERSION, 4,
    EGL_CONTEXT_MINOR_VERSION, 2,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
    EGL_NONE
  };

  EGLContext context = eglCreateContext(display, configs[0], EGL_NO_CONTEXT, attrib_config);

  if (context == NULL) {
    printf("%s\n", "No context was returned, aborting.");
    exit(EXIT_FAILURE);
  }

  x11_display = XOpenDisplay(NULL);
  if (x11_display == NULL) {
    printf("%s\n", "Could not open x11-display, aborting.");
    exit(EXIT_FAILURE);
  }

  window = XCreateSimpleWindow(
      XOpenDisplay(NULL)
      ,DefaultRootWindow(x11_display)
      ,0
      ,0
      ,800
      ,600
      ,0
      ,0
      ,0
  );
  XMapWindow(x11_display, window);

  surface = eglCreateWindowSurface(display, configs[0], window, NULL);
  if (surface == EGL_NO_SURFACE) {
    printf("Error on surface creation: %s\n", eglStrError());
    exit(EXIT_FAILURE);
  }

  eglMakeCurrent(display, surface, surface, context);

  printf(
    "Running the following OpenGL context: %s\n"
    ,glGetString(GL_VERSION)
  );

  return &surface;
}
