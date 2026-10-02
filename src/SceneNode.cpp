#include "SceneNode.h"
void SceneNode::AppliquerVariablesGlobales(EditorContext& variables, Shader shader_cible){
    int loc = 0; //pour les locs
    for(auto & [nom,valeur] : variables.variables_globales){
        loc = GetShaderLocation(shader_cible, nom.c_str());
        if(loc != -1){
            //on ouvre le variant dans "valeur"
            if(std::holds_alternative<int>(valeur)){
                //si la valeur est int
                int la_valeur = std::get<int>(valeur);
                SetShaderValue(shader_cible, loc, &la_valeur, SHADER_UNIFORM_INT);
            }else if(std::holds_alternative<float>(valeur)){
                //si la valeur est float
                float la_valeur = std::get<float>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_FLOAT);
            }else if(std::holds_alternative<Vector2>(valeur)){
                //si la valeur est Vector2
                Vector2 la_valeur = std::get<Vector2>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_VEC2);
            }else if(std::holds_alternative<Vector3>(valeur)){
                //si la valeur est Vector3
                Vector3 la_valeur = std::get<Vector3>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_VEC3);
            }else if(std::holds_alternative<Vector4>(valeur)){
                //si la valeur est Vector4
                Vector4 la_valeur = std::get<Vector4>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_VEC4);
            }else if(std::holds_alternative<Matrix>(valeur)){//on a besoin de faire ça ou on fait else ?
                Matrix la_valeur = std::get<Matrix>(valeur);
                SetShaderValueMatrix(shader_cible,loc,la_valeur);
            }else{
                //erreur on a pas le type ?
                continue;
            }
        }else{
            continue;
        }
    }
}