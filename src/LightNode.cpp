#include "LightNode.h"

LightNode::LightNode(){
    position = {0.0f, 0.0f, 0.0f};
    couleur_lumiere = {1.0f, 1.0f, 1.0f};
    intensite = 1.1;
}

void LightNode::Update(EditorContext& variables){
    variables.variables_globales["lightPos"] = this->position;
    variables.variables_globales["lightColor"] = this->couleur_lumiere;
    variables.variables_globales["lightIntensity"] = this->intensite;
}


void LightNode::Draw(EditorContext& variables){
    DrawSphereWires(position, 0.5f,16,16, YELLOW);
}

std::unique_ptr<SceneNode> LightNode::Cloner(){
    auto clone = std::make_unique<LightNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->intensite = this->intensite;
    clone->isSelected = true;
    static unsigned int compteur_Light_clones = 0; //logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Light_clones); // nouveau nom
    return clone;
}

std::string LightNode::ToCode(){
    return "";
}

std::string LightNode::GetDrawCode(){
    return "";
}

std::string LightNode::GetInitCode(){
    return "";
}

std::string LightNode::GetType() const {
    return "LightNode";
}


