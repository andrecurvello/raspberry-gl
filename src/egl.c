#include "egl.h"
#include "util.h"

egl_values_type create_egl_context(void) {
    egl_values_type egl;
    EGLBoolean result;
    EGLint config_count;
    static EGL_DISPMANX_WINDOW_T nativewindow;
    static EGLint attributes[] = {
        EGL_RED_SIZE,   8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE,  8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };
    static EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    /* Get and initialize the EGL display */
    egl.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(egl.display != EGL_NO_DISPLAY);
    CHECK_GL();
    result = eglInitialize(egl.display, NULL, NULL);
    assert(result != EGL_FALSE);
    CHECK_GL();

    /* Get the EGL configurations */
    result = eglGetConfigs(egl.display, NULL, 0, &config_count);
    assert(result != EGL_FALSE);
    EGLConfig *configs = malloc(config_count * sizeof(EGLConfig));
    assert(configs != NULL);
    CHECK_GL();
    result = eglGetConfigs(egl.display, configs, config_count, &config_count);
    assert(result != EGL_FALSE);
    
    /* Configure the EGL attributes */
    EGLConfig config;
    eglGetConfigAttrib(egl.display, configs[0], EGL_RED_SIZE,     &attributes[1]);
    eglGetConfigAttrib(egl.display, configs[0], EGL_GREEN_SIZE,   &attributes[3]);
    eglGetConfigAttrib(egl.display, configs[0], EGL_BLUE_SIZE,    &attributes[5]);
    eglGetConfigAttrib(egl.display, configs[0], EGL_ALPHA_SIZE,   &attributes[7]);
    eglGetConfigAttrib(egl.display, configs[0], EGL_SURFACE_TYPE, &attributes[9]);
    result = eglChooseConfig(egl.display, attributes, &config, 1, &config_count);
    assert(result != EGL_FALSE);
    CHECK_GL();
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(result != EGL_FALSE);
    CHECK_GL();

    /* Create the EGL rendering context */
    egl.context = eglCreateContext(egl.display, config, EGL_NO_CONTEXT, context_attributes);
    assert(egl.context != EGL_NO_CONTEXT);
    CHECK_GL();

    /* Create the EGL window surface */
    int32_t result32t = graphics_get_display_size(
            0, /*LCD*/ &(egl.screen.width), &(egl.screen.height));
    assert(result32t >= 0);

    VC_RECT_T dst_rect, src_rect;
    dst_rect.x = src_rect.x = 0;
    dst_rect.y = src_rect.y = 0;
    dst_rect.width  = egl.screen.width;
    dst_rect.height = egl.screen.height;
    src_rect.width  = egl.screen.width  << 16;
    src_rect.height = egl.screen.height << 16;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    dispman_display = vc_dispmanx_display_open(0 /*LCD*/);
    dispman_update  = vc_dispmanx_update_start(0);
    dispman_element = vc_dispmanx_element_add(
            dispman_update, dispman_display,
            0 /* layer */, &dst_rect, 0 /* src */, &src_rect,
            DISPMANX_PROTECTION_NONE,
            0, /* Alpha */
            0, /* Clamp */
            0 /* Transform*/);

    nativewindow.element = dispman_element;
    nativewindow.width   = egl.screen.width;
    nativewindow.height  = egl.screen.height;
    vc_dispmanx_update_submit_sync(dispman_update);

    CHECK_GL();

    egl.surface = eglCreateWindowSurface(egl.display, config, &nativewindow, NULL);
    assert(egl.surface != EGL_NO_SURFACE);
    CHECK_GL();

    // Connect the context to the surface
    result = eglMakeCurrent(egl.display, egl.surface, egl.surface, egl.context);
    assert(result != EGL_FALSE);
    CHECK_GL();

    return egl;
}
