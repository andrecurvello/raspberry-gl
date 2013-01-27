#include <stdio.h>
#include <assert.h>

#include <bcm_host.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "egl.h"

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

int main() {
    bcm_host_init();

    egl_values_type egl = create_egl_context();

    program_type shader = create_shader_program_from_file("res/shader.vert", "res/shader.frag");

    glClearColor(0.5,0.5,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(egl.display, egl.surface);

    for(;;);

    bcm_host_deinit();
    return 0;
}

program_type create_shader_program_from_file(char *vertpath, char *fragpath) {
    GLchar *vertsrc = read_shader_file(vertpath);
    GLchar *fragsrc = read_shader_file(fragpath);
    program_type shader = create_shader_program(vertsrc, fragsrc);
    free(vertsrc);
    free(fragsrc);
    return shader;
}

program_type create_shader_program(const GLchar *vertsrc, const GLchar *fragsrc) {
    program_type shader;
    GLint testval;

    /* Build the program */
    char infolog[1056];
    shader.vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader.vert, 1, &vertsrc, 0);
    glCompileShader(shader.vert);
    CHECK_GL();

    shader.frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader.frag, 1, &fragsrc, 0);
    glCompileShader(shader.frag);
    CHECK_GL();

    shader.prog = glCreateProgram();
    glAttachShader(shader.prog, shader.vert);
    glAttachShader(shader.prog, shader.frag);
    glLinkProgram(shader.prog);
    CHECK_GL();

    shader.attrib.vPosition = glGetAttribLocation(shader.prog, "vPosition");
    CHECK_GL();

    return shader;

}
