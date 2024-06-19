#include "overworld/Graphics/Assimp/ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <fstream>

#include "overworld/Graphics/Base/Model.h"

namespace owds::assimp {
  owds::Mesh processMesh(const aiMesh* mesh, const aiScene* scene)
  {
    owds::Mesh out_mesh{};

    for(auto i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex{};

      vertex.position_ = {
        mesh->mVertices[i].x,
        mesh->mVertices[i].y,
        mesh->mVertices[i].z};

      vertex.normal_ = {
        mesh->mNormals[i].x,
        mesh->mNormals[i].y,
        mesh->mNormals[i].z};

      if(mesh->mTextureCoords[0])
      {
        vertex.uv_ = {
          mesh->mTextureCoords[0][i].x,
          mesh->mTextureCoords[0][i].y};
      }

      vertex.rgba_ = {255, 255, 255, 255};

      out_mesh.vertices_.emplace_back(vertex);
    }

    for(auto i = 0; i < mesh->mNumFaces; i++)
    {
      const aiFace face = mesh->mFaces[i];

      for(auto j = 0; j < face.mNumIndices; j++)
      {
        out_mesh.indices_.emplace_back(face.mIndices[j]);
      }
    }

    return std::move(out_mesh);
  }

  void processNode(owds::Model& out, const aiNode* node, const aiScene* scene)
  {
    for(auto i = 0; i < node->mNumMeshes; i++)
    {
      out.meshes_.emplace_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    }

    for(auto i = 0; i < node->mNumChildren; i++)
    {
      processNode(out, node->mChildren[i], scene);
    }
  }

  bool loadModel(owds::Model& out, const std::filesystem::path& path)
  {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
      path,
      0 |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      return false;
    }

    out.source_path_ = path.string();
    processNode(out, scene->mRootNode, scene);

    return true;
  }

  std::unique_ptr<owds::Model> ModelLoader::load(const std::filesystem::path& path) const
  {
    auto model = std::make_unique<owds::Model>();
    model->source_path_ = path.string();

    if(!loadModel(*model, path))
    {
      return nullptr;
    }

    return model;
  }
} // namespace owds::assimp