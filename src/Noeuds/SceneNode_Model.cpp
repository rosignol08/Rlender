#include "SceneNode.h"

ModeleNode::ModeleNode(){
    nom = "Modele";
    taille = {2.0f, 2.0f, 2.0f}; // Petite taille par défaut pour le voir
    type = "ModeleNode";
    chemin_modele = "";
    //ça contient le Material par du modele
    //modele = LoadModel(nom_model.c_str());
}

ModeleNode::~ModeleNode(){
    // faut decharger le modele
    //UnloadModel(modele);
}

void ModeleNode::ChargerFichier3D(std::string chemin, ManagerModele* manager){
    this->chemin_modele = chemin;
    this->modele = manager->Charger_modele(chemin);
}

std::string ModeleNode::GetDrawCode(){
    std::stringstream code;
    if(chemin_modele != ""){
        std::string nom_model = FormaterNomModele(chemin_modele);
        code << "            DrawModelEx(" << nom_model
        << ", pos_" << nom
        << ", Vector3{0.0f, 1.0f, 0.0f}, 0.0f, "
        << "Vector3{" << std::to_string(taille.x) << "f, "
                      << std::to_string(taille.y) << "f, "
                      << std::to_string(taille.z) << "f}, "
        << "Color{" << (int)couleur.r << ", " << (int)couleur.g << ", " << (int)couleur.b << ", 255});\n";
        return code.str();
    }
    return "";
}

std::string ModeleNode::GetInitCode(EditorContext& variables){
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
        modele->transform = GetMatriceMonde();
        //matTransform;

        //Draw normal (position 0, car la matrice s'occupe de tout)
        DrawModel(*modele, (Vector3){0, 0, 0}, 1.0f, couleur_rendu);
        if(isSelected){
            DrawModelWires(*modele, (Vector3){0, 0, 0}, 1.0f, YELLOW);
        }
    }
}

BoundingBox ModeleNode::GetBoiteLocale() const {
    if (modele == nullptr || modele->meshCount == 0) {
        return BoundingBox{ {-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f} };
    }

    BoundingBox box = GetMeshBoundingBox(modele->meshes[0]);
    for (int i = 1; i < modele->meshCount; i++) {
        BoundingBox b = GetMeshBoundingBox(modele->meshes[i]);
        box.min = Vector3Min(box.min, b.min);
        box.max = Vector3Max(box.max, b.max);
    }
    return box;
}

BoundingBox ModeleNode::GetBoiteCollision() {
    BoundingBox lb = GetMeshBoundingBox(modele->meshes[0]); // locale, sans transform
    Matrix world = GetMatriceMonde();

    Vector3 corners[8] = {
        {lb.min.x, lb.min.y, lb.min.z}, {lb.min.x, lb.min.y, lb.max.z},
        {lb.min.x, lb.max.y, lb.min.z}, {lb.min.x, lb.max.y, lb.max.z},
        {lb.max.x, lb.min.y, lb.min.z}, {lb.max.x, lb.min.y, lb.max.z},
        {lb.max.x, lb.max.y, lb.min.z}, {lb.max.x, lb.max.y, lb.max.z}
    };

    Vector3 wmin = Vector3Transform(corners[0], world);
    Vector3 wmax = wmin;
    for (int i = 1; i < 8; i++) {
        Vector3 p = Vector3Transform(corners[i], world);
        wmin = Vector3Min(wmin, p);
        wmax = Vector3Max(wmax, p);
    }
    return { wmin, wmax };
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

