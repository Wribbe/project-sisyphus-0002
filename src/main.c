/* gcc this.c -lGL -lX11 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

/*
    License: Public domain

    Contents
    --------
    - Create_display_and_window
    - Create_the_modern_OpenGL_context
    - Show_the_window
    - Application_loop
*/

typedef GLXContext (*glXCreateContextAttribsARBProc)
    (Display*, GLXFBConfig, GLXContext, Bool, const int*);

int main()
{
    Display* disp = 0;
    Window win = 0;

    /* Create_display_and_window
       -------------------------
       Skip if you already have a display and window */
    disp = XOpenDisplay(0);
    win = XCreateSimpleWindow(disp, DefaultRootWindow(disp),
                              10, 10,   /* x, y */
                              800, 600, /* width, height */
                              0, 0,     /* border_width, border */
                              0);       /* background */

    /* Create_the_modern_OpenGL_context
       -------------------------------- */
    static int visual_attribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
    };

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(disp,
                                         DefaultScreen(disp),
                                         visual_attribs, &num_fbc);
    if (!fbc) {
        printf("glXChooseFBConfig() failed\n");
        exit(1);
    }

    /* If we were on Windows (i.e. WGL), we would need to create an old
       dummy OpenGL context here, before calling GetProcAddress(). This is
       unnecessary on Linux (GLX).

       For details, refer to the spec
       (https://www.khronos.org/registry/OpenGL/extensions/ARB/GLX_ARB_get_proc_address.txt)
       which says:
           > Are function pointers context-independent? Yes. The pointer to an
           > extension function can be used with any context [...]

       This is in direct contrast to WGL's wglGetProcAddress. */

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB =
        (glXCreateContextAttribsARBProc)
        glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    /* If we were on Winows, we would destroy the dummy context here. Again,
       this is unnecessary on Linux.*/

    if (!glXCreateContextAttribsARB) {
        printf("glXCreateContextAttribsARB() not found\n");
        exit(1);
    }

    /* Set desired minimum OpenGL version */
    static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 2,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };
    /* Create modern OpenGL context */
    GLXContext ctx = glXCreateContextAttribsARB(disp, fbc[0], NULL, true,
                                                context_attribs);
    if (!ctx) {
        printf("Failed to create OpenGL context. Exiting.\n");
        exit(1);
    }

    /* Show_the_window
       --------------- */
    XMapWindow(disp, win);
    glXMakeCurrent(disp, win, ctx);

    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OpenGL context created.\nVersion %d.%d\nVendor %s\nRenderer %s\n",
           major, minor,
           glGetString(GL_VENDOR),
           glGetString(GL_RENDERER));

    printf("Version: %s\n", glGetString(GL_VERSION));

    /* Application_loop
       ---------------- */
    while(1) {}

    return 0;
}
