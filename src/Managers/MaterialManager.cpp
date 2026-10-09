#include "MaterialManager.h"


ManagerMateriel::ManagerMateriel(){
    ManagerMateriel::CreerMateriau("Materiau_Defaut");
}
ManagerMateriel::~ManagerMateriel(){
    NettoyerTout();
}

int ManagerMateriel::CreerMateriau(const std::string& nom) {
    DataMateriel nouveau_mat(nom);
    //taille actuelle comme identifiant
    nouveau_mat.identifiant = liste_materiaux.size(); 
    
    liste_materiaux.push_back(nouveau_mat);
    return nouveau_mat.identifiant;
}

DataMateriel* ManagerMateriel::GetMateriau(int id) {
        if (id >= 0 && id < liste_materiaux.size()) {
            return &liste_materiaux[id];
        }
        return nullptr;
    }
void ManagerMateriel::ChargerTextureAlbedo(int id, const std::string& cheminFichier) {
        DataMateriel* mat = GetMateriau(id);
        if (mat) {
            // Si une texture existait déjà, on la décharge pour éviter les fuites de VRAM
            if (mat->textureAlbedo.id != 0) {
                UnloadTexture(mat->textureAlbedo);
            }
            mat->textureAlbedo = LoadTexture(cheminFichier.c_str());
            std::cout << "Texture chargee pour le materiau : " << mat->nom << std::endl;
        }
    }

void ManagerMateriel::NettoyerTout() {
    for (auto& mat : liste_materiaux) {
        if (mat.textureAlbedo.id != 0) {
            UnloadTexture(mat.textureAlbedo);
            mat.textureAlbedo = { 0 };
        }
    }
    liste_materiaux.clear();
}
void ManagerMateriel::Nettoyer_Materiau(int id){
    for (auto it = liste_materiaux.begin(); it != liste_materiaux.end(); ++it) {
        
        if (it->identifiant == id) {
            UnloadTexture(it->textureAlbedo);
            liste_materiaux.erase(it);
            break; 
        }
    }
}

std::vector<DataMateriel>& ManagerMateriel::GetTousLesMateriaux() { return liste_materiaux; }

// DataMateriel