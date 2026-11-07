#include "Noeuds/SceneNode.h"
#include "Noeuds/LightNode.h"
#include "Noeuds/DirectionalLightNode.h"//au cas où
#include <algorithm>

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
    code << "#include \"rlgl.h\"\n\n";
    code << "#include \"raymath.h\"\n\n";
    code << "static RenderTexture2D LoadShadowmapRenderTexture(int width, int height){\n";
    code << "    RenderTexture2D target = { 0 };\n";
    code << "    target.id = rlLoadFramebuffer(); // Load an empty framebuffer\n\n";
    code << "    target.texture.width = width;\n";
    code << "    target.texture.height = height;\n";
    code << "    if (target.id > 0){\n";
    code << "        rlEnableFramebuffer(target.id);\n";
    code << "        target.depth.id = rlLoadTextureDepth(width, height, false);\n";
    code << "        target.depth.width = width;\n";
    code << "        target.depth.height = height;\n";
    code << "        target.depth.format = 19; // DEPTH_COMPONENT_24BIT?\n";
    code << "        target.depth.mipmaps = 1;\n";
    code << "        // Attach depth texture to FBO\n";
    code << "        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);\n";
    code << "        // Check if fbo is complete with attachments (valid)\n";
    code << "        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, \"FBO: [ID %i] Framebuffer object created successfully\", target.id);\n";
    code << "        rlDisableFramebuffer();\n";
    code << "    }\n";
    code << "    else TRACELOG(LOG_WARNING, \"FBO: Framebuffer object can not be created\");\n";
    code << "    return target;\n";
    code <<"} \n";

    code << "static void UnloadShadowmapRenderTexture(RenderTexture2D target){\n";
    code << "    if (target.id > 0){rlUnloadFramebuffer(target.id);}\n";
    code << "}\n";

    code << "\n";

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
    std::cout << "shaders initialisé" << std::endl;
    code << "\n";
    int compteur_export = 0;
    for (auto& node : nodes) {
        if (node->GetType() == "LightNode" || node->GetType() == "Light") {
            static_cast<LightNode*>(node.get())->index = compteur_export++;
        }
    }
    //init des modeles
    code << "    // --- Initialisation des modeles ---\n";
    for (const auto& [nom_modele, modele] : modele_manager->dictionnaire_modeles) {
        std::string varName = FormaterNomModele(nom_modele);//le nom du modele c'est son emplacement absolu donc on s'en fou
        //Les modeles sont chargés depuis les fichiers
        std::string chemin = nom_modele; //faut remplacer chemin par "modeles/" + std::filesystem::path(nom_modele).filename().string() apres
        std::replace(chemin.begin(), chemin.end(), '\\', '/');
        code << "    Model " << varName << " = LoadModel(\"" << chemin << "\");\n";
        code << "    for (int i = 0; i < " << varName << ".materialCount; ++i) { "<< varName << ".materials[i].shader = shader_defaut; }\n";
    }
    code << "\n";

    std::cout << "modele initialisé" << std::endl;
    //init noeuds
    code << "    // --- Initialisation des objets ---\n";
    for (const auto& node : nodes) {
        code << node->GetInitCode(Les_variables);
    }
    std::cout << "objets initialisé" << std::endl;

    if (est3D) {
        code << "    Camera3D* camera_active = &" << nom_camera_principale << ";\n";
    } else if (est2D) {
        code << "    Camera2D* camera_active = &" << nom_camera_principale << ";\n";
    }

    std::cout << "camera initialisé" << std::endl;
    //nb de lumieres pour chaque shader
    for (const auto& [nom_shader, _] : shaders_manager->dictionnaire_shaders) {
        std::string varName = FormaterNomVariableShader(nom_shader);
        code << "    int locCount_" << nom_shader << " = GetShaderLocation("<<varName << ", \"lightCount\");\n";
        code << "    int count_" << nom_shader << " = " << compteur_export << ";\n";
        code << "    SetShaderValue("<< varName << ", locCount_" << nom_shader << ", &count_" << nom_shader << ", SHADER_UNIFORM_INT);\n";
    }

    if (est3D){    
        for (const auto& [nom_shader, _] : shaders_manager->dictionnaire_shaders) {
            std::string varName = FormaterNomVariableShader(nom_shader);
            code << "        int locViewPos_" << varName << " = GetShaderLocation(" << varName << ", \"viewPos\");\n";
            code << "        SetShaderValue(" << varName << ", locViewPos_" << varName << ", &camera_active->position, SHADER_UNIFORM_VEC3);\n";
        }
    }

    std::cout << "est 3D initialisé" << std::endl;
    std::stringstream ombres_liste;
    //dessin ombres
    for (const auto& node : nodes) {
        if(node->emetOmbre){
            ombres_liste << node->GetDrawCode();
        }
    }
    
    //boucle
    code << "\n    while (!WindowShouldClose()) {\n";
    
    //maj des noeuds
    for (const auto& node : nodes) {
        code << node->GetUpdateCode(Les_variables);
    }

    std::cout << "Maj noeud" << std::endl;

    //passe d'ombre
    for(const auto& node : nodes){
        code << node->GetShadowPassCode(Les_variables, ombres_liste.str());
    }

    std::cout << "pass ombre" << std::endl;
    //code << ;
        
    code << "          BeginDrawing();\n";
    code << "          ClearBackground(SKYBLUE);\n";
    
    if(est2D){
        code << "            BeginMode2D(*camera_active);\n\n";
    }else if (est3D){
        code << "            BeginMode3D(*camera_active);\n\n";//TODO donner le vrai nom
    }
    
    //update des shaders
    if (est3D){    
        for (const auto& [nom_shader, _] : shaders_manager->dictionnaire_shaders) {
            std::string varName = FormaterNomVariableShader(nom_shader);
            code << "        SetShaderValue(" << varName << ", locViewPos_" << varName << ", &camera_active->position, SHADER_UNIFORM_VEC3);\n";
        }
    }

    std::cout << "update shader" << std::endl;

    //pour l'ombre de la lumiere directionelle
    for (const auto& node : nodes) {
        code << node->GetShadowBindCode(Les_variables);
    }
    std::cout << "ombre bind" << std::endl;

    //dessin noeuds
    for (const auto& node : nodes) {
        code << node->GetDrawCode();
    }

    std::cout << "rendu des noeud" << std::endl;

    //pour l'ombre de la lumiere directionelle
    for (const auto& node : nodes) {
        code << node->GetShadowUnbindCode();
    }

    std::cout << "ombre unbind" << std::endl;

    //fermeture
    if(est2D){
        code << "            EndMode2D();\n";
    }else if (est3D){
        code << "            EndMode3D();\n";
    }
    code << "        EndDrawing();\n";
    code << "    }\n\n";

    std::cout << "end drawing" << std::endl;

    //nettoyage
    for (const auto& node : nodes) {
        code << node->GetCleanupCode();
    }

    std::cout << "nettoyage" << std::endl;

    code << "    //nettoyage\n";
    for (const auto& materiel : materiaux_managers->GetTousLesMateriaux()) {
        if (materiel.chemins_textures.count("albedo") > 0) {
            code << "    UnloadTexture(texture_" << materiel.nom << ");\n";
        }
    }
    std::cout << "nettoyage materiaux" << std::endl;
    for (const auto& [nom_shader, code_shader] : shaders_manager->dictionnaire_shaders) {
        std::string varName = FormaterNomVariableShader(nom_shader);
        code << "    UnloadShader(" << varName << ");\n";
    }

    std::cout << "nettoyage shaders" << std::endl;
    code << "\n    CloseWindow();\n";
    code << "    return 0;\n";
    code << "}\n";

    return code.str();
}