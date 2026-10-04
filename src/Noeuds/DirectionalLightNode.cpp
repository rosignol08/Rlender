#include "DirectionalLightNode.h"

SoleilNode::SoleilNode(){
    position = {0.0f, 0.0f, 0.0f};
    direction = {0.0f, -1.0f, 0.0f};
    couleur_lumiere = {1.0f, 1.0f, 1.0f};
    puissance = 1.0f;
    est_allume = true;
}

void SoleilNode::Update(EditorContext& variables){
    if (!est_allume) {
        //eteint puissance = 0
        variables.variables_globales["sunPower"] = 0.0f;
        return;
    }

    //soleil : PAS de compteur_lumieres
    //variables uniques.
    variables.variables_globales["sunDirection"] = this->direction;
    variables.variables_globales["sunColor"] = this->couleur_lumiere;
    variables.variables_globales["sunPower"] = this->puissance;
}


void SoleilNode::Draw(EditorContext& variables){
    //TODO dessiner un triangle ou une fleche qui pointe vers la direction du noeud 
    DrawSphereWires(position, 0.5f,16,16, YELLOW);
    float longueur_direction = Vector3Length(direction);
    if (longueur_direction > 0.001f) {
        
        // 3. On normalise (ramène la longueur à 1)
        Vector3 dir_norm = Vector3Normalize(direction);
        
        // 4. On calcule le point final de la ligne (ex: longueur de 3.0 unités)
        // fin = position + (direction * 3.0)
        Vector3 fin_ligne = Vector3Add(position, Vector3Scale(dir_norm, 3.0f));

        // 5. On dessine la ligne qui montre où la lumière frappe !
        DrawLine3D(position, fin_ligne, YELLOW);

        // Optionnel : Si l'objet est sélectionné, on dessine une petite "pointe" rouge 
        // au bout de la ligne pour bien voir le sens de la flèche
        if (this->isSelected) {
            DrawSphereWires(fin_ligne, 0.2f, 8, 8, RED);
        }
    }
}

std::unique_ptr<SceneNode> SoleilNode::Cloner(){
    auto clone = std::make_unique<SoleilNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->direction = this->direction;
    clone->puissance = this->puissance;
    clone->est_allume = this->est_allume;
    clone->isSelected = true;
    static unsigned int compteur_Soleil_clones = 0; //logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Soleil_clones); // nouveau nom
    compteur_Soleil_clones++;
    return clone;
}

std::string SoleilNode::ToCode(){
    return "";
}

std::string SoleilNode::GetDrawCode(){
    return "";
}

std::string SoleilNode::GetInitCode(){
    return "";
}

BoundingBox SoleilNode::GetBoiteCollision(){
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

