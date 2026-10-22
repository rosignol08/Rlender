#include "SceneNode.h"

// Sphere
SphereNode::SphereNode(){
    nom = "Sphere";
    taille = {2.0f, 2.0f, 2.0f};
    type = "sphere";
    // ça contient automatiquement un Material par défaut
    modele = LoadModelFromMesh(GenMeshSphere(1.0f, 16.0f, 16.0f));
}

SphereNode::~SphereNode(){
    // faut decharger le modele
    UnloadModel(modele);
}

void SphereNode::Draw(EditorContext& variables){
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

std::string CubeNode::GetDrawCode(){
    std::stringstream code;
    code << "            DrawModel(model_" << this->nom << ", pos_" << this->nom << ", 1.0f, WHITE);\n";
    return code.str();
}

std::string CubeNode::GetInitCode(EditorContext& variables){
    std::stringstream code;
    //recuperation nom materiel
    DataMateriel* mat = variables.banque_materiaux->GetMateriau(this->id_materiau);
    std::string nom_mat = mat->nom;

    //la pos
    code << "    Vector3 pos_" << this->nom << " = { "<< this->position.x << "f, " << this->position.y << "f, " << this->position.z << "f };\n";
    
    //gen modele 3D
    code << "    Model model_" << this->nom << " = LoadModelFromMesh(GenMeshCube("<< this->taille.x << "f, " << this->taille.y << "f, " << this->taille.z << "f));\n";

    //couleur genre "model_Cube_1.materials[0]...color = couleur_Bois;"
    code << "    model_" << this->nom << ".materials[0].maps[MATERIAL_MAP_ALBEDO].color = couleur_" << nom_mat << ";\n";
    
    //texture
    if (mat->chemins_textures.count("albedo") > 0) {
        code << "    model_" << this->nom << ".materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture_" << nom_mat << ";\n";
    }

    //shader
    if (!mat->nom_shader_actuel.empty()) {
        code << "    model_" << this->nom << ".materials[0].shader = shader_" << mat->nom_shader_actuel << ";\n";
    }

    return code.str();
}

std::string CubeNode::GetCleanupCode() {
    std::stringstream code;
    code << "    UnloadModel(model_"<< this->nom<<");\n";
    return code.str();
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
    clone->id_materiau = this->id_materiau;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    static unsigned int compteur_sphere_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_sphere_clones); // nouveau nom
    return clone;
}