#ifndef SHADER_H
#define SHADER_H

#include <GLES2/gl2.h>

typedef struct {
    GLuint vert;
    GLuint frag;
    GLuint prog;

    struct {
        GLuint vPosition;
    } attrib;

    struct {
        
    } uniform;
} program_type;

program_type create_shader_program(const GLchar *vertsrc, const GLchar *fragsrc);
program_type create_shader_program_from_file(char *vspath, char *fspath);

#endif
