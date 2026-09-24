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
void CubeNode::Draw()
{
    // les variables héritées de SceneNode
    DrawModelEx(modele, position, {0.0f, 1.0f, 0.0f}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawModelWiresEx(modele, position, {0.0f, 1.0f, 0.0f}, 0.0f, taille, YELLOW);
    }
}

std::unique_ptr<SceneNode> CubeNode::Cloner()
{
    auto clone = std::make_unique<CubeNode>(); // faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->isSelected = true;
    static unsigned int compteur_cube_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_cube_clones); // nouveau nom
    return clone;
}

//TODO le faire pour les autres
void CubeNode::AppliquerShader(const std::string& nom_shader, Shader le_shader) {
    nom_shader_actuel = nom_shader;
    //assigne le shader à la carte graphique pour ce modèle précis
    modele.materials[0].shader = le_shader; 
}

// Sphere

SphereNode::SphereNode()
{
    nom = "Sphere";
    taille = {2.0f, 2.0f, 2.0f};
    type = "sphere";
    // un maillage de base
    Mesh maillage = GenMeshSphere(1.0f, 16.0f, 16.0f);
    // ça contient automatiquement un Material par défaut
    modele = LoadModelFromMesh(maillage);
}
SphereNode::~SphereNode()
{
    // faut decharger le modele
    UnloadModel(modele);
}

void SphereNode::Draw()
{
    // les variables héritées de SceneNode
    DrawModelEx(modele, position, {0.0f, 1.0f, 0.0f}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawSphereWires(position, taille.x, 16, 16, YELLOW); // 10 par defaut TODO a changer
    }
}

std::string SphereNode::ToCode()
{
    return "";
}
std::string SphereNode::GetDrawCode()
{
    return "";
}
std::string SphereNode::GetInitCode()
{
    return "";
}

BoundingBox SphereNode::GetBoiteCollision()
{
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

std::unique_ptr<SceneNode> SphereNode::Cloner()
{
    auto clone = std::make_unique<SphereNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->isSelected = true;
    static unsigned int compteur_sphere_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_sphere_clones); // nouveau nom
    return clone;
}

// le plan
PlaneNode::PlaneNode()
{
    nom = "Plane";
    type = "plane";
    taille = {5.0f, 1.0f, 5.0f};
    modele = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 10, 10)); // resolution 10x10
}
PlaneNode::~PlaneNode()
{
    UnloadModel(modele);
}

void PlaneNode::Draw()
{
    DrawModelEx(modele, position, {0, 1, 0}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawModelWiresEx(modele, position, {0, 1, 0}, 0.0f, taille, YELLOW);
    }
}

BoundingBox PlaneNode::GetBoiteCollision()
{
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
    clone->isSelected = true;
    clone->nom = this->nom + "_clone";
    return clone;
}

std::string PlaneNode::ToCode()
{
    return "";
}

std::string PlaneNode::GetDrawCode()
{
    return "";
}

std::string PlaneNode::GetInitCode()
{
    return "";
}

// cylindre
CylinderNode::CylinderNode()
{
    nom = "Cylinder";
    type = "cylinder";
    taille = {1.0f, 2.0f, 1.0f};
    modele = LoadModelFromMesh(GenMeshCylinder(1.0f, 1.0f, 16));
}
CylinderNode::~CylinderNode()
{
    UnloadModel(modele);
}

void CylinderNode::Draw()
{
    DrawModelEx(modele, position, {0, 1, 0}, 0.0f, taille, couleur);
    if (isSelected)
    {
        DrawModelWiresEx(modele, position, {0, 1, 0}, 0.0f, taille, YELLOW);
    }
}

BoundingBox CylinderNode::GetBoiteCollision()
{
    return {{position.x - taille.x, position.y - taille.y / 2, position.z - taille.z},
            {position.x + taille.x, position.y + taille.y / 2, position.z + taille.z}};
}
std::unique_ptr<SceneNode> CylinderNode::Cloner()
{
    auto clone = std::make_unique<CylinderNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->isSelected = true;
    clone->nom = this->nom + "_clone";
    return clone;
}

std::string CylinderNode::ToCode()
{
    return "";
}
std::string CylinderNode::GetDrawCode()
{
    return "";
}

std::string CylinderNode::GetInitCode()
{
    return "";
}

// cone
// pour representer un cone
ConeNode::ConeNode()
{
    nom = "Cone";
    type = "cone";
    taille = {1.0f, 2.0f, 1.0f};
    modele = LoadModelFromMesh(GenMeshCone(1.0f, 1.0f, 16));
}

ConeNode::~ConeNode()
{
    UnloadModel(modele);
}

void ConeNode::Draw()
{
    DrawModelEx(modele, position, {0, 1, 0}, 0.0f, taille, couleur);
    if (isSelected)
        DrawModelWiresEx(modele, position, {0, 1, 0}, 0.0f, taille, YELLOW);
}

