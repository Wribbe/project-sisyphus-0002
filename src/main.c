#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lib/graphics.h"

#include <GL/glext.h>

GLfloat vertices[] = {
  0.0f, 0.5f, 0.0f,
  0.5f,-0.5f, 0.0f,
 -0.5f,-0.5f, 0.0f
};

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;


const char * vertex_shader =
"#version 400\n"
"in vec3 vp;\n"
"void main() {\n"
"  gl_Position = vec4(vp, 1.0);\n"
"}\n";


const char * fragment_shader =
"#version 400\n"
"out vec4 frag_color;\n"
"void main() {\n"
"  frag_color = vec4(0.5, 0.0, 0.5, 1.0);\n"
"}\n";


PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;

int
main(void)
{
  Window * window = init_graphics();

  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


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


  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  glUseProgram(shader_program);

  for (;;) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    swapBuffers(window);
  }

}
