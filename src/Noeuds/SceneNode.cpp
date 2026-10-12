#include "SceneNode.h"
void SceneNode::AppliquerVariablesGlobales(EditorContext& variables, Shader shader_cible){
    int loc = 0; //pour les locs
    static int cpt = 0;
    for(auto & [nom,valeur] : variables.variables_globales){
        loc = GetShaderLocation(shader_cible, nom.c_str());
        if(!(cpt %50)){

            std::cout << "nom : "<< nom<< std::endl;
            std::cout << "loc : "<< loc << std::endl;
        }
        cpt++;
        if(loc != -1){
            //on ouvre le variant dans "valeur"
            std::cout << "loc : "<< loc << std::endl;
            if(std::holds_alternative<int>(valeur)){
                //si la valeur est int
                int la_valeur = std::get<int>(valeur);
                SetShaderValue(shader_cible, loc, &la_valeur, SHADER_UNIFORM_INT);
                std::cout << "valeur trouve : int loc : " << loc << std::endl;
            }else if(std::holds_alternative<float>(valeur)){
                //si la valeur est float
                float la_valeur = std::get<float>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_FLOAT);
                std::cout << "valeur trouve : float loc : " << loc << std::endl;
            }else if(std::holds_alternative<Vector2>(valeur)){
                //si la valeur est Vector2
                Vector2 la_valeur = std::get<Vector2>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_VEC2);
                std::cout << "valeur trouve : Vector2 loc : " << loc << std::endl;
            }else if(std::holds_alternative<Vector3>(valeur)){
                //si la valeur est Vector3
                Vector3 la_valeur = std::get<Vector3>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_VEC3);
                
            }else if(std::holds_alternative<Vector4>(valeur)){
                //si la valeur est Vector4
                Vector4 la_valeur = std::get<Vector4>(valeur);
                SetShaderValue(shader_cible,loc, &la_valeur,SHADER_UNIFORM_VEC4);
                std::cout << "valeur trouve : Vector4 loc : " << loc << std::endl;
            }else if(std::holds_alternative<Matrix>(valeur)){//on a besoin de faire ça ou on fait else ?
                Matrix la_valeur = std::get<Matrix>(valeur);
                SetShaderValueMatrix(shader_cible,loc,la_valeur);
                std::cout << "valeur trouve : Matrix loc : " << loc << std::endl;
            }else{
                //erreur on a pas le type ?
                continue;
            }
        }else{
            continue;
        }
    }
}