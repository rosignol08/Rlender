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
    Color couleur_rendu = WHITE;//couleur de rendu

    if (variables.banque_materiaux != nullptr) {
        //demande du matériel de cet objet
        DataMateriel* mat = variables.banque_materiaux->GetMateriau(this->id_materiau);
        
        //securite si l'ID existe plus matériel par défaut (0)
        if (mat == nullptr) {
            mat = variables.banque_materiaux->GetMateriau(0);
            this->id_materiau = 0; //l'objet pour la prochaine fois
        }else{
            couleur_rendu = mat->couleurAlbedo;
            
            //TODO ajouter les autre types de textures
            if(mat->textureAlbedo.id != 0){
                modele.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = mat->textureAlbedo;
                //TraceLog(LOG_INFO, "Texture");
            }else{
                modele.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = {0};
            }
            if (variables.gestionnaire_shaders != nullptr) {
                //check le shader dans le dictionnaire
                if (variables.gestionnaire_shaders->dictionnaire_shaders.count(mat->nom_shader_actuel) > 0) {
                    modele.materials[0].shader = variables.gestionnaire_shaders->dictionnaire_shaders[mat->nom_shader_actuel];
                }
            }
        }
    }
    Shader le_shader = modele.materials[0].shader;//pour limiter les acces mémoire
    AppliquerVariablesGlobales(variables, le_shader);//fonction utilitaire de gestion des uniformes de shaders
    Matrix matTransform = MatrixIdentity();
    matTransform = MatrixMultiply(matTransform, MatrixScale(taille.x, taille.y, taille.z));
    matTransform = MatrixMultiply(matTransform, MatrixRotateXYZ((Vector3){rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD}));
    matTransform = MatrixMultiply(matTransform, MatrixTranslate(position.x, position.y, position.z));

    //injecte dans modele
    modele.transform = matTransform;

    //Draw normal (position 0, car la matrice s'occupe de tout)
    DrawModel(modele, (Vector3){0, 0, 0}, 1.0f, couleur_rendu);
    if(isSelected){
        DrawModelWires(modele, (Vector3){0, 0, 0}, 1.0f, YELLOW);
    }
}

BoundingBox ConeNode::GetBoiteCollision(){
    return {{position.x - taille.x, position.y - 0.0f, position.z - taille.z}, // Base au sol
            {position.x + taille.x, position.y + taille.y, position.z + taille.z}};
}
std::unique_ptr<SceneNode> ConeNode::Cloner(){
    auto clone = std::make_unique<ConeNode>(); // faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->id_materiau = this->id_materiau;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    static unsigned int compteur_Cone_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Cone_clones); // nouveau nom
    compteur_Cone_clones++;
    return clone;
}

std::string ConeNode::GetUpdateCode(){
    return "";
}

std::string ConeNode::GetDrawCode(){
    return "";
}

std::string ConeNode::GetInitCode(EditorContext& variables){
    return "";
}