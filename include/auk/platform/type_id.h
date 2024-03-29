#pragma once

#include <functional>

namespace auk {
struct Type {
private:
    size_t id;
    static inline size_t type_count = 0;

    Type(size_t i) { id = i; }

public:
    Type() = default;

    bool operator==(const Type& other) { return id == other.id; }
    bool operator!=(const Type& other) { return id != other.id; }
    operator size_t() const { return id; }

    static size_t count() { return type_count; }

    template <typename T>
    static Type of() {
        static const size_t _id = type_count++;
        return Type(_id);
    }
};
}  // namespace auk

namespace std {
template <>
struct hash<auk::Type> {
    std::size_t operator()(const auk::Type& t) const { return t; }
};

}  // namespace std
