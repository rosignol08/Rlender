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
        SoleilNode();
        void changement_shadowmap();
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string GetUpdateCode(EditorContext& variables) override;
        std::string GetDrawCode() override;
        std::string GetInitCode(EditorContext& variables) override;
        std::string GetType() const override{
            return "SoleilNode";
        }
        BoundingBox GetBoiteCollision() override;
};