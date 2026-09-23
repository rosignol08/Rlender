#include "SceneNode.h"

CubeNode::CubeNode() { 
    nom = "Cube"; 
    taille = {2.0f, 2.0f, 2.0f}; // Petite taille par défaut pour le voir
    type = "cube";
    //un maillage de base
    Mesh maillage = GenMeshCube(1.0f, 1.0f, 1.0f);
    //ça contient automatiquement un Material par défaut
    modele = LoadModelFromMesh(maillage);
}

CubeNode::~CubeNode() {
    //faut decharger le modele
    UnloadModel(modele);
}

std::string CubeNode::GetDrawCode() {
    std::stringstream code;
    code << "\t\t\t\t\t\t\t\tDrawCube(" << "pos_" << this->nom << ", " << std::to_string(this->taille.x) << ", " << std::to_string(this->taille.y) << ", " << std::to_string(this->taille.z) << ", " << "{ " << std::to_string(this->couleur.r) << ", " << std::to_string(this->couleur.g) << ", " <<std::to_string(this->couleur.b) << ", 255" <<"}" << ");\n";
    return code.str();
}

std::string CubeNode::GetInitCode(){
        std::stringstream code;
        code << "\t\t\t\tVector3 " << "pos_" << this->nom << " = { " << std::to_string(this->position.x) << ", " << std::to_string(this->position.y) << ", " <<std::to_string(this->position.z) << "};\n";
        return code.str();
    }
void CubeNode::Draw() {
    //les variables héritées de SceneNode
    DrawModelEx(modele, position, {0.0f, 1.0f, 0.0f}, 0.0f, taille, couleur);
    if (isSelected) {
        DrawModelWiresEx(modele,position, {0.0f, 1.0f, 0.0f}, 0.0f, taille, YELLOW);
    }
}


std::unique_ptr<SceneNode> CubeNode::Cloner() {
    auto clone = std::make_unique<CubeNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->isSelected = true;
    static unsigned int compteur_cube_clones = 0;//logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_cube_clones); //nouveau nom
    return clone;
}