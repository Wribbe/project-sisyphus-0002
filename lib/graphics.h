#include <stdbool.h>

#include <X11/Xlib.h>

#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/gl.h>
#include <GL/glext.h>


void
swapBuffers(Window * window);

Window *
init_graphics();

void
events_process();

bool
window_is_open();
