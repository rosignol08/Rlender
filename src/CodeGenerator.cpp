#include "Noeuds/SceneNode.h"
#include "Noeuds/LightNode.h"

std::string FormaterNomVariableShader(const std::string& nomBrut) {
    std::string nomNettoye = nomBrut;
    
    //faut enlever "Shader_" ou "shader_" si c'est là
    if (nomNettoye.rfind("Shader_", 0) == 0) nomNettoye = nomNettoye.substr(7);
    if (nomNettoye.rfind("shader_", 0) == 0) nomNettoye = nomNettoye.substr(7);
    
    //minuscules pour coherence
    for (char &c : nomNettoye) c = std::tolower(c);

    //return un nom unique et propre : "shader_defaut"
    return "shader_" + nomNettoye;
}

std::string GenererCodeComplet(const std::vector<std::unique_ptr<SceneNode>>& nodes, ManagerMateriel* materiaux_managers, ShaderManager *shaders_manager, ManagerModele *modele_manager, EditorContext& Les_variables) {
    std::stringstream code;
    std::string nom_camera_principale = "";
    bool est3D, est2D;
    est3D = false;
    est2D = false;
    //faut voir le type de la camera TODO voir des optimisation possibles
    for (const auto& node : nodes) {
        if (node->GetType() == "Camera3D"){
            est3D = true;
            nom_camera_principale = node->nom;
        } else if (node->GetType() == "Camera2D"){
            est2D = true;
            nom_camera_principale = node->nom;
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
        std::string varName = FormaterNomVariableShader(nom_shader);
        if (nom_shader == "Shader_defaut" || nom_shader == "Defaut") {
            //cas shader par défaut est injecté en dur dans le code généré
            code << "    const char* code_vs_defaut = R\"(\n" << Les_variables.codeVertexShader << "\n)\";\n\n";
                 
            code << "    const char* code_fs_defaut = R\"(\n" << Les_variables.codeFragmentShader << "\n)\";\n\n";
                 
            code << "    Shader " << varName << " = LoadShaderFromMemory(code_vs_defaut, code_fs_defaut);\n";
        } 
        else {
            //cas autres shaders sont chargés depuis les fichiers
            code << "    Shader " << varName << " = LoadShader(\"Assets/" << nom_shader << ".vs\", \"Assets/" << nom_shader << ".fs\");\n";
        }
        code << "    " << varName << ".locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(" << varName << ", \"matModel\");\n";
    }
    code << "\n";
    int compteur_export = 0;
    for (auto& node : nodes) {
        if (node->GetType() == "LightNode" || node->GetType() == "Light") {
            static_cast<LightNode*>(node.get())->index = compteur_export++;
        }
    }
    //init noeuds
    code << "    // --- Initialisation des objets ---\n";
    for (const auto& node : nodes) {
        code << node->GetInitCode(Les_variables);
    }

    if (est3D) {
        code << "    Camera3D* camera_active = &" << nom_camera_principale << ";\n";
    } else if (est2D) {
        code << "    Camera2D* camera_active = &" << nom_camera_principale << ";\n";
    }

    //nb de lumieres pour chaque shader
    for (const auto& [nom_shader, _] : shaders_manager->dictionnaire_shaders) {
        std::string varName = FormaterNomVariableShader(nom_shader);
        code << "    int locCount_" << nom_shader << " = GetShaderLocation("<<varName << ", \"lightCount\");\n";
        code << "    int count_" << nom_shader << " = " << compteur_export << ";\n";
        code << "    SetShaderValue("<< varName << ", locCount_" << nom_shader << ", &count_" << nom_shader << ", SHADER_UNIFORM_INT);\n";
    }

    //boucle
    code << "\n    while (!WindowShouldClose()) {\n";
    
    //maj des noeuds
    for (const auto& node : nodes) {
        code << node->GetUpdateCode(Les_variables);
    }
    
    if (est3D){    
        for (const auto& [nom_shader, _] : shaders_manager->dictionnaire_shaders) {
            std::string varName = FormaterNomVariableShader(nom_shader);
            code << "        int locViewPos_" << varName << " = GetShaderLocation(" << varName << ", \"viewPos\");\n";
            code << "        SetShaderValue(" << varName << ", locViewPos_" << varName << ", &camera_active->position, SHADER_UNIFORM_VEC3);\n";
        }
    }
        
    code << "          BeginDrawing();\n";
    code << "          ClearBackground(SKYBLUE);\n";
    
    if(est2D){
        code << "            BeginMode2D(*camera_active);\n\n";
    }else if (est3D){
        code << "            BeginMode3D(*camera_active);\n\n";//TODO donner le vrai nom
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

    code << "    //nettoyage\n";
    for (const auto& materiel : materiaux_managers->GetTousLesMateriaux()) {
        if (materiel.chemins_textures.count("albedo") > 0) {
            code << "    UnloadTexture(texture_" << materiel.nom << ");\n";
        }
    }
    for (const auto& [nom_shader, code_shader] : shaders_manager->dictionnaire_shaders) {
        std::string varName = FormaterNomVariableShader(nom_shader);
        code << "    UnloadShader(" << varName << ");\n";
    }

    code << "\n    CloseWindow();\n";
    code << "    return 0;\n";
    code << "}\n";

    return code.str();
}