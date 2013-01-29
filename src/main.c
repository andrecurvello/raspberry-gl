#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <wiringPi.h>

#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "egl.h"
#include "shader.h"

struct {

    egl_values_type egl;

    program_type shader;

    GLuint vertexBuffer;

    struct {
        GLuint vPosition;
    } attr;

    struct {
        GLuint uColor;
    } unif;

} state;


static const GLfloat vertex_data[] = {
    -0.5, -0.5, -1.0,
     0.0,  0.5, -1.0,
     0.5, -0.5, -1.0
};

int main() {
    if (wiringPiSetup() == -1)
        return 1;
    pinMode(3, INPUT);

    bcm_host_init();

    state.egl = create_egl_context();

    state.shader = create_shader_program_from_file("res/shader.vert", "res/shader.frag");

    state.attr.vPosition = glGetAttribLocation(state.shader.prog, "vPosition");
    glEnableVertexAttribArray(state.attr.vPosition);
    state.unif.uColor = glGetUniformLocation(state.shader.prog, "uColor");
    CHECK_GL();


    glClearColor(0.5,0.5,0.5,1.0);

    // Setup viewport
    glViewport(0, 0, state.egl.screen.width, state.egl.screen.height);
    CHECK_GL();

    // Upload vertex data
    glGenBuffers(1, &state.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, state.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();


    for(;;) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(state.shader.prog);

        bool pin = digitalRead(3);
        if (pin)
            glUniform4f(state.unif.uColor, 1.0, 0.0, 0.0, 1.0);
        else
            glUniform4f(state.unif.uColor, 1.0, 1.0, 1.0, 1.0);

        glBindBuffer(GL_ARRAY_BUFFER, state.vertexBuffer);
        glEnableVertexAttribArray(state.attr.vPosition);
        glVertexAttribPointer(state.attr.vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glFlush();
        glFinish();

        eglSwapBuffers(state.egl.display, state.egl.surface);
    }

    bcm_host_deinit();

    return 0;
}

