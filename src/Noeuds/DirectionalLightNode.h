#include "SceneNode.h"
#pragma once

class SoleilNode : public SceneNode {
    public :
    Vector3 direction;
    Vector3 couleur_lumiere;
    float puissance;
    bool est_allume = true;
        SoleilNode();
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string ToCode() override;
        std::string GetDrawCode() override;
        std::string GetInitCode() override;
        std::string GetType() const override{
            return "SoleilNode";
        }
        BoundingBox GetBoiteCollision() override;
};