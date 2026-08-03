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
        CameraMode mode_camera = CAMERA_FREE;//par défaut
    CameraNode(){
        nom = "camera3D";
    }
    std::string GetDrawCode(){
        std::stringstream code;
        //on dessine la camera en wireframe comme godot etc
        code << "            DrawCubeWires(position, 1.0f, 1.0f, 1.0f, PURPLE);\n";
        code << "            DrawLine3D(position, target, PURPLE);\n";        
        return code.str();
    }

    std::string GetInitCode(){
        std::stringstream code;
        code << " Camera3D camera = { 0 };\n";
        code << " camera.position = " << position << ";\n";
        code << " camera.target = " << target << ";\n";
        code << " camera.up = { 0.0f, 1.0f, 0.0f };\n";
        code << " camera.fovy = " << fovy << ";\n";
        code << " camera.projection = " << mode_camera << ";\n";
    }

};

class Camera2DNode : public SceneNode{
    
}
