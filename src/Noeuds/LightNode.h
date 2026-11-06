#include "SceneNode.h"
#pragma once
class LightNode : public SceneNode{
    public:
    Vector3 couleur_lumiere;
    float intensite;
    float puissance;
    float taille;
    bool est_allume = true;
    int index;
    bool Ombre;
    int res_shadowmap;
    Matrix matrices [6] = {0};//matrices des 6 cameras
    RenderTexture texture_ombre;
        LightNode();
        void UpdateMatricesOmbre();
        void changement_shadowmap();
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string GetUpdateCode(EditorContext& variables) override;
        std::string GetDrawCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetType() const {
            return "LightNode";
        }
        BoundingBox GetBoiteCollision() override;
};

//fonction utilitaire pour les lumieres
//Copyright (c) 2023-2025 TheManTheMythTheGameDev (@TheManTheMythTheGameDev)
static RenderTexture2D LoadShadowmapRenderTexture(int width, int height);

// Unload shadowmap render texture from GPU memory (VRAM)
static void UnloadShadowmapRenderTexture(RenderTexture2D target);
