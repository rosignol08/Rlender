#include "SceneNode.h"

// Sphere
SphereNode::SphereNode(){
    nom = "Sphere";
    taille = {2.0f, 2.0f, 2.0f};
    rayon = 1.0f;
    slices = 16;
    ring = 16;
    type = "sphere";
    // ça contient automatiquement un Material par défaut
    modele = LoadModelFromMesh(GenMeshSphere(rayon, ring, slices));
}

SphereNode::~SphereNode(){
    // faut decharger le modele
    UnloadModel(modele);
}

void SphereNode::ReconstruireMesh(){
    UnloadModel(modele);
    modele = LoadModelFromMesh(GenMeshSphere(rayon, ring, slices));
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
    AppliquerVariablesGlobales(variables, le_shader);//fonction utilitaire de gestion des uniformes de shaders
    Matrix matTransform = MatrixIdentity();
    matTransform = MatrixMultiply(matTransform, MatrixScale(taille.x, taille.y, taille.z));
    matTransform = MatrixMultiply(matTransform, MatrixRotateXYZ((Vector3){rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD}));
    matTransform = MatrixMultiply(matTransform, MatrixTranslate(position.x, position.y, position.z));

    //injecte dans modele
    modele.transform = GetMatriceMonde();
    //matTransform;

    //Draw normal (position 0, car la matrice s'occupe de tout)
    DrawModel(modele, (Vector3){0, 0, 0}, 1.0f, couleur_rendu);
    if(isSelected){
        DrawModelWires(modele, (Vector3){0, 0, 0}, 1.0f, YELLOW);
    }
}

std::string SphereNode::GetDrawCode(){
    std::stringstream code;
    code << "            DrawModel(model_" << this->nom << ", pos_" << this->nom << ", 1.0f, WHITE);\n";
    return code.str();
}

std::string SphereNode::GetInitCode(EditorContext& variables){
    std::stringstream code;
    //recuperation nom materiel
    DataMateriel* mat = variables.banque_materiaux->GetMateriau(this->id_materiau);
    std::string nom_mat = mat->nom;

    //la pos
    code << "    Vector3 pos_" << this->nom << " = { "<< std::to_string(this->position.x) << "f, " << std::to_string(this->position.y) << "f, " << std::to_string(this->position.z) << "f };\n";
    
    //gen modele 3D
    code << "    Model model_" << this->nom << " = LoadModelFromMesh(GenMeshSphere(" << std::to_string(this->rayon) << "f, 16, 16));\n";

    //couleur genre "model_Cube_1.materials[0]...color = couleur_Bois;"
    code << "    model_" << this->nom << ".materials[0].maps[MATERIAL_MAP_ALBEDO].color = couleur_" << nom_mat << ";\n";
    
    //texture
    if (mat->chemins_textures.count("albedo") > 0) {
        code << "    model_" << this->nom << ".materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture_" << nom_mat << ";\n";
    }

    //shader
    if (!mat->nom_shader_actuel.empty()) {
        std::string nom_shader_cle = mat->nom_shader_actuel;
        std::string nom_shader_var = FormaterNomVariableShader(nom_shader_cle);
        code << "    model_" << this->nom << ".materials[0].shader = " << nom_shader_var << ";\n";
    }

    return code.str();
}

std::string SphereNode::GetCleanupCode() {
    std::stringstream code;
    code << "    UnloadModel(model_"<< this->nom<<");\n";
    return code.str();
}

BoundingBox SphereNode::GetBoiteLocale() const {
    return GetMeshBoundingBox(modele.meshes[0]);
}

BoundingBox SphereNode::GetBoiteCollision() {
    BoundingBox lb = GetMeshBoundingBox(modele.meshes[0]); // locale, sans transform
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