// | -------------------------------
#include "ObjectStateIO.hpp"
// | -------------------------------
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/IOPrimitives.hpp"
// | -------------------------------
#include <fstream>
// | -------------------------------


namespace ENG
{
  void WriteObject(std::ofstream& f, const ObjectState& s)
  {
    WriteString(f, s.m_name);
    WritePOD(f, s.m_layer);

    Write(f, s.m_tData);
    Write(f, s.m_sData);

    WriteOptional<ISpriteData>(f, s.m_spData, Write);
    WriteOptional<IAnimatorData>(f, s.m_anData, Write);
    WriteVector<IBoundingBoxData>(f, s.m_bbData, Write);
    WriteOptional<IColorData>(f, s.m_cData, Write);
  }

  ObjectState ReadObject(std::ifstream& f)
  {
    ObjectState s;
    s.m_name = ReadString(f);
    ReadPOD(f, s.m_layer);

    s.m_tData = ReadTransformData(f);
    s.m_sData = ReadStatsData(f);

    ReadOptional<ISpriteData>(f, s.m_spData, ReadSpriteData);
    ReadOptional<IAnimatorData>(f, s.m_anData, ReadAnimatorData);
    s.m_bbData = ReadVector<IBoundingBoxData>(f, ReadBoundingBoxData);
    ReadOptional<IColorData>(f, s.m_cData, ReadColorData);

    return s;
  }

  void Write(std::ofstream& f, const ITransformData& d) 
  {
    WritePOD(f, d);
  }
  ITransformData ReadTransformData(std::ifstream& f)
  {
    ENG::ITransformData d; 
    ReadPOD(f, d); 
    return d;
  }

  void Write(std::ofstream& f, const IStatsData& d)
  {
    WritePOD(f, d);
  }
  IStatsData ReadStatsData(std::ifstream& f)
  {
    IStatsData d;
    ReadPOD(f, d);
    return d;
  }

  void Write(std::ofstream& f, const IColorData& d)
  {
    WritePOD(f, d);
  }
  IColorData ReadColorData(std::ifstream& f)
  {
    IColorData d;
    ReadPOD(f, d);
    return d;
  }

  void Write(std::ofstream& f, const IBoundingBoxData& d)
  {
    WritePOD(f, d);
  }
  IBoundingBoxData ReadBoundingBoxData(std::ifstream& f)
  {
    IBoundingBoxData d;
    ReadPOD(f, d);
    return d;
  }

  void Write(std::ofstream& f, const IAnimatorData& d)
  {
    WriteString(f, d.m_key);
    WritePOD(f, d.m_frames);
    WritePOD(f, d.m_step);
    WritePOD(f, d.m_speed);
    WritePOD(f, d.m_scale);
  }
  IAnimatorData ReadAnimatorData(std::ifstream& f)
  {
    IAnimatorData d;
    d.m_key = ReadString(f);
    ReadPOD(f, d.m_frames);
    ReadPOD(f, d.m_step);
    ReadPOD(f, d.m_speed);
    ReadPOD(f, d.m_scale);
    return d;
  }

  void Write(std::ofstream& f, const ISpriteData& d)
  {
    WriteString(f, d.m_keyName);
    WritePOD(f, d.m_width);
    WritePOD(f, d.m_height);
    WritePOD(f, d.m_scale);
  }

  ISpriteData ReadSpriteData(std::ifstream& f)
  {
    ISpriteData d;
    d.m_keyName = ReadString(f);
    ReadPOD(f, d.m_width);
    ReadPOD(f, d.m_height);
    ReadPOD(f, d.m_scale);
    return d;
  }
}
