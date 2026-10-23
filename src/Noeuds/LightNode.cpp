#include "LightNode.h"

static unsigned int compteur_Light_clones = 0; //compteur pour les lumières

LightNode::LightNode(){
    position = {0.0f, 0.0f, 0.0f};
    couleur_lumiere = {1.0f, 1.0f, 1.0f};
    intensite = 1.1;
    puissance = 5.0f;
    taille = 1.0f;
    type = "LightNode";
    //index et du nom par défaut
    this->index = compteur_Light_clones;
    this->nom = "Light_" + std::to_string(this->index);
    
    compteur_Light_clones++;
}



void LightNode::Update(EditorContext& variables){
    //limite de 4 lumières differentes
    if (!est_allume) return;
    if (variables.compteur_lumieres >= 4) return; 

    //tableau des lumières "[0]" etc
    std::string i = "[" + std::to_string(variables.compteur_lumieres) + "]";
    variables.variables_globales["lightPos" + i] = this->position;
    variables.variables_globales["lightColor" + i] = this->couleur_lumiere;
    variables.variables_globales["lightIntensity" + i] = this->intensite;
    variables.variables_globales["lightPower" + i] = this->puissance;

    variables.compteur_lumieres++;
    
    //maj du nombre total de lumières pour le shader
    variables.variables_globales["lightCount"] = variables.compteur_lumieres;
}


void LightNode::Draw(EditorContext& variables){
    DrawSphereWires(position, 0.5f,16,16, YELLOW);
    if (this->isSelected) {
        DrawSphereWires(position, intensite, 16, 16, Fade(YELLOW, 0.3f));
    }
}

std::unique_ptr<SceneNode> LightNode::Cloner(){
    auto clone = std::make_unique<LightNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->intensite = this->intensite;
    clone->puissance = this->puissance;
    clone->est_allume = this->est_allume;
    clone->isSelected = true;
    clone->nom = this->nom + "_" + std::to_string(compteur_Light_clones); // nouveau nom
    return clone;
}

std::string LightNode::GetUpdateCode(EditorContext& variables){
    std::stringstream code;

    //maj position et couleur
    code << "    pos_" << nom << " = { " 
         << std::to_string(position.x) << "f, " 
         << std::to_string(position.y) << "f, " 
         << std::to_string(position.z) << "f };\n";
    
    code << "    couleur_" << nom << " = { " 
         << std::to_string(couleur_lumiere.x) << "f, " 
         << std::to_string(couleur_lumiere.y) << "f, " 
         << std::to_string(couleur_lumiere.z) << "f };\n";

    //check les shaders enregistrés pour générer l'envoi vers CHACUN
    for (const auto& [nom_shader, shader_raylib] : variables.gestionnaire_shaders->dictionnaire_shaders) {
        //std::string s_var = "shader_" + nom_shader;
        std::string s_var = FormaterNomVariableShader(nom_shader);
        code << "    SetShaderValue(" << s_var << ", loc_pos_" << this->nom << "_" << nom_shader << ", &pos_" << this->nom << ", SHADER_UNIFORM_VEC3);\n";
        code << "    SetShaderValue(" << s_var << ", loc_col_" << this->nom << "_" << nom_shader << ", &couleur_" << this->nom << ", SHADER_UNIFORM_VEC3);\n";
        code << "    SetShaderValue(" << s_var << ", loc_int_" << this->nom << "_" << nom_shader << ", &intensite_" << this->nom << ", SHADER_UNIFORM_FLOAT);\n";
        code << "    SetShaderValue(" << s_var << ", loc_pow_" << this->nom << "_" << nom_shader << ", &puissance_" << this->nom << ", SHADER_UNIFORM_FLOAT);\n";
    }

    return code.str();
}

std::string LightNode::GetDrawCode(){
    return "";
}

std::string LightNode::GetInitCode(EditorContext& variables){
    std::stringstream code;
    //on donne nos loc ? je sais pas si c'est pas dans le shader qu'on fait ça plutot
    code << "Vector3 pos_"<< nom << "= {" << std::to_string(position.x) <<"f,"<< std::to_string(position.y) <<"f,"<< std::to_string(position.z)<<"f};\n";
    code << "Vector3 couleur_"<< nom << "= {" << std::to_string(couleur_lumiere.x) <<"f,"<< std::to_string(couleur_lumiere.y) <<"f,"<< std::to_string(couleur_lumiere.z)<<"f};\n";
    code << "float intensite_" << this->nom << " = 10.0f;\n";
    code << "float puissance_" << this->nom << " = 1.0f;\n";

    //les locs pour chaque shader de la banque
    std::string idx = std::to_string(this->index);
    for (const auto& [nom_shader, _] : variables.gestionnaire_shaders->dictionnaire_shaders) {
        std::string s_var = FormaterNomVariableShader(nom_shader);

        code << "    int loc_pos_" << nom << "_" << nom_shader << " = GetShaderLocation(" << s_var << ", \"lightPos[" << idx << "]\");\n";
        code << "    int loc_col_" << nom << "_" << nom_shader << " = GetShaderLocation(" << s_var << ", \"lightColor[" << idx << "]\");\n";
        code << "    int loc_int_" << nom << "_" << nom_shader << " = GetShaderLocation(" << s_var << ", \"lightIntensity[" << idx << "]\");\n";
        code << "    int loc_pow_" << nom << "_" << nom_shader << " = GetShaderLocation(" << s_var << ", \"lightPower[" << idx << "]\");\n";
    }

    return code.str();
}

BoundingBox LightNode::GetBoiteCollision(){
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

