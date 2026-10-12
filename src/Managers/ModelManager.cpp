#include "ModelManager.h"

//std::unordered_map<std::string, Model> dictionnaire_modeles;

Model* ManagerModele::Charger_modele(std::string nom_modele){
    auto pos = dictionnaire_modeles.find(nom_modele);
    if(pos == dictionnaire_modeles.end()){
        //existe pas dedans on le charge
        dictionnaire_modeles.insert({nom_modele, LoadModel(nom_modele.c_str())});
        return &dictionnaire_modeles.at(nom_modele);
    }else{
        return &(pos->second);
    }
}


void ManagerModele::Vider_modeles(){
    //on vide tous les modele du dictionaire
    for(auto & element : dictionnaire_modeles){
        UnloadModel(element.second);
    }
    dictionnaire_modeles.clear();
}

ManagerModele::ManagerModele(){};

ManagerModele::~ManagerModele(){
    Vider_modeles();
}