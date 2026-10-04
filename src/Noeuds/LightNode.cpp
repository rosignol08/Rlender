#include "LightNode.h"

LightNode::LightNode(){
    position = {0.0f, 0.0f, 0.0f};
    couleur_lumiere = {1.0f, 1.0f, 1.0f};
    intensite = 1.1;
    puissance = 5.0f;
    taille = 1.0f;
}

void LightNode::Update(EditorContext& variables){
    //limite de 4 lumières differentes
    if (!est_allume) return;
    if (variables.compteur_lumieres >= 4) return; 

    //tableau des lumières "[0]" etc
    std::string i = "[" + std::to_string(variables.compteur_lumieres) + "]";
    variables.variables_globales["lightPos" + i] = this->position;
    variables.variables_globales["lightColor" + i] = this->couleur_lumiere;
    variables.variables_globales["lightIntensity" + i] = this->intensite;
    variables.variables_globales["lightPower" + i] = this->puissance;

    variables.compteur_lumieres++;
    
    //maj du nombre total de lumières pour le shader
    variables.variables_globales["lightCount"] = variables.compteur_lumieres;
}


void LightNode::Draw(EditorContext& variables){
    DrawSphereWires(position, 0.5f,16,16, YELLOW);
    if (this->isSelected) {
        DrawSphereWires(position, intensite, 16, 16, Fade(YELLOW, 0.3f));
    }
}

std::unique_ptr<SceneNode> LightNode::Cloner(){
    auto clone = std::make_unique<LightNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->intensite = this->intensite;
    clone->puissance = this->puissance;
    clone->est_allume = this->est_allume;
    clone->isSelected = true;
    static unsigned int compteur_Light_clones = 0; //logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Light_clones); // nouveau nom
    compteur_Light_clones++;
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

BoundingBox LightNode::GetBoiteCollision(){
    Vector3 min, max;
    BoundingBox ma_bounding_box;
    float rayon_selection = 0.5f;
    min.x = position.x - rayon_selection;
    min.y = position.y - rayon_selection;
    min.z = position.z - rayon_selection;
    max.x = position.x + rayon_selection;
    max.y = position.y + rayon_selection;
    max.z = position.z + rayon_selection;

    ma_bounding_box.min = min;
    ma_bounding_box.max = max;
    return ma_bounding_box;
}

