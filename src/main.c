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
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;

GLuint
program_create(
  const char * source_vertex,
  const char * source_fragment
){
  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &source_vertex, NULL);
  glCompileShader(vertex);

  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &source_fragment, NULL);
  glCompileShader(fragment);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return program;
}


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

  GLuint shader_program = program_create(vertex_shader, fragment_shader);



  while (window_is_open()) {
    events_process();
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    draw_ui();
    swapBuffers(window);
  }
  printf("%s\n", "Outside event loop..");

}