BoundingBox ConeNode::GetBoiteCollision()
{
    return {{position.x - taille.x, position.y - 0.0f, position.z - taille.z}, // Base au sol
            {position.x + taille.x, position.y + taille.y, position.z + taille.z}};
}
std::unique_ptr<SceneNode> ConeNode::Cloner()
{
    auto clone = std::make_unique<ConeNode>();
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->isSelected = true;
    clone->nom = this->nom + "_clone";
    return clone;
}
std::string ConeNode::ToCode()
{
    return "";
}

std::string ConeNode::GetDrawCode()
{
    return "";
}

std::string ConeNode::GetInitCode()
{
    return "";
}

// pour la camera
CameraNode::CameraNode()
{
    nom = "camera3D";
    type = "camera3D";
}
void CameraNode::Draw()
{
    // on dessine la camera en wireframe comme godot etc
    DrawCubeWires(position, 1.0f, 1.0f, 1.0f, PURPLE);
    DrawLine3D(position, target, PURPLE);

    if (isSelected)
    {
        DrawCubeWires(position, 1.0f, 1.0f, 1.0f, YELLOW);
        DrawLine3D(position, target, YELLOW);
    }
}
std::string CameraNode::ToCode()
{
    return "";
}

std::string CameraNode::GetDrawCode()
{
    // faut ajouter cette ligne dans la boucle de rendu
    std::stringstream code;
    code << "     UpdateCamera(&" << nom << " , " << mode_camera << ");\n";
    return code.str();
}

std::string CameraNode::GetInitCode()
{
    std::stringstream code;
    code << " Camera3D " << nom << " = { 0 };\n";
    code << nom << ".position = {" << position.x << "f, " << position.y << "f, " << position.z << "f};\n";
    code << nom << ".target = {" << target.x << "f, " << target.y << "f, " << target.z << "f};\n";
    code << nom << ".up = { 0.0f, 1.0f, 0.0f };\n";
    code << nom << ".fovy = " << fovy << ";\n";
    code << nom << ".projection = " << projetction_cam << ";\n";
    return code.str();
}

BoundingBox CameraNode::GetBoiteCollision()
{
    Vector3 min, max;
    BoundingBox ma_bounding_box;

    min.x = position.x - taille.x / 2.0f;
    min.y = position.y - taille.y / 2.0f;
    min.z = position.z - taille.z / 2.0f;

    max.x = position.x + taille.x / 2.0f;
    max.y = position.y + taille.y / 2.0f;
    max.z = position.z + taille.z / 2.0f;
    ma_bounding_box.max = max;
    ma_bounding_box.min = min;
    return ma_bounding_box;
}
std::unique_ptr<SceneNode> CameraNode::Cloner()
{
    auto clone = std::make_unique<CameraNode>(*this);
    clone->isSelected = true;
    static unsigned int compteur_camera_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_camera_clones); // nouveau nom
    return clone;
}

// pour camera2D
Camera2DNode::Camera2DNode()
{
    nom = "camera2D";
    type = "camera2D";
}

void Camera2DNode::Draw()
{
    Color couleurLigne = isSelected ? YELLOW : PURPLE;

    // les 4 coins du cadre autour de la pos 3D du noeud
    Vector3 p1 = {position.x - taille.x, position.y, position.z - taille.y};
    Vector3 p2 = {position.x + taille.x, position.y, position.z - taille.y};
    Vector3 p3 = {position.x + taille.x, position.y, position.z + taille.y};
    Vector3 p4 = {position.x - taille.x, position.y, position.z + taille.y};

    // le rectangle
    DrawLine3D(p1, p2, couleurLigne); // haut
    DrawLine3D(p3, p4, couleurLigne); // bas
    DrawLine3D(p4, p1, couleurLigne); // gauche
    DrawLine3D(p2, p3, couleurLigne); // droite
}

std::string Camera2DNode::ToCode()
{
    return "";
}

std::string Camera2DNode::GetDrawCode()
{
    return "";
}

std::string Camera2DNode::GetCleanupCode()
{
    return "";
}

std::string Camera2DNode::GetInitCode()
{
    std::stringstream code;
    code << " Camera2D " << nom << " = { 0 };\n";
    code << nom << ".target = {" << position.x << "f, " << position.y << "f};\n";
    code << nom << ".offset = {" << offset_camera.x << "f, " << offset_camera.y << "f};\n";
    code << nom << ".rotation = " << rotation.x << ";\n";
    code << nom << ".zoom = " << zoom_camera << ";\n";
    return code.str();
}
std::unique_ptr<SceneNode> Camera2DNode::Cloner()
{
    auto clone = std::make_unique<Camera2DNode>(*this);
    clone->isSelected = true;
    static unsigned int compteur_camera2D_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_camera2D_clones); // nouveau nom
    return clone;
}
