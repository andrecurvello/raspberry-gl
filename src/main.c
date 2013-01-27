#include <stdio.h>
#include <assert.h>

#include <bcm_host.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "egl.h"

typedef struct {
    GLuint vshader;
    GLuint fshader;
    GLuint program;
} program_type;

program_type create_shader_program(char *vshader, char *fshader);
program_type create_shader_program_from_file(char *vspath, char *fspath);

int main() {
    bcm_host_init();

    egl_values_type egl = create_egl_context();

    glClearColor(0.5,0.5,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(egl.display, egl.surface);

    for(;;);

    bcm_host_deinit();
    return 0;
}

