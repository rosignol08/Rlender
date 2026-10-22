#include "SceneNode.h"

ModeleNode::ModeleNode(){
    nom = "Modele";
    taille = {2.0f, 2.0f, 2.0f}; // Petite taille par défaut pour le voir
    type = "ModeleNode";
    chemin_modele = "";
    //ça contient le Material par du modele
    //modele = LoadModel(nom_model.c_str());
}

ModeleNode::~ModeleNode()
{
    // faut decharger le modele
    //UnloadModel(modele);
}

void ModeleNode::ChargerFichier3D(std::string chemin, ManagerModele* manager){
    this->modele = manager->Charger_modele(chemin);
}

std::string ModeleNode::GetDrawCode(){
    std::stringstream code;
    code << "\t\t\t\t\t\t\t\tDrawModele(" << "pos_" << this->nom << ", " << std::to_string(this->taille.x) << ", " << std::to_string(this->taille.y) << ", " << std::to_string(this->taille.z) << ", " << "{ " << std::to_string(this->couleur.r) << ", " << std::to_string(this->couleur.g) << ", " << std::to_string(this->couleur.b) << ", 255" << "}" << ");\n";
    return code.str();
}

std::string ModeleNode::GetInitCode(EditorContext& variables)
{
    std::stringstream code;
    code << "\t\t\t\tVector3 " << "pos_" << this->nom << " = { " << std::to_string(this->position.x) << ", " << std::to_string(this->position.y) << ", " << std::to_string(this->position.z) << "};\n";
    return code.str();
}

void ModeleNode::Draw(EditorContext& variables){
    if (this->modele == nullptr){
        DrawCubeWires(position, taille.x, taille.y, taille.z, GRAY);
        return;
    }   
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

                Texture2D texture_a_appliquer = variables.banque_materiaux->GetMateriau(0)->textureAlbedo;
                if (mat->textureAlbedo.id != 0) {
                    texture_a_appliquer = mat->textureAlbedo;
                }

                Shader shader_a_appliquer = modele->materials[0].shader; //valeur de secours
                if (variables.gestionnaire_shaders != nullptr) {
                    if (variables.gestionnaire_shaders->dictionnaire_shaders.count(mat->nom_shader_actuel) > 0) {
                        shader_a_appliquer = variables.gestionnaire_shaders->dictionnaire_shaders[mat->nom_shader_actuel];
                    }
                }

                for (int i = 0; i < modele->materialCount; i++) {
                //appliquation du shader defaut
                modele->materials[i].shader = shader_a_appliquer;
                
                //ecrasement texture si on a un matériau custom (ID différent de 0)
                if (this->id_materiau != 0) {
                    if (mat->textureAlbedo.id != 0) {
                        modele->materials[i].maps[MATERIAL_MAP_ALBEDO].texture = mat->textureAlbedo;
                    } else {
                        //si le matériau perso a pas de texture du blanc pour masquer l'ancienne texture
                        modele->materials[i].maps[MATERIAL_MAP_ALBEDO].texture = variables.banque_materiaux->GetMateriau(0)->textureAlbedo;
                    }
                }
            }
                
                //TODO ajouter les autre types de textures
            }
        
        AppliquerVariablesGlobales(variables, modele->materials[0].shader);//fonction utilitaire de gestion des uniformes de shaders
        Matrix matTransform = MatrixIdentity();
        matTransform = MatrixMultiply(matTransform, MatrixScale(taille.x, taille.y, taille.z));
        matTransform = MatrixMultiply(matTransform, MatrixRotateXYZ((Vector3){rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD}));
        matTransform = MatrixMultiply(matTransform, MatrixTranslate(position.x, position.y, position.z));

        //injecte dans modele
        modele->transform = matTransform;

        //Draw normal (position 0, car la matrice s'occupe de tout)
        DrawModel(*modele, (Vector3){0, 0, 0}, 1.0f, couleur_rendu);
        if(isSelected){
            DrawModelWires(*modele, (Vector3){0, 0, 0}, 1.0f, YELLOW);
        }
    }
}

BoundingBox ModeleNode::GetBoiteCollision(){
    if(modele == nullptr){
        Vector3 min, max;
        BoundingBox ma_bounding_box;

        min.x = position.x - taille.x/2.0f;
        min.y = position.y - taille.y/2.0f;
        min.z = position.z - taille.z/2.0f;

        max.x = position.x + taille.x/2.0f;
        max.y = position.y + taille.y/2.0f;
        max.z = position.z + taille.z/2.0f;
        ma_bounding_box.max = max;
        ma_bounding_box.min = min;
        return ma_bounding_box;   
    }
    return GetModelBoundingBox(*modele);
}

std::unique_ptr<SceneNode> ModeleNode::Cloner(){
    auto clone = std::make_unique<ModeleNode>(); // faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->rotation = this->rotation;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->id_materiau = this->id_materiau;
    clone->chemin_modele = this->chemin_modele;
    clone->modele = this->modele;
    clone->isSelected = true;
    static unsigned int compteur_Modele_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Modele_clones); // nouveau nom
    compteur_Modele_clones++;
    return clone;
}

