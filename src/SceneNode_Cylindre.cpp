#include "SceneNode.h"

// cylindre
CylinderNode::CylinderNode(){
    nom = "Cylinder";
    type = "cylinder";
    taille = {1.0f, 2.0f, 1.0f};
    modele = LoadModelFromMesh(GenMeshCylinder(1.0f, 1.0f, 16));
}

CylinderNode::~CylinderNode(){
    UnloadModel(modele);
}

void CylinderNode::Draw(){
    DrawModelEx(modele, position, {0, 1, 0}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawModelWiresEx(modele, position, {0, 1, 0}, 0.0f, taille, YELLOW);
    }
}

BoundingBox CylinderNode::GetBoiteCollision(){
    return {{position.x - taille.x, position.y - taille.y / 2, position.z - taille.z},
            {position.x + taille.x, position.y + taille.y / 2, position.z + taille.z}};
}
std::unique_ptr<SceneNode> CylinderNode::Cloner(){
    auto clone = std::make_unique<CylinderNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->nom_shader_actuel = this->nom_shader_actuel;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    clone->nom = this->nom + "_clone";
    return clone;
}

std::string CylinderNode::ToCode(){
    return "";
}

std::string CylinderNode::GetDrawCode(){
    return "";
}

std::string CylinderNode::GetInitCode(){
    return "";
}
