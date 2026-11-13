#include <string>
#include <sstream> //pour std::stringstream
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath> //pour les bounding box
#include "raylib.h"
#include "raymath.h"
#include "../utils.h"
#include <rlgl.h>
#include "../Managers/MaterialManager.h"
#include "../Managers/ModelManager.h"

#pragma once

//la classe pour representer un objets de la scene
class SceneNode {
    protected:
        //la fonction des uniformes pour les shaders
        void AppliquerVariablesGlobales(EditorContext& variables, Shader shader_cible);
    public:
        int id_materiau = 0; //de base
        std::string nom;
        Vector3 position;
        Vector3 rotation;
        Vector3 taille;
        Color couleur;
        bool isSelected;
        bool emetOmbre = true;//utile que pour les objets physiques
        bool recoitOmbre = true;//utile que pour les objets physiques
        std::string type; //pour pouvoir savoir à quoi on a affaire pour la sauvgarde
        
        virtual std::string GetShadowPassCode(EditorContext& Les_variables, std::string drawMeshes){return "";}
        virtual std::string GetShadowBindCode(EditorContext& Les_variables){return "";}
        virtual std::string GetShadowUnbindCode(){return "";}
        virtual BoundingBox GetBoiteCollision(){
            //pour la selectioner avec la souris l'objet a une bounding box
            return BoundingBox{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
        }
    virtual Matrix GetMatriceMonde() const {
        Matrix m = MatrixScale(taille.x, taille.y, taille.z);
        m = MatrixMultiply(m, MatrixRotateXYZ({rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD}));
        m = MatrixMultiply(m, MatrixTranslate(position.x, position.y, position.z));
        return m;
    }
    virtual BoundingBox GetBoiteLocale() const {
        return BoundingBox{ {-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f} };
    }
    
    //virtual parce que on les définies dans les classe qui hérite d'elle
    virtual ~SceneNode() = default;
    virtual void Draw(EditorContext& variables) = 0;
    virtual std::string GetUpdateCode(EditorContext& variables) = 0;
    //avant while (!WindowShouldClose())
    virtual std::string GetInitCode(EditorContext& variables);//par défaut TODO à changer

    //dans le BeginMode3D
    virtual std::string GetDrawCode();

    //pour dupliquer
    virtual std::unique_ptr<SceneNode> Cloner();

    //la boucle les unloads
    virtual std::string GetCleanupCode();

    //update
    virtual void Update(EditorContext& variables) {}

    //pour l'ui
    virtual std::string GetType() const = 0; //les enfants disent qui ils sont
    //faut faire un constructeur par défaut au cas où
    SceneNode(){
        nom = "";
        position = {0.0f,0.0f,0.0f};
        rotation = {0.0f,0.0f,0.0f};
        taille = {0.0f,0.0f,0.0f};
        couleur = WHITE;
        isSelected = false;
    }
    std::string FormaterNomVariableShader(const std::string& nomBrut);
};

//pour representer un cube
class CubeNode : public SceneNode{
    private:
        Model modele;
    public :
    CubeNode();

    ~CubeNode() override;

    void Draw(EditorContext& variables) override;
    std::string GetUpdateCode(EditorContext& variables){//pour apres ? TODO
        return "";
    }
    std::string GetDrawCode() override;
    
    std::string GetInitCode(EditorContext& variables) override;
    
    std::string GetCleanupCode() override;
    
    BoundingBox GetBoiteLocale() const;
    BoundingBox GetBoiteCollision() override;

    std::unique_ptr<SceneNode> Cloner() override;
    std::string GetType() const override { return "CubeNode"; }
};

//pour representer une sphere
class SphereNode : public SceneNode{
    private:
        Model modele;
    public :
        float rayon;
        int slices;
        int ring;
    SphereNode();
    ~SphereNode() override;
    void ReconstruireMesh();
    void Draw(EditorContext& variables) override;

    std::string GetUpdateCode(EditorContext& variables){
        return "";
    }
    std::string GetDrawCode()override;
    std::string GetInitCode(EditorContext& variables)override;
    std::string GetCleanupCode() override;

    BoundingBox GetBoiteLocale() const;
    BoundingBox GetBoiteCollision() override;
    
    std::unique_ptr<SceneNode> Cloner() override;
    
