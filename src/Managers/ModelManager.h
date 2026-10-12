#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "MaterialManager.h"

//pour stoquer une seule fois les modèles
class ManagerModele {
    public :
        std::unordered_map<std::string, Model> dictionnaire_modeles;
        Model* Charger_modele(std::string nom_modele);//pour chercher ou charger le modele dans le dictionaire
        void Vider_modeles();
        ManagerModele();
        ~ManagerModele();
};