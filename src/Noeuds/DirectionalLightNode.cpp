#include "DirectionalLightNode.h"

SoleilNode::SoleilNode(){
    position = {0.0f, 0.0f, 0.0f};
    direction = {0.0f, -1.0f, 0.0f};
    couleur_lumiere = {1.0f, 1.0f, 1.0f};
    puissance = 1.0f;
    est_allume = true;
    type = "SoleilNode";
}

void SoleilNode::Update(EditorContext& variables){
    if (!est_allume) {
        //eteint puissance = 0
        variables.variables_globales["sunPower"] = 0.0f;
        return;
    }

    //soleil : PAS de compteur_lumieres
    //variables uniques.
    variables.variables_globales["sunDirection"] = this->direction;
    variables.variables_globales["sunColor"] = this->couleur_lumiere;
    variables.variables_globales["sunPower"] = this->puissance;
}


void SoleilNode::Draw(EditorContext& variables){
    //TODO dessiner un triangle ou une fleche qui pointe vers la direction du noeud 
    DrawSphereWires(position, 0.5f,16,16, YELLOW);
    Matrix matRot = MatrixRotateXYZ((Vector3){ rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD });
    Vector3 direction_par_defaut = { 0.0f, -1.0f, 0.0f };
    this->direction = Vector3Transform(direction_par_defaut, matRot);
    float longueur_direction = Vector3Length(direction);
    if (longueur_direction > 0.001f) {
        
        // 3. On normalise (ramène la longueur à 1)
        Vector3 dir_norm = Vector3Normalize(direction);
        
        // 4. On calcule le point final de la ligne (ex: longueur de 3.0 unités)
        // fin = position + (direction * 3.0)
        Vector3 fin_ligne = Vector3Add(position, Vector3Scale(dir_norm, 3.0f));

        // 5. On dessine la ligne qui montre où la lumière frappe !
        DrawLine3D(position, fin_ligne, YELLOW);

        // Optionnel : Si l'objet est sélectionné, on dessine une petite "pointe" rouge 
        // au bout de la ligne pour bien voir le sens de la flèche
        if (this->isSelected) {
            DrawSphereWires(fin_ligne, 0.2f, 8, 8, RED);
        }
    }
}

std::unique_ptr<SceneNode> SoleilNode::Cloner(){
    auto clone = std::make_unique<SoleilNode>(); //faut en cree un autre pour eviter les crash de mémoire
    clone->position = this->position;
    clone->taille = this->taille;
    clone->couleur = this->couleur;
    clone->direction = this->direction;
    clone->puissance = this->puissance;
    clone->est_allume = this->est_allume;
    clone->isSelected = true;
    static unsigned int compteur_Soleil_clones = 0; //logiquement on a un compteur pour les clones
    clone->nom = this->nom + "_" + std::to_string(compteur_Soleil_clones); // nouveau nom
    compteur_Soleil_clones++;
    return clone;
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

    return code.str();
}

std::string SoleilNode::GetDrawCode(){
    return "";
}

std::string SoleilNode::GetInitCode(EditorContext& variables){
    std::stringstream code;
    //on donne nos loc ? je sais pas si c'est pas dans le shader qu'on fait ça plutot
    code << "Vector3 dir_"<< nom << "= Vector3{" << std::to_string(direction.x) <<"f,"<< std::to_string(direction.y) <<"f,"<< std::to_string(direction.z)<<"f};\n";
    code << "Vector3 couleur_"<< nom << "= {" << std::to_string(couleur_lumiere.x) <<"f,"<< std::to_string(couleur_lumiere.y) <<"f,"<< std::to_string(couleur_lumiere.z)<<"f};\n";
        code << "float puissance_" << this->nom << " = " << std::to_string(puissance) <<"f;\n";

    //les locs pour chaque shader de la banque
    //std::string idx = std::to_string(this->index); pas besoin on en a qu'un seul soleil
    for (const auto& [nom_shader, _] : variables.gestionnaire_shaders->dictionnaire_shaders) {
        std::string s_var = FormaterNomVariableShader(nom_shader);

        code << "    int loc_dir_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"sunDirection\");\n";
        code << "    int loc_col_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"sunColor\");\n";
        code << "    int loc_pow_" << nom << "_" << s_var << " = GetShaderLocation(" << s_var << ", \"sunPower\");\n";
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

