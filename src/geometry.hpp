#pragma once

class Pipe;
class Mesh;
class Texture;

namespace geometry {
    std::unique_ptr<Mesh> cuboid(const glm::vec3& halfExtents, bool inwards, const std::shared_ptr<Texture>& texture);
    std::unique_ptr<Mesh> sphere(uint32_t stacks, uint32_t slices, float radius, const std::shared_ptr<Texture>& texture);
    std::unique_ptr<Mesh> quad(const glm::vec2& extent, const std::shared_ptr<Texture>& texture);
    std::unique_ptr<Mesh> octahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture);
    std::unique_ptr<Mesh> tetrahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture);
    std::unique_ptr<Mesh> pipe(const Pipe& pipe, const std::shared_ptr<Texture>& texture);
}

namespace shape {
    std::vector<glm::vec3> circle(const glm::vec2& radius, int steps);
}