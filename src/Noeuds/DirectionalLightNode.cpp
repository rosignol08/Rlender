#include "DirectionalLightNode.h"

SoleilNode::SoleilNode(){
    position = {0.0f, 0.0f, 0.0f};
    direction = {0.0f, -1.0f, 0.0f};
    couleur_lumiere = {1.0f, 1.0f, 1.0f};
    puissance = 1.0f;
    est_allume = true;
    Ombre = true;
    res_shadowmap = 1024;//valeur par defaut on a plus besoin de la valeur générale dans utils.h
    texture_ombre = LoadShadowmapRenderTexture(res_shadowmap,res_shadowmap);
    ombre_cam.projection = CAMERA_ORTHOGRAPHIC;
    ombre_cam.position = position;
    ombre_cam.fovy = 40.0f;
    //ombre_cam.target = //a voir 
    type = "SoleilNode";
    emetOmbre = false;
    recoitOmbre = false;
}

SoleilNode::~SoleilNode(){
    //destructeur
    UnloadShadowmapRenderTexture(texture_ombre);
}

void SoleilNode::changement_shadowmap(){
    //reallocation texture shadow map
    UnloadShadowmapRenderTexture(texture_ombre);
    texture_ombre = LoadShadowmapRenderTexture(res_shadowmap,res_shadowmap);
}

void SoleilNode::Update(EditorContext& variables){
    if (!est_allume) {
        //eteint puissance = 0
        variables.variables_globales["sunPower"] = 0.0f;
        return;
    }

    //un sele soleil
    //variables uniques.
    variables.variables_globales["sunDirection"] = this->direction;
    variables.variables_globales["sunColor"] = this->couleur_lumiere;
    variables.variables_globales["sunPower"] = this->puissance;

    Vector3 dir_norm = Vector3Normalize(this->direction);
    
    this->ombre_cam.position = Vector3Scale(dir_norm, -20.0f); 
    
    this->ombre_cam.target = Vector3Zero(); 
    
    this->ombre_cam.fovy = 40.0f;
    if (fabs(dir_norm.x) < 0.001f && fabs(dir_norm.z) < 0.001f) {
        this->ombre_cam.up = Vector3{ 0.0f, 0.0f, -1.0f }; // On triche en mettant le Up sur Z
    } else {
        this->ombre_cam.up = Vector3{ 0.0f, 1.0f, 0.0f };  // Comportement normal
    }
}


void SoleilNode::Draw(EditorContext& variables){
    DrawSphereWires(position, 0.5f,16,16, YELLOW);
    Matrix matRot = MatrixRotateXYZ(Vector3{ rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD });
    Vector3 direction_par_defaut = { 0.0f, -1.0f, 0.0f };
    this->direction = Vector3Transform(direction_par_defaut, matRot);
    float longueur_direction = Vector3Length(direction);
    if (longueur_direction > 0.001f) {
        
        Vector3 dir_norm = Vector3Normalize(direction);
        
        Vector3 fin_ligne = Vector3Add(position, Vector3Scale(dir_norm, 3.0f));

        DrawLine3D(position, fin_ligne, YELLOW);

        //boule pour indiquer la direction
        if (this->isSelected) {
            DrawSphereWires(fin_ligne, 0.2f, 8, 8, RED);
        }
    }
}

std::unique_ptr<SceneNode> SoleilNode::Cloner(){
    auto clone = std::make_unique<SoleilNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = position;
    clone->taille = taille;
    clone->couleur_lumiere = couleur_lumiere;
    clone->couleur = couleur;
    clone->direction = direction;
    clone->puissance = puissance;
    clone->est_allume = est_allume;
    clone->isSelected = true;
    clone->Ombre = Ombre;
    clone->res_shadowmap = res_shadowmap;
    static unsigned int compteur_Soleil_clones = 0; //logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Soleil_clones); // nouveau nom
    compteur_Soleil_clones++;
    return clone;
}

