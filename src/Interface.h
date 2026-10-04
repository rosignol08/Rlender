#pragma once
#include "raylib.h"
#include "Managers/SceneManager.h"
#include "utils.h"
#include "imgui.h"
#include "rlImGui.h"
#include "Sauvgarde.h"
#include "tinyfiledialogs.h"
#include "Noeuds/LightNode.h"
#include "Noeuds/DirectionalLightNode.h"
//pour l'interface utilisateur pour reduire la taille du code dans main.cpp
void gere_interface(SceneManager& La_scene, Camera3D& cameraEditeur, EditorContext& les_parametres, Parametres &Les_parametres);
void Fonction_Log(int type_message, const char *texte,  va_list arguments);
void Initialiser_Logs(EditorContext* contexte_cible);