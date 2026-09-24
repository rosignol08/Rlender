#pragma once
#include "raylib.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>

class ShaderManager {
public:
    std::unordered_map<std::string, Shader> dictionnaire_shaders;//la liste des shaders compilés
    //load les fichiers GLSL (.vs et .fs) depuis le disque
    void ChargerShaders(const std::string& cheminVS, const std::string& cheminFS);
    
    //clean la VRAM à la fermeture de l'application
    void Nettoyer_tout();
    void Nettoyer_un(const std::string& nom);
    //load et compile un shader directement avec code (Live Coding)
    void ChargerShaderDepuisTexte(const std::string& codeVS, const std::string& codeFS);
};