#pragma once
#include "raylib.h"
#include "SceneManager.h"
#include "utils.h"
#include "imgui.h"
#include "rlImGui.h"

//pour l'interface utilisateur pour reduire la taille du code dans main.cpp
void gere_interface(SceneManager& La_scene, Camera3D& cameraEditeur, EditorContext& les_parametres);