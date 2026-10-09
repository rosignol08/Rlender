#include "SceneNode.h"

CubeNode::CubeNode()
{
    nom = "Cube";
    taille = {2.0f, 2.0f, 2.0f}; // Petite taille par défaut pour le voir
    type = "cube";
    // un maillage de base
    Mesh maillage = GenMeshCube(1.0f, 1.0f, 1.0f);
    // ça contient automatiquement un Material par défaut
    modele = LoadModelFromMesh(maillage);
}

CubeNode::~CubeNode()
{
    // faut decharger le modele
    UnloadModel(modele);
}

std::string CubeNode::GetDrawCode()
{
    std::stringstream code;
    code << "\t\t\t\t\t\t\t\tDrawCube(" << "pos_" << this->nom << ", " << std::to_string(this->taille.x) << ", " << std::to_string(this->taille.y) << ", " << std::to_string(this->taille.z) << ", " << "{ " << std::to_string(this->couleur.r) << ", " << std::to_string(this->couleur.g) << ", " << std::to_string(this->couleur.b) << ", 255" << "}" << ");\n";
    return code.str();
}

std::string CubeNode::GetInitCode()
{
    std::stringstream code;
    code << "\t\t\t\tVector3 " << "pos_" << this->nom << " = { " << std::to_string(this->position.x) << ", " << std::to_string(this->position.y) << ", " << std::to_string(this->position.z) << "};\n";
    return code.str();
}
void CubeNode::Draw(EditorContext& variables){
    Color couleur_rendu = WHITE;//couleur de rendu

    if (variables.banque_materiaux != nullptr) {
        //demande du matériel de cet objet
        DataMateriel* mat = variables.banque_materiaux->GetMateriau(this->id_materiau);
        
        //securite si l'ID existe plus matériel par défaut (0)
        if (mat == nullptr) {
            mat = variables.banque_materiaux->GetMateriau(0);
            this->id_materiau = 0; //l'objet pour la prochaine fois
        }
        
        if (mat != nullptr) {
            couleur_rendu = mat->couleurAlbedo;
            
            //plus tard :
            //modele.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = mat->textureAlbedo;
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
    DrawModel(modele, (Vector3){0, 0, 0}, 1.0f, WHITE);
    if(isSelected){
        DrawModelWires(modele, (Vector3){0, 0, 0}, 1.0f, YELLOW);
    }
}

std::unique_ptr<SceneNode> CubeNode::Cloner(){
    auto clone = std::make_unique<CubeNode>(); // faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->nom_shader_actuel = this->nom_shader_actuel;
    clone->modele.materials[0].shader = this->modele.materials[0].shader; // on clone aussi le shader et le materiel
    clone->isSelected = true;
    static unsigned int compteur_cube_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_cube_clones); // nouveau nom
    compteur_cube_clones++;
    return clone;
}

//TODO le faire pour les autres
void CubeNode::AppliquerShader(const std::string& nom_shader, Shader le_shader) {
    nom_shader_actuel = nom_shader;
    //assigne le shader à la carte graphique pour ce modèle précis
    modele.materials[0].shader = le_shader; 
}

//std::string GetType() { return "CubeNode"; }