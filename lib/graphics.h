#include <X11/Xlib.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>

void
swapBuffers(EGLSurface * surface);

EGLSurface *
init_graphics();
