#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "ShaderManager.h"

struct DataMateriel{
    int identifiant;//pour pouvoir identifier un materiel precis
    std::string nom;
    Color couleurAlbedo = WHITE;
    Texture2D textureAlbedo = {0};
    std::unordered_map<std::string, std::string> chemins_textures;
    std::string nom_shader_actuel = "Defaut";//faut que je mette le shader defaut un shader pbr phong etc
    //plus tard : Normal map, Roughness, Metallic, et l'ID du Shader
    DataMateriel(std::string n) : nom(n){};
};


class ManagerMateriel {
    //par défaut c'est privé
        std::vector<DataMateriel> liste_materiaux;
    public:
        ManagerMateriel();
        ~ManagerMateriel();

        int CreerMateriau(const std::string& nom);
        DataMateriel* GetMateriau(int id);
        DataMateriel* GetMateriau(const std::string& nom);
        void ChargerTextureAlbedo(int id, const std::string& cheminFichier);
        void NettoyerTout();
        void Nettoyer_Materiau(int id);
        std::vector<DataMateriel>& GetTousLesMateriaux();
};

