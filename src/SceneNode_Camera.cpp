#include "SceneNode.h"

// pour la camera
CameraNode::CameraNode(){
    nom = "camera3D";
    type = "camera3D";
}

void CameraNode::Draw(EditorContext& variables){
    // on dessine la camera en wireframe comme godot etc
    DrawCubeWires(position, 1.0f, 1.0f, 1.0f, PURPLE);
    DrawLine3D(position, target, PURPLE);

    if (isSelected)
    {
        DrawCubeWires(position, 1.0f, 1.0f, 1.0f, YELLOW);
        DrawLine3D(position, target, YELLOW);
    }
}

std::string CameraNode::ToCode(){
    return "";
}

std::string CameraNode::GetDrawCode(){
    // faut ajouter cette ligne dans la boucle de rendu
    std::stringstream code;
    code << "     UpdateCamera(&" << nom << " , " << mode_camera << ");\n";
    return code.str();
}

std::string CameraNode::GetInitCode(){
    std::stringstream code;
    code << " Camera3D " << nom << " = { 0 };\n";
    code << nom << ".position = {" << position.x << "f, " << position.y << "f, " << position.z << "f};\n";
    code << nom << ".target = {" << target.x << "f, " << target.y << "f, " << target.z << "f};\n";
    code << nom << ".up = { 0.0f, 1.0f, 0.0f };\n";
    code << nom << ".fovy = " << fovy << ";\n";
    code << nom << ".projection = " << projetction_cam << ";\n";
    return code.str();
}

BoundingBox CameraNode::GetBoiteCollision(){
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

std::unique_ptr<SceneNode> CameraNode::Cloner(){
    auto clone = std::make_unique<CameraNode>(*this);
    clone->isSelected = true;
    static unsigned int compteur_camera_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_camera_clones); // nouveau nom
    return clone;
}

std::string GetType() { return "Camera"; }