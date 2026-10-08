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
    
    Shader le_shader = modele.materials[0].shader;//pour limiter les acces mémoire
    AppliquerVariablesGlobales(variables, le_shader);//fonction utilitaire de gestion des uniformes de shaders
    Matrix matTransform = MatrixIdentity();
    matTransform = MatrixMultiply(matTransform, MatrixScale(taille.x, taille.y, taille.z));
    matTransform = MatrixMultiply(matTransform, MatrixRotateXYZ((Vector3){rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD}));
    matTransform = MatrixMultiply(matTransform, MatrixTranslate(position.x, position.y, position.z));

    //injecte dans modele
    modele.transform = matTransform;

    //Draw normal (position 0, car la matrice s'occupe de tout)
    DrawModel(modele, (Vector3){0, 0, 0}, 1.0f, WHITE);
    if(isSelected){
        DrawModelWires(modele, (Vector3){0, 0, 0}, 1.0f, YELLOW);
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
    static unsigned int compteur_sphere_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_sphere_clones); // nouveau nom
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


void PlaneNode::AppliquerShader(const std::string& nom_shader, Shader le_shader) {
    nom_shader_actuel = nom_shader;
    //assigne le shader à la carte graphique pour ce modèle précis
    modele.materials[0].shader = le_shader; 
}

//std::string GetType() { return "Plan"; }