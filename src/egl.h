#ifndef EGL_H
#define EGL_H

#include <stdint.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <bcm_host.h>
#include "util.h"

typedef struct {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    struct {
        uint32_t width;
        uint32_t height;
    } screen;
} egl_values_type;

egl_values_type create_egl_context(void);

#endif
