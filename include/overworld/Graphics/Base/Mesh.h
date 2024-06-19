#ifndef OWDS_GRAPHICS_BASE_MESH_H
#define OWDS_GRAPHICS_BASE_MESH_H

#include <vector>

#include "overworld/Graphics/Base/Vertex.h"

namespace owds {
  class Mesh
  {
  public:
    Mesh();

    // As for actor, each mesh is associated with a non-zero, unique id.
    const std::size_t id_;

    std::vector<owds::Vertex> vertices_{};
    std::vector<std::uint32_t> indices_{};
  };
} // namespace owds

#endif // OWDS_GRAPHICS_BASE_MESH_H
