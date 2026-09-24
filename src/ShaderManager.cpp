#include "ShaderManager.h"

//faut changer la fonction pour charger plein de shaders différents pas que la lumière
void ShaderManager::ChargerShaders(const std::string& cheminVS, const std::string& cheminFS) {
    //loadShader compile les fichiers texte (.vs et .fs) directement sur la carte graphique
    if (cheminVS.empty() && cheminFS.empty()) {
        std::cerr << "ERREUR : Aucun chemin fourni" << std::endl;
        return;
    }
    //le nom
    std::string nom = "";
    if (!cheminVS.empty()) {
        nom = std::filesystem::path(cheminVS).stem().string(); 
    } else {
        nom = std::filesystem::path(cheminFS).stem().string();
    }
    //chargement du shader
    Shader le_shader = { 0 };
    if (!cheminVS.empty() && !cheminFS.empty()) {
        
        le_shader = LoadShader(cheminVS.c_str(), cheminFS.c_str());
    
    }else if (!cheminVS.empty()) {

        le_shader = LoadShader(cheminVS.c_str(), 0);//0 pour shader par défaut
    
    }else if (!cheminFS.empty()) {
        
        le_shader = LoadShader(0, cheminFS.c_str());
    
    }
    //sauvgarde dans le dico
    if (le_shader.id != 0){//check si c'est bon
        //si il existe déjà on le décharge du gpu pour l'update
        if(dictionnaire_shaders.count(nom) > 0){
            UnloadShader(dictionnaire_shaders[nom]);
        }
        dictionnaire_shaders.insert_or_assign(nom,le_shader);
        std::cout<< "SUCCES : Shader '" << nom << "' ajoute au dictionaire" << std::endl;
    }
    else {
        std::cerr << "ERREUR : Impossible de charger les shaders depuis " << cheminVS << " et " << cheminFS << std::endl;
    }
}

void ShaderManager::Nettoyer_tout() {
    for (auto& [cle, valeur] : dictionnaire_shaders) {
        UnloadShader(valeur);
    }
    dictionnaire_shaders.clear();
}

//pour enlever qu'un seule shader
void ShaderManager::Nettoyer_un(const std::string& nom) {
    if (dictionnaire_shaders.count(nom) > 0){//verification
        UnloadShader(dictionnaire_shaders[nom]);
        dictionnaire_shaders.erase(nom);
    }
}
void ShaderManager::ChargerShaderDepuisTexte(const std::string& nom, const std::string& codeVS, const std::string& codeFS){
    //chargement du shader
    Shader le_shader = { 0 };
    if (!codeVS.empty() && !codeFS.empty()) {
        
        le_shader = LoadShaderFromMemory(codeVS.c_str(), codeFS.c_str());
    
    }else if (!codeVS.empty()) {

        le_shader = LoadShaderFromMemory(codeVS.c_str(), 0);//0 pour shader par défaut
    
    }else if (!codeFS.empty()) {
        
        le_shader = LoadShaderFromMemory(0, codeFS.c_str());
    
    }
    //sauvgarde dans le dico
    if (le_shader.id != 0){//check si c'est bon
        //si il existe déjà on le décharge du gpu pour l'update
        if(dictionnaire_shaders.count(nom) > 0){
            UnloadShader(dictionnaire_shaders[nom]);
        }
        dictionnaire_shaders.insert_or_assign(nom,le_shader);
        std::cout<< "SUCCES : Shader '" << nom << "' ajoute au dictionaire" << std::endl;
    }
    else {
        std::cerr << "ERREUR : Impossible de charger le shader" << std::endl;
    }
}