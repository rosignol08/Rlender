#include "SceneNode.h"
#pragma once
class LightNode : public SceneNode{
    public:
    Vector3 couleur_lumiere;
    float intensite;
    float puissance;
    float taille;
    bool est_allume = true;
        LightNode();
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string ToCode() override;
        std::string GetDrawCode() override;
        std::string GetInitCode() override;
        std::string GetType() const {
            return "LightNode";
        }
        BoundingBox GetBoiteCollision() override;
        //bool DessinerProprietesImGui(ShaderManager& shaderManager) override;
};