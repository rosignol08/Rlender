#include "SceneNode.h"
#pragma once

class SoleilNode : public SceneNode {
    public :
    Vector3 direction;
    Vector3 couleur_lumiere;
    float puissance;
    bool est_allume = true;
    bool Ombre;
    int res_shadowmap;
    Camera3D ombre_cam;
    RenderTexture2D texture_ombre;
    Matrix lightViewProj;
        SoleilNode();
        virtual ~SoleilNode();
        std::string GetShadowPassCode(EditorContext& Les_variables, std::string Liste_rendu_ombre) override;
        std::string GetShadowBindCode(EditorContext&) override;
        std::string GetShadowUnbindCode() override;
        void changement_shadowmap();
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string GetUpdateCode(EditorContext& variables) override;
        std::string GetDrawCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetCleanupCode()override;
        std::string GetType() const override{
            return "SoleilNode";
        }
        BoundingBox GetBoiteCollision() override;
};


//fonction utilitaire pour les lumieres
//Copyright (c) 2023-2025 TheManTheMythTheGameDev (@TheManTheMythTheGameDev)
static RenderTexture2D LoadShadowmapRenderTexture(int width, int height);

// Unload shadowmap render texture from GPU memory (VRAM)
static void UnloadShadowmapRenderTexture(RenderTexture2D target);
