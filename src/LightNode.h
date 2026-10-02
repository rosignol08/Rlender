#include "SceneNode.h"

class LightNode : public SceneNode{
    public:
    Vector3 couleur_lumiere;
    float intensite;
        LightNode();
        void Update(EditorContext& variables) override;
        void Draw(EditorContext& variables) override;
        std::unique_ptr<SceneNode> Cloner() override;
        std::string ToCode() override;
        std::string GetDrawCode() override;
        std::string GetInitCode() override;
        std::string GetType() const override;
        //bool DessinerProprietesImGui(ShaderManager& shaderManager) override;
};