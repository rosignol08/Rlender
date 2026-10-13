#include "Noeuds/SceneNode.h"

std::string GenererCodeComplet(const std::vector<std::unique_ptr<SceneNode>>& nodes, ManagerMateriel* materiaux_managers, ShaderManager *shaders_manager, ManagerModele *modele_manager, EditorContext& Les_variables) {
    std::stringstream code;
    bool est3D, est2D;
    est3D = false;
    est2D = false;
    //faut voir le type de la camera TODO voir des optimisation possibles
    for (const auto& node : nodes) {
        //if (dynamic_cast<CameraNode*>(node.get()) != nullptr) {
        //    est3D = true;
        //} else if (dynamic_cast<Camera2DNode*>(node.get()) != nullptr) {
        //    est2D = true;
        //}
        if (node->GetType() == "Camera3D"){
            est3D = true;
        } else if (node->GetType() == "Camera2D"){
            est2D = true;
        }
    }
    //ça doit ecrire le code de base de raylib

    //base
    code << "#include \"raylib.h\"\n\n";
    code << "int main(void) {\n";
    code << "    InitWindow(1280, 720, \"Jeu Genere\");\n";
    code << "    SetTargetFPS(60);\n\n";
    
    //init des materiaux
    code << "    // --- Initialisation des materiaux ---\n";
    
    for (const auto& materiel : materiaux_managers->GetTousLesMateriaux()) {
        
        //couleur du matériau
        //cast en (int) sinon uint8_t affiche comme caractère invisible
        code << "    Color couleur_" << materiel.nom << " = { "
             << (int)materiel.couleurAlbedo.r << ", "
             << (int)materiel.couleurAlbedo.g << ", "
             << (int)materiel.couleurAlbedo.b << ", "
             << (int)materiel.couleurAlbedo.a << " };\n";

        //chargement texture si existe
        //check si chemin enregistre pour albedo
        if (materiel.chemins_textures.count("albedo") > 0) {
            std::string chemin_fichier = materiel.chemins_textures.at("albedo");
            code << "    Texture2D texture_" << materiel.nom << " = LoadTexture(\"" << chemin_fichier << "\");\n";
        }
    }
    code << "\n";

    //pour les shaders
    code << "    // --- Shaders ---\n";
    for (const auto& [nom_shader, code_shader] : shaders_manager->dictionnaire_shaders) {
        // Idéalement, il faudra exporter les textes des shaders dans des vrais fichiers .vs et .fs
        code << "    Shader shader_" << nom_shader << " = LoadShader(\"Assets/" << nom_shader << ".vs\", \"Assets/" << nom_shader << ".fs\");\n";
    }
    
    //init noeuds
    code << "    // --- Initialisation des objets ---\n";
    for (const auto& node : nodes) {
        code << node->GetInitCode(Les_variables);
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
        code << node->GetDrawCode(Les_variables);
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

    code << "    //nettoyage\n";
    for (const auto& materiel : materiaux_managers->GetTousLesMateriaux()) {
        if (materiel.chemins_textures.count("albedo") > 0) {
            code << "    UnloadTexture(texture_" << materiel.nom << ");\n";
        }
    }
    for (const auto& [nom_shader, code_shader] : shaders_manager->dictionnaire_shaders) {
        code << "    UnloadShader(shader_" << nom_shader << ");\n";
    }

    code << "\n    CloseWindow();\n";
    code << "    return 0;\n";
    code << "}\n";

    return code.str();
}