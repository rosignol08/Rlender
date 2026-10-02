#include "SceneNode.h"

// Sphere
SphereNode::SphereNode(){
    nom = "Sphere";
    taille = {2.0f, 2.0f, 2.0f};
    type = "sphere";
    // un maillage de base
    Mesh maillage = GenMeshSphere(1.0f, 16.0f, 16.0f);
    // ça contient automatiquement un Material par défaut
    modele = LoadModelFromMesh(maillage);
}

SphereNode::~SphereNode(){
    // faut decharger le modele
    UnloadModel(modele);
}

void SphereNode::Draw(EditorContext& variables){
    // les variables héritées de SceneNode
    DrawModelEx(modele, position, {0.0f, 1.0f, 0.0f}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawSphereWires(position, taille.x, 16, 16, YELLOW); // 10 par defaut TODO a changer
    }
}

std::string SphereNode::ToCode(){
    return "";
}

std::string SphereNode::GetDrawCode(){
    return "";
}

std::string SphereNode::GetInitCode(){
    return "";
}

BoundingBox SphereNode::GetBoiteCollision(){
    Vector3 min, max;
    BoundingBox ma_bounding_box;

    min.x = position.x - taille.x;
    min.y = position.y - taille.x;
    min.z = position.z - taille.x;
    max.x = position.x + taille.x;
    max.y = position.y + taille.x;
    max.z = position.z + taille.x;

    ma_bounding_box.min = min;
    ma_bounding_box.max = max;
    return ma_bounding_box;
}

std::unique_ptr<SceneNode> SphereNode::Cloner(){
    auto clone = std::make_unique<SphereNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->nom_shader_actuel = this->nom_shader_actuel;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    static unsigned int compteur_sphere_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_sphere_clones); // nouveau nom
    return clone;
}

//std::string GetType() { return "Sphere"; }