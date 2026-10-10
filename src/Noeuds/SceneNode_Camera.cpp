#include "SceneNode.h"

// pour la camera
CameraNode::CameraNode(){
    nom = "camera3D";
    type = "Camera3D";
}

void CameraNode::Draw(EditorContext& variables) {
    Matrix matRot = MatrixRotateXYZ((Vector3){ 
        rotation.x * DEG2RAD, 
        rotation.y * DEG2RAD, 
        rotation.z * DEG2RAD 
    });

    Vector3 direction_defaut = { 0.0f, 0.0f, -1.0f }; 
    Vector3 direction_actuelle = Vector3Transform(direction_defaut, matRot);
    this->target = Vector3Add(position, Vector3Scale(direction_actuelle, 3.0f));

    Color couleurAffichage = isSelected ? YELLOW : PURPLE;

    //dessin de la cam
    rlPushMatrix();
        rlTranslatef(position.x, position.y, position.z);
        rlMultMatrixf(MatrixToFloat(matRot)); 
        
        //les dimensions de la pyramide
        float w = 0.6f;  //largeur
        float h = 0.4f;  //hauteur
        float d = -1.0f; //profondeur

        //les 5 points de la pyramide
        Vector3 lens = { 0.0f, 0.0f, 0.0f }; //objectif (la pointe)
        Vector3 hd   = {  w,  h, d };        //haut-Droite
        Vector3 bd   = {  w, -h, d };        //bas-Droite
        Vector3 bg   = { -w, -h, d };        //bas-Gauche
        Vector3 hg   = { -w,  h, d };        //haut-Gauche

        //base rectangulaire (ecran)
        DrawLine3D(hd, bd, couleurAffichage);
        DrawLine3D(bd, bg, couleurAffichage);
        DrawLine3D(bg, hg, couleurAffichage);
        DrawLine3D(hg, hd, couleurAffichage);

        //liaison 4 coins a ecrant (pointe)
        DrawLine3D(lens, hd, couleurAffichage);
        DrawLine3D(lens, bd, couleurAffichage);
        DrawLine3D(lens, bg, couleurAffichage);
        DrawLine3D(lens, hg, couleurAffichage);

        //pour indiquer l'orientation
        Vector3 top1 = { -0.2f, h, d };
        Vector3 top2 = {  0.2f, h, d };
        Vector3 top3 = {  0.0f, h + 0.3f, d };
        DrawLine3D(top1, top2, couleurAffichage);
        DrawLine3D(top2, top3, couleurAffichage);
        DrawLine3D(top3, top1, couleurAffichage);

    rlPopMatrix();
    // ----------------------------

    //ligne de visée centrale
    DrawLine3D(position, this->target, couleurAffichage);
    DrawSphereWires(this->target, 0.1f, 4, 4, couleurAffichage);
    if (isSelected) {
        DrawBoundingBox(GetBoiteCollision(), LIME);
    }
}

Camera3D CameraNode::ObtenirCameraRaylib() const {
    Camera3D cam = { 0 };
    cam.position = this->position;
    cam.target = this->target;
    cam.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // L'axe vertical
    cam.fovy = this->fovy;
    cam.projection = this->projetction_cam; // CAMERA_PERSPECTIVE ou CAMERA_ORTHOGRAPHIC
    return cam;
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

BoundingBox CameraNode::GetBoiteCollision() {
    Vector3 min = { position.x - 0.5f, position.y - 0.5f, position.z - 0.5f };
    Vector3 max = { position.x + 0.5f, position.y + 0.5f, position.z + 0.5f };

    return BoundingBox{ min, max };
}

std::unique_ptr<SceneNode> CameraNode::Cloner(){
    auto clone = std::make_unique<CameraNode>(*this);
    clone->isSelected = true;
    static unsigned int compteur_camera_clones = 0;                        // logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_camera_clones); // nouveau nom
    return clone;
}

//std::string GetType() { return "Camera"; }