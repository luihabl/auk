#pragma once

#include <vector>

namespace auk {

enum class BufferUsage { Static, Dynamic };
enum class DrawMode { Triangles, Lines, Points };

class Renderer {
public:
    struct VertexAttribute {
        enum class Type { Float, Double, UnsignedByte };

        int n_elements;
        Type attrib_type;
        bool normalized;
        int offset;
    };

    struct VertexInfo {
        int size;
        std::vector<VertexAttribute> attributes;
    };

    Renderer() = default;
    explicit Renderer(VertexInfo&& vertex_info, BufferUsage usage);

    virtual ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&& other) noexcept;

    void upload_indices(unsigned* indices, unsigned n_indices);
    void upload_vertices(void* vertices, unsigned n_vertices);

    void draw_arrays(DrawMode mode, unsigned count, unsigned offset, unsigned instances = 1);
    void draw_elements(DrawMode mode,
                       unsigned index_count,
                       unsigned index_offset,
                       unsigned instances = 1);

    unsigned int uploaded_indices() const { return uploaded_indices_; }
    unsigned int uploaded_vertices() const { return uploaded_vertices_; }

private:
    bool initialized_ = false;
    unsigned int vao_id_ = 0;
    unsigned int vbo_id_ = 0;
    unsigned int ebo_id_ = 0;

    unsigned int uploaded_vertices_ = 0;
    unsigned int uploaded_indices_ = 0;

    BufferUsage usage_ = BufferUsage::Dynamic;
    VertexInfo vertex_info_;
};

}  // namespace auk
