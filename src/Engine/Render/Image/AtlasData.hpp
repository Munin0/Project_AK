// | -------------------------------
#pragma once
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
#include <string>
#include <unordered_map>
// | -------------------------------


namespace ENG
{
  struct AnimationRegion
  {
    int col;         // columna de inicio en el grid (índice, no pixeles)
    int row;         // fila de inicio en el grid (índice, no pixeles)
    int frameCount;  // cuántas casillas consecutivas hacia la derecha ocupa la animación
  };

  struct AtlasData
  {
    std::string name;
    std::string texturePath;
    int atlasWidth, atlasHeight;   // tamaño real del atlas en pixeles
    int tileSize;                   // tamaño de cada casilla del grid, en pixeles
    int atlasLayer;                 // se llena al hacer UploadLayer(), no viene del JSON
    int containerWidth = 0, containerHeight = 0; // tamaño real de la capa en la TextureArray (base de normalizacion UV)
    std::unordered_map<std::string, AnimationRegion> animations;
  };

  struct UVRect
  {
    glm::vec2 uvMin;
    glm::vec2 uvMax;
  };

  [[maybe_unused]] UVRect GetFrameUV(const AtlasData& atlas, const std::string& animName, int frameIndex);
  [[maybe_unused]]AtlasData ParseAtlasJSON(const std::string& jsonPath);
}



