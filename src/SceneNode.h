#include <string>
#include <sstream> //pour std::stringstream
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "raylib.h"
#pragma once

//la classe pour representer un objets de la scene
class SceneNode {
    public:
        std::string nom;
        Vector3 position;
        Vector3 rotation;
        Vector3 taille;
        Color couleur;
        bool isSelected;
        std::string type; //pour pouvoir savoir à quoi on a affaire pour la sauvgarde
    
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
        type = "cube";
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
    std::string GetDrawCode(){
        return "";
    }
    std::string GetInitCode(){
        return "";
    }
};

//pour representer une sphere
class SphereNode : public SceneNode{
    public :
    SphereNode(){
        nom = "Sphere";
        taille = {2.0f,2.0f,2.0f};
        type = "sphere";
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
    std::string GetDrawCode(){
        return "";
    }
    std::string GetInitCode(){
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
        type = "camera3D";
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
    std::string ToCode(){
        return "";
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
    public :
        int screenWidth = 1280.0f;
        int screenHeight = 720.0f;
        Vector2 offset_camera = {screenWidth/2.0f,screenHeight/2.0f}; // si je veut faire {screenWidth/2, screenHeight/2}; faut les definir en globales dans le projet
        //Vector2 target_camera = {position.x,position.y};//par exemple
        //float rotation_camera = rotation.x;
        float zoom_camera = 1.0f;
        Camera2DNode(){
            nom = "camera2D";
            type = "camera2D";
        }

    void Draw() override {
        Color couleurLigne = isSelected ? YELLOW : PURPLE;

        //les 4 coins du cadre autour de la pos 3D du noeud
        Vector3 p1 = {position.x - taille.x, position.y, position.z - taille.y};
        Vector3 p2 = {position.x + taille.x, position.y, position.z - taille.y};
        Vector3 p3 = {position.x + taille.x, position.y, position.z + taille.y};
        Vector3 p4 = {position.x - taille.x, position.y, position.z + taille.y};

        //le rectangle
        DrawLine3D(p1, p2, couleurLigne); //haut
        DrawLine3D(p3, p4, couleurLigne); //bas
        DrawLine3D(p4, p1, couleurLigne); //gauche
        DrawLine3D(p2, p3, couleurLigne); //droite
    }

    std::string ToCode(){
        return "";
    }

    std::string GetDrawCode(){
        return "";
    }
    
    std::string GetCleanupCode(){
        return "";
    }

    std::string GetInitCode(){
        std::stringstream code;
        code << " Camera2D " << nom << " = { 0 };\n";
        code << nom << ".target = {" << position.x << "f, " << position.y << "f};\n";
        code << nom << ".offset = {" << offset_camera.x << "f, " << offset_camera.y << "f};\n";
        code << nom << ".rotation = " << rotation.x << ";\n";
        code << nom << ".zoom = " << zoom_camera << ";\n";
        return code.str();
    }
};

std::string GenererCodeComplet(const std::vector<std::unique_ptr<SceneNode>>& nodes);