#pragma once
#include "SceneNode.h"


class LumiereOmbreNode : public SceneNode {
    public :
    //Vector3 direction;
    Vector3 couleur_lumiere;
    float puissance = 1.0f;
    bool est_allume = true;
    bool Ombre = true;
    int res_shadowmap = 1024;
    int index_ombre = 0;
    Camera3D ombre_cam = {};
    RenderTexture2D texture_ombre = {};
    Matrix lightViewProj = {};
        LumiereOmbreNode();
        
        //securite
        LumiereOmbreNode(const LumiereOmbreNode&) = delete;
        LumiereOmbreNode& operator=(const LumiereOmbreNode&) = delete;

        virtual ~LumiereOmbreNode();
        
        void changement_shadowmap();
        //virtual void Maj_camera_Ombre() = 0; jsp si ça sert ou pas de deleguer dans une fonction autre que update
        
        std::string GetShadowPassCode(EditorContext& Les_variables, std::string Liste_rendu_ombre) override;
        std::string GetShadowBindCode(EditorContext&) override;
        std::string GetShadowUnbindCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetCleanupCode()override;
        //std::string GetType() const override;
        //BoundingBox GetBoiteCollision() override;
};

class SoleilNode : public LumiereOmbreNode {
    public:
        Vector3 direction;
    
        SoleilNode();
        
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string GetUpdateCode(EditorContext& variables) override;
        std::string GetDrawCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetCleanupCode() override;
        std::string GetType() const {
            return "SoleilNode";
        }
        BoundingBox GetBoiteCollision() override;
};

class SpotNode : public LumiereOmbreNode {
    public:
        Vector3 direction;
    
        SpotNode();
        
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string GetUpdateCode(EditorContext& variables) override;
        std::string GetDrawCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetCleanupCode() override;
        std::string GetType() const override;
        BoundingBox GetBoiteCollision() override;
};


//fonction utilitaire pour les lumieres
//Copyright (c) 2023-2025 TheManTheMythTheGameDev (@TheManTheMythTheGameDev)
RenderTexture2D LoadShadowmapRenderTexture(int width, int height);

// Unload shadowmap render texture from GPU memory (VRAM)
void UnloadShadowmapRenderTexture(RenderTexture2D target);