std::string SoleilNode::GetShadowPassCode(EditorContext& Les_variables, std::string Liste_rendu_ombre){
    std::stringstream code;
    if(est_allume && Ombre){
        code << "BeginTextureMode(texture_ombre_" << nom << ");\n";
        code << "ClearBackground(WHITE); // Le blanc = profondeur maximale (loin)\n";
        code << "rlSetClipPlanes(1.0, 60.0);\n";
        code << "BeginMode3D(ombre_cam_" << nom << ");\n";

        code << "Matrix lightView_"<< nom <<" = GetCameraMatrix(ombre_cam_" << nom << ");\n";
        code << "Matrix lightProj_"<< nom <<" = rlGetMatrixProjection(); \n";
        code << "lightViewProj_" << nom << " = MatrixMultiply(lightView_"<<nom<<", lightProj_"<< nom <<");\n";
        code << "rlEnableBackfaceCulling();\n";
        code << "rlSetCullFace(RL_CULL_FACE_FRONT);\n";
        code << Liste_rendu_ombre;
        code << "rlSetCullFace(RL_CULL_FACE_BACK);\n";
        

        code << "EndMode3D();\n";
        code << "rlSetClipPlanes(0.05, 4000.0);\n";
        code << "EndTextureMode();\n";
        // après EndTextureMode :
        for (const auto& [nom_shader, _] : Les_variables.gestionnaire_shaders->dictionnaire_shaders) {
            std::string s_var = FormaterNomVariableShader(nom_shader);
            code << "SetShaderValueMatrix(" << s_var << ", loc_vp_" << nom << "_" << s_var
                 << ", lightViewProj_" << nom << ");\n";
        }
        return code.str();
    }else{
        return "";
    }
}

std::string SoleilNode::GetShadowBindCode(EditorContext& Les_variables){
    if(!(est_allume && Ombre)) return "";
    std::stringstream code;
    code << "rlActiveTextureSlot(slot_" << nom << ");\n";
    code << "rlEnableTexture(texture_ombre_" << nom << ".depth.id);\n";
    for (const auto& [nom_shader, _] : Les_variables.gestionnaire_shaders->dictionnaire_shaders) {
        std::string s_var = FormaterNomVariableShader(nom_shader);
        code << "SetShaderValue(" << s_var << ", loc_shadow_" << nom << "_" << s_var
             << ", &slot_" << nom << ", SHADER_UNIFORM_INT);\n";
    }
    return code.str();
}

std::string SoleilNode::GetShadowUnbindCode(){
    if(!(est_allume && Ombre)) return "";
    std::stringstream code;
    code << "rlActiveTextureSlot(slot_" << nom << ");\n";
    code << "rlDisableTexture();\n";
    return code.str();
}

std::string SoleilNode::GetUpdateCode(EditorContext& variables){
    std::stringstream code;
    //maj direction et couleur
    code << "    dir_" << nom << " = { " 
         << std::to_string(direction.x) << "f, " 
         << std::to_string(direction.y) << "f, " 
         << std::to_string(direction.z) << "f };\n";
    
    code << "    couleur_" << nom << " = { " 
         << std::to_string(couleur_lumiere.x) << "f, " 
         << std::to_string(couleur_lumiere.y) << "f, " 
         << std::to_string(couleur_lumiere.z) << "f };\n";

    //check les shaders enregistrés pour générer l'envoi vers chacun
    for (const auto& [nom_shader, shader_raylib] : variables.gestionnaire_shaders->dictionnaire_shaders) {
        std::string s_var = FormaterNomVariableShader(nom_shader);
        code << "    SetShaderValue(" << s_var << ", loc_dir_" << this->nom << "_" << s_var << ", &dir_" << this->nom << ", SHADER_UNIFORM_VEC3);\n";
        code << "    SetShaderValue(" << s_var << ", loc_col_" << this->nom << "_" << s_var << ", &couleur_" << this->nom << ", SHADER_UNIFORM_VEC3);\n";
        code << "    SetShaderValue(" << s_var << ", loc_pow_" << this->nom << "_" << s_var << ", &puissance_" << this->nom << ", SHADER_UNIFORM_FLOAT);\n";
    }
    if(Ombre){
        code << "Vector3 dir_norm_"<< nom << " = Vector3Normalize(dir_" << nom << ");\n";
        code << "ombre_cam_" << nom << ".position = Vector3Scale(dir_norm_"<< nom <<", -20.0f);\n";
        code << "ombre_cam_" << nom << ".target = Vector3Zero();\n";
        code << "ombre_cam_" << nom << ".fovy = 40.0f;\n";
        code << "if (fabs(dir_norm_"<< nom <<".x) < 0.001f && fabs(dir_norm_"<< nom <<".z) < 0.001f) {ombre_cam_" << nom << ".up = Vector3{ 0.0f, 0.0f, -1.0f };}\n";
        code << "else{ombre_cam_" << nom << ".up = Vector3{ 0.0f, 1.0f, 0.0f };}\n";
    }

    return code.str();
}

