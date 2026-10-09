#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>

struct DataMateriel{
    int identifiant;//pour pouvoir identifier un materiel precis
    std::string nom;
    Color couleurAlbedo = WHITE;
    Texture2D textureAlbedo = {0};
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
        void ChargerTextureAlbedo(int id, const std::string& cheminFichier);
        void NettoyerTout();
        void Nettoyer_Materiau(int id);
        std::vector<DataMateriel>& GetTousLesMateriaux();
};

