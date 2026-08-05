#pragma once
#include "raylib.h"

//structure qui regroupe toutes les variables volantes de l'éditeur
struct EditorContext {
    bool flag_changements = false;
    float cubePosition[3] = {0.0f, 0.0f, 0.0f};
    
    //la camera
    bool modeFlyActif = false;
    float tempsMaintien = 0.0f;
    float tempsExige = 1.5f;
    CameraMode modeCameraActif = CAMERA_FIRST_PERSON;
    bool perspect = true;
    bool orto = false;
    int type_projection_camera = CAMERA_PERSPECTIVE;

    //les paramètres
    float tempsMaintien = 0.0f;//le temps actuel accumulé
    float tempsExige = 0.5f;//TODO issue #7
};