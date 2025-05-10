
#include "auk/graphics/renderer.h"

#include <glad/glad.h>

namespace {
#define ENUMCOMP(ec, el, glel) \
    if (v == ec::el)           \
        return glel;

#define ENUM_TRANSLATOR(fname, eclass) \
    GLenum fname(eclass v) {           \
        ENUM_TRANSLATIONS(eclass)      \
        return 0;                      \
    }

#define ENUM_TRANSLATIONS(ec)       \
    ENUMCOMP(ec, Double, GL_DOUBLE) \
    ENUMCOMP(ec, Float, GL_FLOAT)   \
    ENUMCOMP(ec, UnsignedByte, GL_UNSIGNED_BYTE)
ENUM_TRANSLATOR(vertex_type, auk::Renderer::VertexAttribute::Type)
#undef ENUM_TRANSLATIONS

#define ENUM_TRANSLATIONS(ec)            \
    ENUMCOMP(ec, Static, GL_STATIC_DRAW) \
    ENUMCOMP(ec, Dynamic, GL_DYNAMIC_DRAW)
ENUM_TRANSLATOR(buffer_usage_type, auk::BufferUsage)
#undef ENUM_TRANSLATIONS

#define ENUM_TRANSLATIONS(ec)             \
    ENUMCOMP(ec, Triangles, GL_TRIANGLES) \
    ENUMCOMP(ec, Lines, GL_LINES)         \
    ENUMCOMP(ec, Points, GL_POINTS)
ENUM_TRANSLATOR(draw_mode_type, auk::DrawMode)
#undef ENUM_TRANSLATIONS

}  // namespace

namespace auk {

Renderer::Renderer(VertexInfo&& vertex_info, BufferUsage usage)
    : vertex_info_(vertex_info), usage_(usage) {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    glGenBuffers(1, &vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);

    // Add vertex attributes
    for (int i = 0; i < vertex_info_.attributes.size(); ++i) {
        const auto& attr = vertex_info_.attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attr.n_elements, vertex_type(attr.attrib_type), attr.normalized,
                              vertex_info_.size, reinterpret_cast<void*>(attr.offset));
    }

    // Create EBO
    glGenBuffers(1, &ebo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    initialized_ = true;
}

Renderer::~Renderer() {
    if (initialized_) {
        glDeleteVertexArrays(1, &vao_id_);
        glDeleteBuffers(1, &vbo_id_);
        glDeleteBuffers(1, &ebo_id_);
    }
}
Renderer::Renderer(Renderer&& other) noexcept {
    if (this != &other) {
        initialized_ = other.initialized_;
        vao_id_ = other.vao_id_;
        vbo_id_ = other.vbo_id_;
        ebo_id_ = other.ebo_id_;
        usage_ = other.usage_;
        uploaded_indices_ = other.uploaded_indices_;
        uploaded_vertices_ = other.uploaded_vertices_;
        vertex_info_ = std::move(other.vertex_info_);
        other.initialized_ = false;
    }
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        initialized_ = other.initialized_;
        vao_id_ = other.vao_id_;
        vbo_id_ = other.vbo_id_;
        ebo_id_ = other.ebo_id_;
        usage_ = other.usage_;
        uploaded_indices_ = other.uploaded_indices_;
        uploaded_vertices_ = other.uploaded_vertices_;
        vertex_info_ = std::move(other.vertex_info_);
        other.initialized_ = false;
    }

    return *this;
}

void Renderer::upload_indices(unsigned* indices, unsigned n_indices) {
    glBindVertexArray(vao_id_);

    // Uploading indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * n_indices, indices,
                 buffer_usage_type(usage_));

    glBindVertexArray(0);

    uploaded_indices_ = n_indices;
}
void Renderer::upload_vertices(void* vertices, unsigned n_vertices) {
    glBindVertexArray(vao_id_);

    // Uploading vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, vertex_info_.size * n_vertices, vertices,
                 buffer_usage_type(usage_));

    glBindVertexArray(0);

    uploaded_vertices_ = n_vertices;
}
void Renderer::draw_arrays(DrawMode mode, unsigned count, unsigned offset, unsigned instances) {
    glBindVertexArray(vao_id_);
    if (instances > 1)
        glDrawArraysInstanced(draw_mode_type(mode), offset, count, instances);
    else
        glDrawArrays(draw_mode_type(mode), offset, count);
    glBindVertexArray(0);
}

void Renderer::draw_elements(DrawMode mode, unsigned count, unsigned offset, unsigned instances) {
    glBindVertexArray(vao_id_);
    if (instances > 1)
        glDrawElementsInstanced(draw_mode_type(mode), (GLsizei)count, GL_UNSIGNED_INT,
                                (void*)(sizeof(unsigned int) * offset), instances);
    else
        glDrawElements(draw_mode_type(mode), (GLsizei)count, GL_UNSIGNED_INT,
                       (void*)(sizeof(unsigned int) * offset));
    glBindVertexArray(0);
}

}  // namespace auk