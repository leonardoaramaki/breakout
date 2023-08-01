#pragma once

namespace panda2d {
struct Vec2 {
    union { float x; float w; };
    union { float y; float h; };

    bool operator<(const Vec2 &v) const {
        return (x < v.x && y < v.y);
    }
};

struct Vec3 {
    union { float x; float r; };
    union { float y; float g; };
    union { float z; float b; };
};

struct Vec4 {
    union { float x; float r; };
    union { float y; float g; };
    union { float z; float b; float width; };
    union { float w; float a; float height; };
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

} // namespace panda2d
