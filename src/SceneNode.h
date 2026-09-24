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
        virtual BoundingBox GetBoiteCollision(){
            //pour la selectioner avec la souris l'objet a une bounding box
            return BoundingBox{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
        }
        std::string nom_shader_actuel = "Defaut";
    
    //virtual parce que on les définies dans les classe qui hérite d'elle
    virtual ~SceneNode() = default;
    virtual void Draw() = 0;
    virtual std::string ToCode() = 0;
    //avant while (!WindowShouldClose())
    virtual std::string GetInitCode() { return ""; }//par défaut TODO à changer

    //dans le BeginMode3D
    virtual std::string GetDrawCode() = 0;

    //pour dupliquer
    virtual std::unique_ptr<SceneNode> Cloner() = 0;

    //la boucle les unloads
    virtual std::string GetCleanupCode() { return "";}
    //le shader
    virtual void AppliquerShader(const std::string& nom_shader, Shader le_shader) {
        nom_shader_actuel = nom_shader;
    }
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
    private:
        Model modele;
    public :
    CubeNode();

    ~CubeNode() override;

    void Draw() override;
    std::string ToCode(){
        return "";
    }
    std::string GetDrawCode() override;
    
    std::string GetInitCode() override;
    
    BoundingBox GetBoiteCollision() override {
        Vector3 min, max;
        BoundingBox ma_bounding_box;

        min.x = position.x - taille.x/2.0f;
        min.y = position.y - taille.y/2.0f;
        min.z = position.z - taille.z/2.0f;

        max.x = position.x + taille.x/2.0f;
        max.y = position.y + taille.y/2.0f;
        max.z = position.z + taille.z/2.0f;
        ma_bounding_box.max = max;
        ma_bounding_box.min = min;
        return ma_bounding_box;
    }

    std::unique_ptr<SceneNode> Cloner() override;
    void AppliquerShader(const std::string& nom_shader, Shader le_shader) override;
};

//pour representer une sphere
class SphereNode : public SceneNode{
    private:
        Model modele;
    public :
    SphereNode();
    ~SphereNode() override;

    void Draw() override;

    std::string ToCode()override;
    std::string GetDrawCode()override;
    std::string GetInitCode()override;

    BoundingBox GetBoiteCollision() override;
    
    std::unique_ptr<SceneNode> Cloner() override;
};


//pour representer un plan (sol)
class PlaneNode : public SceneNode {
private:
    Model modele;
public:
    PlaneNode();
    ~PlaneNode() override ;

    void Draw() override;
    
    BoundingBox GetBoiteCollision() override;
    std::unique_ptr<SceneNode> Cloner() override;
    std::string ToCode() override;
    std::string GetDrawCode() override;
    std::string GetInitCode() override;
};

//pour representer un cylindre
class CylinderNode : public SceneNode {
    private:
        Model modele;
    public:
        CylinderNode();
        ~CylinderNode() override;

        void Draw() override ;

        BoundingBox GetBoiteCollision() override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string ToCode() override;
        std::string GetDrawCode() override;
        std::string GetInitCode() override;
};

//pour representer un cone
class ConeNode : public SceneNode {
private:
    Model modele;
public:
    ConeNode();
    ~ConeNode() override;
    void Draw() override;
    BoundingBox GetBoiteCollision() override;
    std::unique_ptr<SceneNode> Cloner() override;
    std::string ToCode()override;
    std::string GetDrawCode()override;
    std::string GetInitCode()override;
};

class CameraNode : public SceneNode{
    public:
        Vector3 target = {0.0f, 0.0f, 0.0f};
        float fovy = 45.0f;//par defaut
        CameraMode mode_camera = CAMERA_FREE;//par defaut
        CameraProjection projetction_cam = CAMERA_PERSPECTIVE;//par defaut
    CameraNode();
    void Draw() override;
    std::string ToCode()override;

    std::string GetDrawCode()override;
    

    std::string GetInitCode()override;

    BoundingBox GetBoiteCollision()override;
    std::unique_ptr<SceneNode> Cloner() override;
};

class Camera2DNode : public SceneNode{
    public :
        int screenWidth = 1280.0f;
        int screenHeight = 720.0f;
        Vector2 offset_camera = {screenWidth/2.0f,screenHeight/2.0f}; // si je veut faire {screenWidth/2, screenHeight/2}; faut les definir en globales dans le projet
        //Vector2 target_camera = {position.x,position.y};//par exemple
        //float rotation_camera = rotation.x;
        float zoom_camera = 1.0f;
        Camera2DNode();

    void Draw()override;

    std::string ToCode()override;

    std::string GetDrawCode()override;
    
    std::string GetCleanupCode()override;

    std::string GetInitCode()override;
    std::unique_ptr<SceneNode> Cloner()override;
};

std::string GenererCodeComplet(const std::vector<std::unique_ptr<SceneNode>>& nodes);

