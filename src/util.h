#ifndef UTIL_H
#define UTIL_H
#include <assert.h>
#include <stdio.h>
#include <GLES2/gl2.h>

#define CHECK_GL() (assert(glGetError() == 0))

GLchar *read_shader_file(char *path);

#endif
