#include <glad/glad.h>
#include <string>

#include "tinysdl/graphics/graphics.h"
#include "tinysdl/platform/log.h"
#include "tinysdl/graphics/color.h"

using namespace TinySDL;


void Graphics::load_functions(GraphicsLoaderFunction loader_func) {

    ASSERT_MSG(gladLoadGLLoader((GLADloadproc) loader_func), "Failed to load GL functions");
    Log::info("OpenGL %s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
}


void APIENTRY gl_debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity, [[maybe_unused]] GLsizei length, const char *message, [[maybe_unused]] const void *user_param) {

    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 
    
    std::string source_msg;
    switch (source) {
        case GL_DEBUG_SOURCE_API:              source_msg = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:    source_msg = "Window system"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:  source_msg = "Shader compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:      source_msg = "Third party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:      source_msg = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:            source_msg = "Other"; break;
        default:                               source_msg = "-";
    } 

    std::string type_msg;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:                type_msg = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  type_msg = "Deprecated behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   type_msg = "Undefined behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:          type_msg = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:          type_msg = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:               type_msg = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:           type_msg = "Push group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:            type_msg = "Pop group"; break;
        case GL_DEBUG_TYPE_OTHER:                type_msg = "Other"; break;
        default:                                 type_msg = "-";
    }
    
    std::string severity_msg;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         severity_msg = "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severity_msg = "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          severity_msg = "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severity_msg = "Notification"; break;
        default:                             severity_msg = "-";
    } 

    Log::debug("OpenGL message [%i]: %s\nsource: %s\ntype: %s\nseverity: %s\n", id, message, source_msg.c_str(), type_msg.c_str(), severity_msg.c_str());
}

void Graphics::setup_debug() {
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        Log::debug("OpenGL debug activated");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void Graphics::clear(const Color & c) {
    glClearColor(c.r(), c.g(), c.b(), c.a()); 
    glClear(GL_COLOR_BUFFER_BIT); 
}

void Graphics::viewport(int w, int h) {
    glViewport(0, 0, w, h);
}

void Graphics::viewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void Graphics::set_blend_mode() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}