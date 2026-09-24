#pragma once
#include "raylib.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>

class ShaderManager {
public:
    Shader shaderEclairage; //shader principal qui la lumière de base
    std::unordered_map<std::string, Shader> dictionnaire_shaders;//la liste des shaders compilés
    //load les fichiers GLSL (.vs et .fs) depuis le disque
    void ChargerShaders(const std::string& cheminVS, const std::string& cheminFS);
    
    //clean la VRAM à la fermeture de l'application
    void Nettoyer();
    //load et compile un shader directement avec code (Live Coding)
    void ChargerShaderDepuisTexte(const std::string& codeVS, const std::string& codeFS);
};