#include "shapes.hpp"
#include "vertex.hpp"
#include "mesh.hpp"
#include "pipe.hpp"

std::unique_ptr<Mesh> geometry::cuboid(const glm::vec3& halfExtents, bool inwards, const std::shared_ptr<Texture>& texture) {
    float orientation = 1;
    if (inwards)
        orientation = -1;

    std::vector<Vertex> cuboid_vertices {
        //front
        //                                 position														 normal					      tex coord
        { {-1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 0.f, 1.f } },

        //right
        { { 1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

        //back
        { { 1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 0.f } },
        { {-1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f, 0.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f, 1.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 1.f } },

        //left
        { {-1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
        { {-1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

        //top
        { {-1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

        //bottom
        { {-1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 1.f } },
    };

    std::vector<uint32_t> cuboid_indices {
        0,  1,  2,		 0,  2,  3,  //front
        4,  5,  6,		 4,  6,  7,  //right
        8,  9, 10,		 8, 10, 11,  //back
        12, 13, 14,		12, 14, 15,  //left
        16, 17, 18,		16, 18, 19,  //upper
        20, 21, 22,		20, 22, 23   //bottom
    };

    return std::make_unique<Mesh>(std::move(cuboid_vertices), std::move(cuboid_indices), texture);
}

std::unique_ptr<Mesh> geometry::sphere(uint32_t stacks, uint32_t slices, float radius, const std::shared_ptr<Texture>& texture) {
    std::vector<Vertex> sphere_vertices;
    std::vector<uint32_t> sphere_indices;

    const float sliceStep = 2 * M_PI / slices;
    const float stackStep = M_PI / stacks;
    const float lengthInv = 1.0f / radius;

    for (uint32_t i = 0; i <= stacks; ++i) {
        const float stackAngle = M_PI_2 - i * stackStep;  // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);             // r * cos(u)
        float z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sliceCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (uint32_t j = 0; j <= slices; ++j) {
            const float sliceAngle = j * sliceStep;       // starting from 0 to 2pi

            // vertex position (x, y, z)
            const float pos_x = xy * cosf(sliceAngle);    // r * cos(u) * cos(v)
            const float pos_y = xy * sinf(sliceAngle);    // r * cos(u) * sin(v)
            const float pos_z = z;

            // normalized vertex normal (x, y, z)
            float norm_x = pos_x * lengthInv;
            float norm_y = pos_y * lengthInv;
            float norm_z = pos_z * lengthInv;

            // vertex tex coord (x, y) range between [0, 1]
            float tex_y = 1.0f - static_cast<float>(j) / slices;
            float tex_x = static_cast<float>(i) / stacks;

            sphere_vertices.emplace_back(
                    glm::vec3{ pos_x, pos_y, pos_z },
                    glm::vec3{ norm_x, norm_y, norm_z },
                    glm::vec2{ tex_x, tex_y });
        }
    }

    for (uint32_t i = 0; i < stacks; ++i) {
        uint32_t k1 = i * (slices + 1);      // beginning of current stack
        uint32_t k2 = k1 + slices + 1;      // beginning of next stack

        for (uint32_t j = 0; j < slices; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                sphere_indices.push_back(k1);
                sphere_indices.push_back(k2);
                sphere_indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stacks - 1)) {
                sphere_indices.push_back(k1 + 1);
                sphere_indices.push_back(k2);
                sphere_indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_unique<Mesh>(std::move(sphere_vertices), std::move(sphere_indices), texture);
}

std::unique_ptr<Mesh> geometry::quad(const glm::vec2& extent, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec3> vertices;
    vertices.emplace_back(-extent.x, -extent.y, 0.f);
    vertices.emplace_back(extent.x, -extent.y, 0.f);
    vertices.emplace_back(-extent.x, extent.y, 0.f);
    vertices.emplace_back(extent.x, -extent.y, 0.f);
    vertices.emplace_back(extent.x, extent.y, 0.f);
    vertices.emplace_back(-extent.x, extent.y, 0.f);

    std::vector<Vertex> quad_vertices {
        //  position                                  normal                         tex coord
        { vertices.at(0),  { 1.f, 1.f, 1.f },  { 0.f,  0.f } },
        { vertices.at(1),  { 1.f, 1.f, 1.f },  { 1.f,  0.f } },
        { vertices.at(2),  { 1.f, 1.f, 1.f },  { 0.0f, 1.f } },
        { vertices.at(3),  { 1.f, 1.f, 1.f },  { 1.f,  0.f } },
        { vertices.at(4),  { 1.f, 1.f, 1.f },  { 1.f,  1.f } },
        { vertices.at(5),  { 1.f, 1.f, 1.f },  { 0.0f, 1.f } },
    };

    std::vector<uint32_t> quad_indices {
        0,  1,  2,
        3,  4,  5,
    };

    return std::make_unique<Mesh>(std::move(quad_vertices), std::move(quad_indices), texture);
}

std::unique_ptr<Mesh> geometry::octahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec3> vertices;
    vertices.emplace_back(0.f, extent.y, 0.f);
    vertices.emplace_back(extent.x, 0.f, extent.z);
    vertices.emplace_back(-extent.x, 0.f, extent.z);
    vertices.emplace_back(-extent.x, 0.f, -extent.z);
    vertices.emplace_back(extent.x, 0.f, -extent.z);
    vertices.emplace_back(0.f, -extent.y, 0.f);

    std::vector<glm::vec3> normals;
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(4)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(3)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(1)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) - vertices.at(2)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(4)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(3)));

    std::vector<Vertex> octahedron_vertices {
        //  position                             normal                       tex coord
        { vertices.at(0),   normals.at(0),   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(0),   { 1.f,  0.f } },
        { vertices.at(1),   normals.at(0),   { 0.5f, 1.f } },
        { vertices.at(0),   normals.at(1),   { 0.f,  0.f } },
        { vertices.at(3),   normals.at(1),   { 1.f,  0.f } },
        { vertices.at(2),   normals.at(1),   { 0.5f, 1.f } },
        { vertices.at(0),   normals.at(2),   { 0.f,  0.f } },
        { vertices.at(1),   normals.at(2),   { 1.f,  0.f } },
        { vertices.at(4),   normals.at(2),   { 0.5f, 1.f } },
        { vertices.at(0),   normals.at(3),   { 0.f,  0.f } },
        { vertices.at(4),   normals.at(3),   { 1.f,  0.f } },
        { vertices.at(3),   normals.at(3),   { 0.5f, 1.f } },
        { vertices.at(1),   normals.at(4),   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(4),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(4),   { 0.5f, 1.f } },
        { vertices.at(2),   normals.at(5),   { 0.f,  0.f } },
        { vertices.at(3),   normals.at(5),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(5),   { 0.5f, 1.f } },
        { vertices.at(4),   normals.at(6),   { 0.f,  0.f } },
        { vertices.at(1),   normals.at(6),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(6),   { 0.5f, 1.f } },
        { vertices.at(3),   normals.at(7),   { 0.f,  0.f } },
        { vertices.at(4),   normals.at(7),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(7),   { 0.5f, 1.f } },
    };

    return std::make_unique<Mesh>(std::move(octahedron_vertices), texture);
}

std::unique_ptr<Mesh> geometry::tetrahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec3> vertices;
    vertices.emplace_back(0.f, extent.y, 0.f);
    vertices.emplace_back(0.f, 0.f, extent.z);
    vertices.emplace_back(-extent.x, 0.f, -extent.z);
    vertices.emplace_back(extent.x, 0.f, -extent.z);

    std::vector<glm::vec3> normals;
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(3)));
    normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(3)));

    std::vector<Vertex> tetrahedron_vertices {
        //  position			normal			         tex coord
        { vertices.at(0),   normals.at(0),	   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(0),	   { 1.f,  0.f } },
        { vertices.at(1),   normals.at(0),	   { 0.5f, 1.f } },

        { vertices.at(0),   normals.at(1),	   { 0.f,  0.f } },
        { vertices.at(3),   normals.at(1),	   { 1.f,  0.f } },
        { vertices.at(2),   normals.at(1),	   { 0.5f, 1.f } },

        { vertices.at(0),   normals.at(2),	   { 0.f,  0.f } },
        { vertices.at(1),   normals.at(2),	   { 1.f,  0.f } },
        { vertices.at(3),   normals.at(2),	   { 0.5f, 1.f } },

        { vertices.at(1),   normals.at(3),	   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(3),	   { 1.f,  0.f } },
        { vertices.at(3),   normals.at(3),    { 0.5f, 1.f } },
    };

    return std::make_unique<Mesh>(std::move(tetrahedron_vertices), texture);
}

