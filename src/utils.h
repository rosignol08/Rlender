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
"\n// Ce qui vient du Vertex Shader"
"\nin vec3 fragPosition;"
"\nin vec2 fragTexCoord;"
"\nin vec3 fragNormal;"
"\nin vec4 fragColor;"
"\n#define MAX_LIGHTS 4"
"\n// --- LES VARIABLES GLOBALES DE TON MOTEUR ---"
"\nuniform int lightCount;"
"\nuniform vec3 lightPos[MAX_LIGHTS];"
"\nuniform vec3 lightColor[MAX_LIGHTS];"
"\nuniform float lightIntensity[MAX_LIGHTS];"
"\nuniform vec3 viewPos;"
"\n"
"\n// Variables standards de Raylib (gérées automatiquement)"
"\nuniform sampler2D texture0;"
"\nuniform vec4 colDiffuse;"
"\n"
"\n// La couleur finale du pixel"
"\nout vec4 finalColor;"
"\n"
"\nvoid main(){"
"\n    // 1. Couleur de base de l'objet (texture * couleur)"
"\n    vec4 texelColor = texture(texture0, fragTexCoord);"
"\n    vec3 objetCouleur = texelColor.rgb * colDiffuse.rgb;"
"\n"
"\n    // 2. Lumière Ambiante"
"\n    float ambientStrength = 0.1;"
"\n    vec3 resultat = ambientStrength * objetCouleur;"
"\n"
"\n    // 3. Boucle sur les lumières"
"\n    vec3 norm = normalize(fragNormal);"
"\n    vec3 viewDir = normalize(viewPos - fragPosition);"
"\n    for(int i = 0; i < lightCount && i < MAX_LIGHTS; i++){"
"\n         // Calcul de l'atténuation avec la distance"
"\n         float distance = length(lightPos[i] - fragPosition);"
"\n         float attenuation = clamp(1.0 - (distance / lightIntensity[i]), 0.0, 1.0);"
"\n         attenuation *= attenuation;"
"\n"
"\n         // Diffuse (suppression de * lightIntensity)"
"\n         vec3 lightDir = normalize(lightPos[i] - fragPosition);"
"\n         float diff = max(dot(norm, lightDir), 0.0);"
"\n         vec3 diffuse = diff * lightColor[i] * objetCouleur;"
"\n"
"\n         // Spéculaire (suppression de * lightIntensity)"
"\n         vec3 reflectDir = reflect(-lightDir, norm);  "
"\n         float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);"
"\n         vec3 specular = 0.5 * spec * lightColor[i];"
"\n"
"\n         // On applique l'atténuation sur le résultat total de cette lumière !"
"\n         resultat += (diffuse + specular) * attenuation; "
"\n    }"
"\n"
"\n    finalColor = vec4(resultat, texelColor.a * colDiffuse.a);"
"\n}";

// Dans EditorContext
std::string codeVertexShader = "#version 330\n"
"in vec3 vertexPosition;\n"
"in vec2 vertexTexCoord;\n"
"in vec3 vertexNormal;\n"
"in vec4 vertexColor;\n"
"uniform mat4 mvp;\n"
"uniform mat4 matModel;\n"
"out vec3 fragPosition;\n"
"out vec2 fragTexCoord;\n"
"out vec3 fragNormal;\n"
"out vec4 fragColor;\n"
"void main(){\n"
"    fragPosition = vec3(matModel * vec4(vertexPosition, 1.0));\n"
"    fragTexCoord = vertexTexCoord;\n"
"    fragColor = vertexColor;\n"
"    fragNormal = normalize(vec3(matModel * vec4(vertexNormal, 0.0)));\n"
"    gl_Position = mvp * vec4(vertexPosition, 1.0);\n"
"}\n";

std::string nom_nouveau_shader = "ShaderLive";
int compteur_lumieres = 0;//pour les lumières


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

