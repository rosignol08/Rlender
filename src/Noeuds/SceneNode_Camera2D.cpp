#include "SceneNode.h"

// pour camera2D
Camera2DNode::Camera2DNode()
{
    nom = "camera2D";
    type = "camera2D";
}

void Camera2DNode::Draw(EditorContext& variables)
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
