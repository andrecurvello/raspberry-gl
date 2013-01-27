#include <stdio.h>
#include <assert.h>

#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "egl.h"
#include "shader.h"

struct {
    GLuint vPosition;
} attributes;

int main() {
    bcm_host_init();

    egl_values_type egl = create_egl_context();

    program_type shader = create_shader_program_from_file("res/shader.vert", "res/shader.frag");

    attributes.vPosition = glGetAttribLocation(shader.prog, "vPosition");
    CHECK_GL();

    glClearColor(0.5,0.5,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(egl.display, egl.surface);

    for(;;);

    bcm_host_deinit();
    return 0;
}

