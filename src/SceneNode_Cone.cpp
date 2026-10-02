#include "SceneNode.h"

// cone
// pour representer un cone
ConeNode::ConeNode(){
    nom = "Cone";
    type = "cone";
    taille = {1.0f, 2.0f, 1.0f};
    modele = LoadModelFromMesh(GenMeshCone(1.0f, 1.0f, 16));
}

ConeNode::~ConeNode(){
    UnloadModel(modele);
}

void ConeNode::Draw(EditorContext& variables){
    DrawModelEx(modele, position, {0, 1, 0}, 0.0f, taille, couleur);
    if (isSelected)
        DrawModelWiresEx(modele, position, {0, 1, 0}, 0.0f, taille, YELLOW);
}

BoundingBox ConeNode::GetBoiteCollision(){
    return {{position.x - taille.x, position.y - 0.0f, position.z - taille.z}, // Base au sol
            {position.x + taille.x, position.y + taille.y, position.z + taille.z}};
}
std::unique_ptr<SceneNode> ConeNode::Cloner(){
    auto clone = std::make_unique<ConeNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->nom_shader_actuel = this->nom_shader_actuel;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    clone->nom = this->nom + "_clone";
    return clone;
}
std::string ConeNode::ToCode(){
    return "";
}

std::string ConeNode::GetDrawCode(){
    return "";
}

std::string ConeNode::GetInitCode(){
    return "";
}

//std::string GetType() { return "Camera"; }