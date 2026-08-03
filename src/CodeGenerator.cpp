#include "SceneNode.h"

std::string GenererCodeComplet(const std::vector<std::unique_ptr<SceneNode>>& nodes) {
    std::stringstream code;
    bool est3D, est2D;
    est3D = false;
    est2D = false;
    //faut voir le type de la camera TODO voir des optimisation possibles
    for (const auto& node : nodes) {
        if (dynamic_cast<CameraNode*>(node.get()) != nullptr) {
            est3D = true;
        } else if (dynamic_cast<Camera2DNode*>(node.get()) != nullptr) {
            est2D = true;
        }
        
    }
    //ça doit ecrire le code de base de raylib

    //base
    code << "#include \"raylib.h\"\n\n";
    code << "int main(void) {\n";
    code << "    InitWindow(1280, 720, \"Jeu Genere\");\n";
    code << "    SetTargetFPS(60);\n\n";
    
    //init noeuds
    code << "    // --- Initialisation des objets ---\n";
    for (const auto& node : nodes) {
        code << node->GetInitCode();
    }

    //boucle
    code << "\n    while (!WindowShouldClose()) {\n";
    code << "        BeginDrawing();\n";
    code << "            ClearBackground(DARKGRAY);\n";
    

    if(est2D){
        code << "            BeginMode2D(camera);\n\n";
    }else if (est3D){
        code << "            BeginMode3D(camera);\n\n";
    }
    

    //dessin noeuds
    for (const auto& node : nodes) {
        code << node->GetDrawCode();
    }

    //fermeture
    if(est2D){
        code << "            EndMode2D();\n";
    }else if (est3D){
        code << "            EndMode3D();\n";
    }
    code << "        EndDrawing();\n";
    code << "    }\n\n";

    //nettoyage
    for (const auto& node : nodes) {
        code << node->GetCleanupCode();
    }

    code << "\n    CloseWindow();\n";
    code << "    return 0;\n";
    code << "}\n";

    return code.str();
}