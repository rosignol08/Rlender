#pragma once
#include "raylib.h"
#include <vector>
#include <variant>
#include <unordered_map>
//structure qui regroupe toutes les variables volantes de l'éditeur
struct EditorContext {
    int compteurModifs = 0; //un compteur pour dire que
    bool flag_changements = false;
    float cubePosition[3] = {0.0f, 0.0f, 0.0f};
    
    //la camera
    bool modeFlyActif = false;
    float tempsMaintien = 0.0f;//le temps actuel accumulé
    float tempsExige = 0.5f;//TODO issue #7
    char axe_en_cours = '0';

    CameraMode modeCameraActif = CAMERA_FIRST_PERSON;
    bool perspect = true;
    bool orto = false;
    int type_projection_camera = CAMERA_PERSPECTIVE;
    std::string code_preview;

    //Shaders
    std::string codeFragmentShader = "#version 330"
"\n// Input vertex attributes (from vertex shader)"
"\nin vec2 fragTexCoord;"
"\nin vec4 fragColor;"
"\n// Input uniform values"
"\nuniform sampler2D texture0;"
"\nuniform vec4 colDiffuse;"
"\n// Output fragment color"
"\nout vec4 finalColor;"
"\n// NOTE: Add here your custom variables"
"\nvoid main(){"
"\n    // Texel color fetching from texture sampler"
"\n    vec4 texelColor = texture(texture0, fragTexCoord);"
"\n"
"\n    // NOTE: Implement here your fragment shader code"
"\n"
"\n    // final color is the color from the texture "
"\n    //    times the tint color (colDiffuse)"
"\n    //    times the fragment color (interpolated vertex color)"
"\n    finalColor = texelColor*colDiffuse*fragColor;"
"\n    finalColor = vec4(1.0, 0.0, 0.0, 1.0);"
"\n}";//shader rouge par défaut
std::string nom_nouveau_shader = "ShaderLive";
std::vector <std::string> liste_log;//les logs
bool defiler_log = false;

//liste des uniform possibles
using UniformValue = std::variant<int, float, Vector2, Vector3, Vector4, Matrix>;
//dictionaire globale des uniforms :
std::unordered_map<std::string, UniformValue> variables_globales;

};

struct Parametres {
    int limiteSauvgarde = 50;
    int Plein_ecran = 0;
    int screenWidth = 1280; //c'est la taille de la fenetre
    int screenHeight = 720; //c'est la taille de la fenetre
    float Chronos_sauvegarde = 0.0f; //on attend ce temps avant d'ecrire sur le json (pour épargner le disque dur)
    bool attente_sauvegarde; //flag
    int nb_lignes_max_console = 500;
};

