#include <string>
#include <sstream> //pour std::stringstream
#include <vector>
#include <memory>
#include "raylib.h"
std::string GenererCodeComplet();
//la classe pour representer un objets de la scene
class SceneNode {
    public:
        std::string nom;
        Vector3 position;
        Vector3 rotation;
        Vector3 taille;
        Color couleur;
        bool isSelected;
    
    //virtual parce que on les définies dans les classe qui hérite d'elle
    virtual ~SceneNode() = default;
    virtual void Draw() = 0;
    virtual std::string ToCode() = 0;
    //avant while (!WindowShouldClose())
    virtual std::string GetInitCode() { return ""; }//par défaut TODO à changer

    //dans le BeginMode3D
    virtual std::string GetDrawCode() = 0; 

    //la boucle les unloads
    virtual std::string GetCleanupCode() { return "";}

    //faut faire un constructeur par défaut au cas où
    SceneNode(){
        nom = "";
        position = {0.0f,0.0f,0.0f};
        rotation = {0.0f,0.0f,0.0f};
        taille = {0.0f,0.0f,0.0f};
        couleur = WHITE;
        isSelected = false;
    }
};

//pour representer un cube
class CubeNode : public SceneNode{
    public :
    CubeNode() { 
        nom = "Cube"; 
        taille = {2.0f, 2.0f, 2.0f}; // Petite taille par défaut pour le voir
    }

    void Draw() override {
        //les variables héritées de SceneNode
        DrawCubeV(position, taille, couleur);
        
        if (isSelected) {
            DrawCubeWiresV(position, taille, YELLOW);
        }
    }
    std::string ToCode(){
        return "";
    }
};

//pour representer une sphere
class SphereNode : public SceneNode{
    public :
    SphereNode(){
        nom = "Sphere";
        taille = {2.0f,2.0f,2.0f};
    }
    void Draw() override {
        //les variables héritées de SceneNode
        DrawSphere(position, taille.x,couleur);//je prend taille.x par exemple
        
        if (isSelected) {
            DrawSphereWires(position, taille.x,10,10, YELLOW);//10 par defaut TODO a changer
        }
    }

    std::string ToCode(){
        return "";
    }
};

class CameraNode : public SceneNode{
    public:
        Vector3 target = {0.0f, 0.0f, 0.0f};
        float fovy = 45.0f;//par defaut
        CameraMode mode_camera = CAMERA_FREE;//par defaut
        CameraProjection projetction_cam = CAMERA_PERSPECTIVE;//par defaut
        CameraNode(){
        nom = "camera3D";
    }

    void Draw() override {  
        //on dessine la camera en wireframe comme godot etc
        DrawCubeWires(position, 1.0f, 1.0f, 1.0f, PURPLE);
        DrawLine3D(position, target, PURPLE);
        
        if (isSelected) {
            DrawCubeWires(position, 1.0f, 1.0f, 1.0f, YELLOW);
            DrawLine3D(position, target, YELLOW);
        }
    }
    

    std::string GetDrawCode(){
        //faut ajouter cette ligne dans la boucle de rendu
        std::stringstream code;
        code << "     UpdateCamera(&" << nom << " , " << mode_camera << ");\n";
        return code.str();
    }
    

    std::string GetInitCode(){
        std::stringstream code;
        code << " Camera3D " << nom << " = { 0 };\n";
        code << nom << ".position = {" << position.x << "f, " << position.y << "f, " << position.z << "f};\n";
        code << nom << ".target = {" << target.x << "f, " << target.y << "f, " << target.z << "f};\n";
        code << nom << ".up = { 0.0f, 1.0f, 0.0f };\n";
        code << nom << ".fovy = " << fovy << ";\n";
        code << nom << ".projection = " << projetction_cam << ";\n";
        return code.str();
    }

};

class Camera2DNode : public SceneNode{
    int screenWidth = 1280;
    int screenHeight = 720;
    Vector2 offset_camera = {screenWidth/2,screenHeight/2}; // si je veut faire {screenWidth/2, screenHeight/2}; faut les definir en globales dans le projet
    Vector2 target_camera = {position.x,position.y};//par exemple
    float rotation_camera = rotation.x;
    float     zoom_camera = 1.0f;

    void Draw() override {
        if (!isSelected) {
            //haut gauche vers droite
            DrawLine3D({(screenWidth/2) - taille.x,0.0f,(screenHeight/2) + taille.x}, {(screenWidth/2) + taille.x,0.0f,(screenHeight/2) + taille.x}, PURPLE);
            //haut bas vers droite
            DrawLine3D({(screenWidth/2) - taille.x,0.0f,(screenHeight/2) - taille.x}, {(screenWidth/2) + taille.x,0.0f,(screenHeight/2) - taille.x}, PURPLE);
            //haut gauche vers bas
            DrawLine3D({(screenWidth/2) - taille.x,0.0f,(screenHeight/2) + taille.x}, {(screenWidth/2) - taille.x,0.0f,(screenHeight/2) + taille.x}, PURPLE);
            //haut droit vers bas
            DrawLine3D({(screenWidth/2) + taille.x,0.0f,(screenHeight/2) + taille.x}, {(screenWidth/2) + taille.x,0.0f,(screenHeight/2) + taille.x}, PURPLE);
        }else{
            //haut gauche vers droite
            DrawLine3D({(screenWidth/2) - taille.x,0.0f,(screenHeight/2) + taille.x}, {(screenWidth/2) + taille.x,0.0f,(screenHeight/2) + taille.x}, YELLOW);
            //haut bas vers droite
            DrawLine3D({(screenWidth/2) - taille.x,0.0f,(screenHeight/2) - taille.x}, {(screenWidth/2) + taille.x,0.0f,(screenHeight/2) - taille.x}, YELLOW);
            //haut gauche vers bas
            DrawLine3D({(screenWidth/2) - taille.x,0.0f,(screenHeight/2) + taille.x}, {(screenWidth/2) - taille.x,0.0f,(screenHeight/2) + taille.x}, YELLOW);
            //haut droit vers bas
            DrawLine3D({(screenWidth/2) + taille.x,0.0f,(screenHeight/2) + taille.x}, {(screenWidth/2) + taille.x,0.0f,(screenHeight/2) + taille.x}, YELLOW);

        }
    }
};
