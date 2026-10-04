#include "SceneNode.h"

// le plan
PlaneNode::PlaneNode(){
    nom = "Plane";
    type = "plane";
    taille = {5.0f, 1.0f, 5.0f};
    modele = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 10, 10)); // resolution 10x10
}

PlaneNode::~PlaneNode(){
    UnloadModel(modele);
}

void PlaneNode::Draw(EditorContext& variables){
    DrawModelEx(modele, position, {0, 1, 0}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawModelWiresEx(modele, position, {0, 1, 0}, 0.0f, taille, YELLOW);
    }
}

BoundingBox PlaneNode::GetBoiteCollision(){
    return {
        {position.x - taille.x / 2, position.y - 0.01f, position.z - taille.z / 2},
        {position.x + taille.x / 2, position.y + 0.01f, position.z + taille.z / 2}};
}

std::unique_ptr<SceneNode> PlaneNode::Cloner()
{
    auto clone = std::make_unique<PlaneNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->nom_shader_actuel = this->nom_shader_actuel;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    clone->nom = this->nom + "_clone";
    return clone;
}

std::string PlaneNode::ToCode(){
    return "";
}

std::string PlaneNode::GetDrawCode(){
    return "";
}

std::string PlaneNode::GetInitCode(){
    return "";
}
//std::string GetType() { return "Plan"; }