    std::string GetType() const override { return "SphereNode"; }
};


//pour representer un plan (sol)
class PlaneNode : public SceneNode {
private:
    Model modele;
public:
    float width;
    float length;
    int resX;
    int resZ;
    PlaneNode();
    ~PlaneNode() override ;
    void ReconstruireMesh();
    void Draw(EditorContext& variables) override;
    
    std::string GetUpdateCode(EditorContext& variables)override;
    
    std::string GetDrawCode() override;
    std::string GetInitCode(EditorContext& variables) override;
    std::string GetCleanupCode() override;

    BoundingBox GetBoiteLocale() const;
    BoundingBox GetBoiteCollision() override;

    std::unique_ptr<SceneNode> Cloner() override;
    
    std::string GetType() const override { return "PlaneNode"; }
};

//pour representer un cylindre
class CylinderNode : public SceneNode {
    private:
        Model modele;
    public:
        float radius;
        float height;
        int slices;

        void ReconstruireMesh();
        CylinderNode();
        ~CylinderNode() override;

        void Draw(EditorContext& variables) override ;

        std::string GetUpdateCode(EditorContext& variables)override;
        std::string GetDrawCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetCleanupCode() override;

        BoundingBox GetBoiteLocale() const;
        BoundingBox GetBoiteCollision() override;
        
        std::unique_ptr<SceneNode> Cloner() override;
        
        std::string GetType() const override { return "CylinderNode"; }
};

//pour representer un cone
class ConeNode : public SceneNode {
private:
    Model modele;
public:
    float radius;
    float height;
    int slices;

    ConeNode();
    ~ConeNode() override;
    void ReconstruireMesh();
    void Draw(EditorContext& variables) override;
    
    std::string GetUpdateCode(EditorContext& variables)override;
    std::string GetDrawCode()override;
    std::string GetInitCode(EditorContext& variables)override;
    std::string GetCleanupCode() override;

    BoundingBox GetBoiteLocale() const;
    BoundingBox GetBoiteCollision() override;

    std::unique_ptr<SceneNode> Cloner() override;
    
    std::string GetType() const override { return "ConeNode"; }
};

class CameraNode : public SceneNode{
    public:
        Vector3 target = {0.0f, 0.0f, 0.0f};
        float fovy = 45.0f;//par defaut
        CameraMode mode_camera = CAMERA_FREE;//par defaut
        CameraProjection projetction_cam = CAMERA_PERSPECTIVE;//par defaut
        bool active = true;
    CameraNode();
    void Draw(EditorContext& variables) override;
    std::string GetUpdateCode(EditorContext& variables)override;

    std::string GetDrawCode()override;
    

    std::string GetInitCode(EditorContext& variables)override;
    std::string GetCleanupCode() override;
    
    BoundingBox GetBoiteLocale() const;
    BoundingBox GetBoiteCollision() override;

    std::unique_ptr<SceneNode> Cloner() override;
    std::string GetType() const override { return "Camera3D"; }
    Camera3D ObtenirCameraRaylib() const;
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

    void Draw(EditorContext& variables)override;

    std::string GetUpdateCode(EditorContext& variables)override;

    std::string GetDrawCode()override;
    
    std::string GetCleanupCode()override;

    std::string GetInitCode(EditorContext& variables)override;
    std::unique_ptr<SceneNode> Cloner()override;
    std::string GetType() const override { return "Camera2D"; }
};

class ModeleNode : public SceneNode{
    public :
        Model* modele = nullptr;
        std::string chemin_modele;
    ModeleNode();

    ~ModeleNode() override;

    void Draw(EditorContext& variables) override;
    std::string GetUpdateCode(EditorContext& variables){
        return "";
    }

    void ChargerFichier3D(std::string chemin, ManagerModele* manager);
    std::string GetDrawCode() override;
    
    std::string GetInitCode(EditorContext& variables) override;
    //TOTO changer ça
    BoundingBox GetBoiteLocale() const;
    BoundingBox GetBoiteCollision() override;

    std::unique_ptr<SceneNode> Cloner() override;
    std::string GetType() const override { return "ModeleNode"; }
};

std::string GenererCodeComplet(const std::vector<std::unique_ptr<SceneNode>>& nodes, ManagerMateriel* materiaux_managers, ShaderManager *shaders_manager, ManagerModele *modele_manager, EditorContext& Les_variables);

std::string FormaterNomModele(const std::string& chemin);
std::string FormaterNomVariableShader(const std::string& nomBrut);