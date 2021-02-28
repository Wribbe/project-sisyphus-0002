#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lib/graphics.h"

GLfloat vertices[] = {
  0.0f, 0.5f, 0.0f,
  0.5f,-0.5f, 0.0f,
 -0.5f,-0.5f, 0.0f
};

GLfloat vertices_square[] = {
  0.5f, 0.5f, 0.0f,
 -0.5f, 0.5f, 0.0f,
 -0.5f,-0.5f, 0.0f,
 -0.5f,-0.5f, 0.0f,
  0.5f,-0.5f, 0.0f,
  0.5f, 0.5f, 0.0f
};




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


const char * src_ui_vertex =
"#version 400\n"
"uniform vec3 u_scale;\n"
"uniform vec2 u_offset;\n"
"in vec3 vp;\n"
"void main() {\n"
"  gl_Position = vec4((u_scale*vp)+vec3(u_offset, 1.0f), 1.0);\n"
"}\n";


const char * src_ui_fragment =
"#version 400\n"
"uniform vec3 u_color_ui;\n"
"out vec4 frag_color;\n"
"void main() {\n"
"  frag_color = vec4(u_color_ui, 1.0f);\n"
"}\n";


GLuint
program_create(
  const char * source_vertex,
  const char * source_fragment
){
  int success = 0;
  size_t size_buffer = 512;
  char buffer[size_buffer];

  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &source_vertex, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, size_buffer, NULL, buffer);
    printf("Vertex shader failed to compile:\n%s.", buffer);
  }

  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &source_fragment, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, size_buffer, NULL, buffer);
    printf("Fragment shader failed to compile:\n%s.", buffer);
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    printf("%s\n", "Failed to link program.");
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return program;
}


int
main(void)
{
  Window * window = init_graphics();

  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint program_shader = program_create(vertex_shader, fragment_shader);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  GLuint vbo_ui = 0;
  glGenBuffers(1, &vbo_ui);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_ui);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_square), vertices_square, GL_STATIC_DRAW);

  GLuint vao_ui = 0;
  glGenVertexArrays(1, &vao_ui);
  glBindVertexArray(vao_ui);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  GLuint program_ui = program_create(src_ui_vertex, src_ui_fragment);
  glBindVertexArray(0);


  while (window_is_open()) {
    events_process();
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glUseProgram(program_shader);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    draw_ui(program_ui, vao_ui);

    swapBuffers(window);
  }
  printf("%s\n", "Outside event loop..");

}
