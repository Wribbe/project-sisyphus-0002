#include <stdlib.h>
#include <stdio.h>

#include <EGL/egl.h>
#include <GL/gl.h>

#define STRBOOL(x) x ? "True" : "False"



int
main(int argc, char ** argv)
{

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (display == EGL_NO_DISPLAY) {
    printf("%s\n", "No display was returned, aborting.");
    return EXIT_FAILURE;
  }

  int egl_major = 0, egl_minor = 0;
  if (!eglInitialize(display, &egl_major, &egl_minor)) {
    printf("%s\n", "Could not initialize egl, aborting.");
    return EXIT_FAILURE;
  }

  EGLBoolean success = EGL_FALSE;

  printf("Successfully initialized EGL %d.%d.\n", egl_major, egl_minor);

  EGLint num_configs_total = 0;
  success = eglGetConfigs(display, NULL, 0, &num_configs_total);
  if (!success) {
    printf("%s\n", "Failed to retrieve total number of configurations.");
    return EXIT_FAILURE;
  }

  printf("There are %d available configurations.\n", num_configs_total);

  EGLint num_configs = 0;
  EGLConfig configs[num_configs_total];
  success = eglGetConfigs(display, configs, num_configs_total, &num_configs);
  if (!success) {
    printf("%s\n", "Failed to retrieve configurations, aborting.");
    return EXIT_FAILURE;
  }

  printf("Successfully queried %d configurations, beginning enumeration.\n", num_configs);

  struct data_config {
    EGLint id;
    EGLint type_render;
    EGLint bind_to_texture_rgba;
    EGLint size_red;
    EGLint size_green;
    EGLint size_blue;
    EGLint size_buffer;
    EGLint size_alpha;
    EGLint size_depth;
    EGLint time_min_swap;
    EGLint time_max_swap;
    EGLint num_samples;
  };

  struct data_config data_config = {0};

  for (int ii=0; ii<num_configs; ii++) {

    EGLConfig config = configs[ii];

    #define get_conf(v,t) eglGetConfigAttrib(display, config,v , &data_config.t)

    eglGetConfigAttrib(display, config, EGL_CONFIG_ID, &data_config.id);
    eglGetConfigAttrib(
      display
      ,config
      ,EGL_RENDERABLE_TYPE
      ,&data_config.type_render
    );
    eglGetConfigAttrib(
      display
      ,config
      ,EGL_BIND_TO_TEXTURE_RGBA
      ,&data_config.bind_to_texture_rgba
    );
    eglGetConfigAttrib(display, config, EGL_RED_SIZE, &data_config.size_red);
    eglGetConfigAttrib(
      display
      ,config
      ,EGL_GREEN_SIZE
      ,&data_config.size_green
    );
    eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &data_config.size_blue);
    get_conf(EGL_BUFFER_SIZE, size_buffer);
    get_conf(EGL_ALPHA_SIZE, size_alpha);
    get_conf(EGL_DEPTH_SIZE, size_depth);
    get_conf(EGL_MIN_SWAP_INTERVAL, time_min_swap);
    get_conf(EGL_MAX_SWAP_INTERVAL, time_max_swap);
    get_conf(EGL_SAMPLES, num_samples);


    if (!(data_config.type_render | EGL_OPENGL_BIT)) {
      continue;
    }

    if (data_config.size_buffer < 32) {
      continue;
    }

    if (!(data_config.size_alpha > 0)) {
      continue;
    }

    if (data_config.size_depth < 24) {
      continue;
    }

    if (data_config.num_samples < 8) {
      continue;
    }

    printf("Processing config (%d) @ %d\n", data_config.id, ii);

    printf(" Supports:");
    if (data_config.type_render | EGL_OPENGL_BIT) {
      printf(" OpenGL");
    }
    if (data_config.type_render | EGL_OPENGL_ES_BIT) {
      printf(" OpenGL ES 1.x");
    }
    if (data_config.type_render | EGL_OPENGL_ES2_BIT) {
      printf(" OpenGL ES 2.x");
    }
    if (data_config.type_render | EGL_OPENGL_ES3_BIT) {
      printf(" OpenGL ES 3.x");
    }
    if (data_config.type_render | EGL_OPENVG_BIT) {
      printf(" OpenVG 1.x");
    }
    printf("\n");

    printf(" Binding RGBA: %s\n", STRBOOL(data_config.bind_to_texture_rgba));
    printf(" Total buffer bits: %d\n", data_config.size_buffer);
    printf(
      " Color buffer depth: %d %d %d.\n",
      data_config.size_red, data_config.size_blue, data_config.size_green
    );
    printf(" Size alpha: %d\n", data_config.size_alpha);
    printf(" Size depth: %d\n", data_config.size_depth);
    printf(" Min swap time: %d\n", data_config.time_min_swap);
    printf(" Max swap time: %d\n", data_config.time_max_swap);
    printf(" Num samples: %d\n", data_config.num_samples);

    printf("\n");
    break;
  }
}
