#pragma once
#include "raylib.h"
#include <string>
#include <iostream>

class ShaderManager {
public:
    Shader shaderEclairage; //shader principal qui la lumière de base

    //load les fichiers GLSL (.vs et .fs) depuis le disque
    void ChargerShaders(const std::string& cheminVS, const std::string& cheminFS);
    
    //clean la VRAM à la fermeture de l'application
    void Nettoyer();
};