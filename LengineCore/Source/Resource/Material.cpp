#include "Material.h"

// --- STD ---
#include <fstream>
#include <sstream>


//RMaterial::RMaterial()
//{
//}
//
//RMaterial::~RMaterial()
//{
//}

//RMaterialInstance* RMaterial::CreateMaterialInstance()
//{
//    return new RMaterialInstance(this);
//}

//bool RMaterial::Load(const string& a_FileName)
//{
//    TODO("Make a material save file");
//    return false;
//}
//
//bool RMaterial::IsValid()
//{
//    return m_Shader != nullptr;
//}

//void RMaterial::Bind()
//{
//    BindAmbient();
//    BindDiffuse();
//    BindSpecular();
//    BindSpecularPower();
//
//    DiffuseTexture.bind(0);
//    m_Shader->bindUniform("DiffuseTex", 0);
//    SpecularTexture.bind(3);
//    m_Shader->bindUniform("SpecularTexture", 3);
//    NormalTexture.bind(5);
//    m_Shader->bindUniform("NormalTexture", 5);
//}
//
//void RMaterial::BindAmbient()
//{
//    m_Shader->bindUniform("Ka", Ambient);
//}
//
//void RMaterial::BindDiffuse()
//{
//    m_Shader->bindUniform("Kd", Diffuse);
//}
//
//void RMaterial::BindSpecular()
//{
//    m_Shader->bindUniform("Ks", Specular);
//}
//
//void RMaterial::BindEmissive()
//{
//    
//}
//
//void RMaterial::BindSpecularPower()
//{
//    m_Shader->bindUniform("SpecularPower", SpecularPower);
//}
//
//void RMaterial::BindOpacity()
//{
//}
//
//bool RMaterial::LoadMaterial(const char* a_FileName)
//{
//    std::fstream File(a_FileName, std::ios::in);
//    std::string Line;
//    std::string Header;
//    char Buffer[256];
//
//    // Get the path of the a_FileName for use with relative paths for maps later.
//    std::string Directory(a_FileName);
//    int Index = Directory.rfind('/');
//    if (Index != -1)
//        Directory = Directory.substr(0, Index + 1);
//
//    while (!File.eof())
//    {
//        File.getline(Buffer, 256);
//        Line = Buffer;
//        std::stringstream ss(Line,
//            std::stringstream::in | std::stringstream::out);
//        if (Line.find("Ka") == 0)
//            ss >> Header >> Ambient.x >> Ambient.y >> Ambient.z;
//        else if (Line.find("Ks") == 0)
//            ss >> Header >> Specular.x >> Specular.y >> Specular.z;
//        else if (Line.find("Kd") == 0)
//            ss >> Header >> Diffuse.x >> Diffuse.y >> Diffuse.z;
//        else if (Line.find("Ns") == 0)
//            ss >> Header >> SpecularPower;
//        else if (Line.find("map_Kd") == 0)
//        {
//            std::string MapFileName;
//            ss >> Header >> MapFileName;
//            DiffuseTexture.load((Directory + MapFileName).c_str());
//        }
//        else if (Line.find("map_Ks") == 0)
//        {
//            std::string MapFileName;
//            ss >> Header >> MapFileName;
//            SpecularTexture.load((Directory + MapFileName).c_str());
//        }
//        else if (Line.find("bump") == 0)
//        {
//            std::string MapFileName;
//            ss >> Header >> MapFileName;
//            NormalTexture.load((Directory + MapFileName).c_str());
//        }
//    }
//
//    return true;
//}
//
//bool RMaterial::LoadShader(const char* a_FileName)
//{
//    std::string FileName = std::string("Resources/Shaders/").append(a_FileName);
//    //std::string FileName = std::string("shaders/").append(a_FileName);
//    std::string Vert = FileName.c_str();
//    std::string Frag = FileName.c_str();
//
//    m_Shader->loadShader(aie::eShaderStage::VERTEX,
//        Vert.append(".vert").c_str());
//    m_Shader->loadShader(aie::eShaderStage::FRAGMENT,
//        Frag.append(".frag").c_str());
//    if (m_Shader->link() == false)
//    {
//        printf(std::string(a_FileName).append(" Shader Error: %s\n").c_str(), m_Shader->getLastError());
//        return false;
//    }
//
//    Bind();
//
//    return true;
//}
//
//bool RMaterial::LoadTexture(const char* a_FileName)
//{
//    return DiffuseTexture.load(a_FileName);
//}