std::unique_ptr<Mesh> geometry::pipe(const Pipe& pipe, const std::shared_ptr<Texture>& texture) {
    std::vector<Vertex> pipe_vertices;

    // surface
    for (int i = 0; i < pipe.getContourCount() - 1; ++i) {
        const std::vector<glm::vec3>& c1 = pipe.getContour(i);
        const std::vector<glm::vec3>& c2 = pipe.getContour(i + 1);
        const std::vector<glm::vec3>& n1 = pipe.getNormal(i);
        const std::vector<glm::vec3>& n2 = pipe.getNormal(i + 1);

        for (int j = 0; j < c2.size(); ++j) {
            pipe_vertices.emplace_back(c2[j], n2[j], glm::vec2{0});
            pipe_vertices.emplace_back(c1[j], n1[j], glm::vec2{0});
        }
    }

    return std::make_unique<Mesh>(std::move(pipe_vertices), texture, GL_TRIANGLE_STRIP);
}

std::vector<glm::vec3> shape::spiralPath(float r1, float r2, float h1, float h2, float turns, int points) {
    const float PI = acosf(-1);
    std::vector<glm::vec3> vertices;
    glm::vec3 vertex;
    float r = r1;
    float rStep = (r2 - r1) / static_cast<float>(points - 1);
    float y = h1;
    float yStep = (h2 - h1) / static_cast<float>(points - 1);
    float a = 0;
    float aStep = (turns * 2 * PI) / static_cast<float>(points - 1);
    for (int i = 0; i < points; ++i) {
        vertex.x = r * cosf(a);
        vertex.z = r * sinf(a);
        vertex.y = y;
        vertices.push_back(vertex);
        // next
        r += rStep;
        y += yStep;
        a += aStep;
    }
    return vertices;
}

std::vector<glm::vec3> shape::circle(const glm::vec2& radius, int steps) {
    std::vector<glm::vec3> points;
    if(steps < 2) return points;

    const float PI2 = acosf(-1) * 2.0f;
    float x, y, a;
    for (int i = 0; i <= steps; ++i) {
        a = PI2 / steps * i;
        x = radius.x * cosf(a);
        y = radius.y * sinf(a);
        points.emplace_back(x, y, 0);
    }
    return points;
}