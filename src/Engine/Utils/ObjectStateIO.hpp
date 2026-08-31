// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Services/WorldSaver.hpp"
// | -------------------------------
#include <fstream>
#include <string>
// | -------------------------------


/// Write datas
namespace ENG
{
  [[maybe_unused]] void WriteObject(std::ofstream& f, const ObjectState& obj);
  [[maybe_unused]] ObjectState ReadObject(std::ifstream& f);

  [[maybe_unused]] void WriteString(std::ofstream& f, const std::string& s);
  [[maybe_unused]] std::string ReadString(std::ifstream& f);

  [[maybe_unused]] void Write(std::ofstream& f, const ITransformData& d);
  [[maybe_unused]] ITransformData ReadTransformData(std::ifstream& f);

  [[maybe_unused]] void Write(std::ofstream& f, const IStatsData& d);
  [[maybe_unused]] IStatsData ReadStatsData(std::ifstream& f);
  
  [[maybe_unused]] void Write(std::ofstream& f, const IColorData& d);
  [[maybe_unused]] IColorData ReadColorData(std::ifstream& f);

  [[maybe_unused]] void Write(std::ofstream& f, const IBoundingBoxData& d);
  [[maybe_unused]] IBoundingBoxData ReadBoundingBoxData(std::ifstream& f);

  [[maybe_unused]] void Write(std::ofstream& f, const IAnimatorData& d);
  [[maybe_unused]] IAnimatorData ReadAnimatorData(std::ifstream& f);

  [[maybe_unused]] void Write(std::ofstream& f, const ISpriteData& d);
  [[maybe_unused]] ISpriteData ReadSpriteData(std::ifstream& f);
}
  
