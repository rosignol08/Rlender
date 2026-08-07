#pragma once
#include "raylib.h"

//structure qui regroupe toutes les variables volantes de l'éditeur
struct EditorContext {
    bool flag_changements = false;
    float cubePosition[3] = {0.0f, 0.0f, 0.0f};
    
    //la camera
    bool modeFlyActif = false;
    float tempsMaintien = 0.0f;//le temps actuel accumulé
    float tempsExige = 0.5f;//TODO issue #7
    
    CameraMode modeCameraActif = CAMERA_FIRST_PERSON;
    bool perspect = true;
    bool orto = false;
    int type_projection_camera = CAMERA_PERSPECTIVE;
};

struct Parametres {
    int limiteSauvgarde = 50;
    int Plein_ecran = 0;
    int screenWidth = 1280; //c'est la taille de la fenetre
    int screenHeight = 720; //c'est la taille de la fenetre
};