std::string SoleilNode::GetDrawCode(){
    return "";
}

std::string SoleilNode::GetCleanupCode(){
    //nettoyage
    if(!Ombre) return "";
    std::stringstream code;
    code << "UnloadShadowmapRenderTexture(texture_ombre_" << nom << ");\n";
    return code.str();
}

std::string SoleilNode::GetInitCode(EditorContext& variables){
    std::stringstream code;
    //on donne nos loc ? je sais pas si c'est pas dans le shader qu'on fait ça plutot
    code << "Vector3 dir_" << nom << "= Vector3{" << std::to_string(direction.x) <<"f,"<< std::to_string(direction.y) <<"f,"<< std::to_string(direction.z)<<"f};\n";
    code << "Vector3 couleur_" << nom << "= {" << std::to_string(couleur_lumiere.x) <<"f,"<< std::to_string(couleur_lumiere.y) <<"f,"<< std::to_string(couleur_lumiere.z)<<"f};\n";
    code << "float puissance_" << nom << " = " << std::to_string(puissance) <<"f;\n";
    if(Ombre){
        code << "int slot_" << nom << " = 12;\n";
        code << "RenderTexture2D texture_ombre_" << nom << " = LoadShadowmapRenderTexture(" << std::to_string(res_shadowmap) << "," << std::to_string(res_shadowmap) << ");\n";
        code << "Camera3D ombre_cam_" << nom << " = {0};\n";
        code << "ombre_cam_" << nom <<".projection = CAMERA_ORTHOGRAPHIC;\n";
        code << "ombre_cam_" << nom <<".position = Vector3{" << std::to_string(position.x) <<"f,"<< std::to_string(position.y) <<"f,"<< std::to_string(position.z)<<"f};\n";
        code << "ombre_cam_" << nom <<".fovy = 40.0f;\n";
        code << "Matrix lightViewProj_" << nom <<";\n";
    }
        //les locs pour chaque shader de la banque
        //std::string idx = std::to_string(this->index); pas besoin on en a qu'un seul soleil
        for (const auto& [nom_shader, _] : variables.gestionnaire_shaders->dictionnaire_shaders) {
            std::string s_var = FormaterNomVariableShader(nom_shader);

            code << "    int loc_dir_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"sunDirection\");\n";
            code << "    int loc_col_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"sunColor\");\n";
            code << "    int loc_pow_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"sunPower\");\n";
            if(Ombre){
                code << "    int loc_vp_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"lightVP\");\n";
                code << "    int loc_shadow_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"shadowMap\");\n";
            }
        }
        return code.str();
}

BoundingBox SoleilNode::GetBoiteCollision(){
    Vector3 min, max;
    BoundingBox ma_bounding_box;
    float rayon_selection = 0.5f;
    min.x = position.x - rayon_selection;
    min.y = position.y - rayon_selection;
    min.z = position.z - rayon_selection;
    max.x = position.x + rayon_selection;
    max.y = position.y + rayon_selection;
    max.z = position.z + rayon_selection;

    ma_bounding_box.min = min;
    ma_bounding_box.max = max;
    return ma_bounding_box;
}


static RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
{
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(); // Load an empty framebuffer
    target.texture.width = width;
    target.texture.height = height;

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create depth texture
        // NOTE: No need a color texture attachment for the shadowmap
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19; // DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach depth texture to FBO
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

// Unload shadowmap render texture from GPU memory (VRAM)
static void UnloadShadowmapRenderTexture(RenderTexture2D target)
{
    if (target.id > 0)
    {
        // NOTE: Depth texture/renderbuffer is automatically
        // queried and deleted before deleting framebuffer
        rlUnloadFramebuffer(target.id);
    }
}