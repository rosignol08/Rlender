#include "ShaderManager.h"

void ShaderManager::ChargerShaders(const std::string& cheminVS, const std::string& cheminFS) {
    //loadShader compile les fichiers texte (.vs et .fs) directement sur la carte graphique
    shaderEclairage = LoadShader(cheminVS.c_str(), cheminFS.c_str());

    //check si le chargement a réussi (si ID valide)
    if (shaderEclairage.id != 0) {
        std::cout << "SUCCES : Shader d'eclairage charge" << std::endl;
        
        //configuration de base : on dit au shader où se trouve la caméra
        shaderEclairage.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shaderEclairage, "viewPos");
    } else {
        std::cerr << "ERREUR : Impossible de charger les shaders depuis " << cheminVS << " et " << cheminFS << std::endl;
    }
}

void ShaderManager::Nettoyer() {
    if (shaderEclairage.id != 0) {
        UnloadShader(shaderEclairage);
    }
}

void ShaderManager::ChargerShaderDepuisTexte(const std::string& codeVS, const std::string& codeFS) {
    //si déjà un shader : unload pour eviter fuites de VRAM
    if (shaderEclairage.id != 0) {
        UnloadShader(shaderEclairage);
    }

    //compilation depuis string
    shaderEclairage = LoadShaderFromMemory(codeVS.c_str(), codeFS.c_str());

    if (shaderEclairage.id != 0) {
        std::cout << "SUCCES : Shader compile" << std::endl;
        shaderEclairage.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shaderEclairage, "viewPos");
    } else {
        std::cerr << "ERREUR : La compilation du shader echoue" << std::endl;
    }
}