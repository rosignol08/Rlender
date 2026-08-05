#include <vector>
#include <iostream>
#include <memory>//pour les unique ptr
#include "SceneNode.h"
#include "Sauvgarde.h"
#include "SceneManager.h"
#include "interface.h"

int main(void) {

    /*
    - la partie sauvgade
    en gros on peut utiliser un compteur pour dire apres 5 ou 10 modif on enregistre le fichier sinon on garde en ram
    pour eviter la sur utilisation du disque dur ssd etc
    ça doit etre modifiable dans les parametres du logiciel plus tard
    limiteSauvgarde c'est la limite dynamique du compteur c'est ça que faut changer pour reduite au augmenter le nombre de sauvgardes etc
    */
    int compteurModifs = 0; //un compteur pour dire que
    int limiteSauvgarde = 50;
    bool flag_changements = false;//le flag pour dire si un changement a été fait
    std::string contenu = ""; //c'est un pointeur sur la stack le vrai texte est sur le tas donc pas de soucis de taille c'est dans la ram :)

    //pour gerer les objets de la scene:

    SceneManager La_scene;

    //la fenêtre Raylib pour voir le rendu
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Rlender - Test ImGui");
    SetTargetFPS(60);

    /*
    la camera 3D pour voir la scene
    ici je défini la camera et apres on va avoir une section pour changer son type dynamiquement
    */
    CameraMode modeCameraActif = CAMERA_FIRST_PERSON;
    CameraProjection type_projection_camera = CAMERA_PERSPECTIVE; //type de base
    Camera3D cameraEditeur = { 0 };
    cameraEditeur.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    cameraEditeur.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    cameraEditeur.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    cameraEditeur.fovy = 45.0f;
    cameraEditeur.projection = type_projection_camera;//possibilité de changer ça apres

    bool perspect = true;
    bool orto = false;
    float tempsMaintien = 0.0f;//le temps actuel accumulé
    float tempsExige = 0.5f;//TODO issue #7
    bool modeFlyActif = false;//pour savoir si on est en mode fly
    //init rlImGui
    rlImGuiSetup(true);

    // Variable pour la position de notre cube test
    float cubePosition[3] = { 0.0f, 0.0f, 0.0f };

    // Boucle principale
        while (!WindowShouldClose()) {
            //gestion de la camera de l'editeur
            //si le clic droit est maintenu
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                //on compte le temps
                tempsMaintien += GetFrameTime();
                //si on a appuié assez de temps et qu'on est pas déjà entrain de voler
                if(tempsMaintien >= tempsExige && !modeFlyActif){
                    modeFlyActif = true;
                    DisableCursor();
                }
                if(modeFlyActif){
                    UpdateCamera(&cameraEditeur, modeCameraActif);
                }
            }

            //detec du moment où on relache le clic droit
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                tempsMaintien = 0.0f;//on reset le temps
                if(modeFlyActif){
                    EnableCursor(); //ça libere la souris une seule fois
                    modeFlyActif = false;
                }
            }

        // DESSIN idée de base
        BeginDrawing();
            ClearBackground(DARKGRAY);
            //TODO faire un vrai truc ici
            BeginMode3D(cameraEditeur);
            //cette ligne dessine tout
            La_scene.DrawScene();
            DrawGrid(10, 1.0f);
            EndMode3D();
            
            //variable temporaire pour les acces repeté du pointeur pour pas réécrire "La_scene.GetSelection()->" à chaque fois
            SceneNode* noeuds_selectione = La_scene.SceneManager::GetSelection();

            // B. Dessin de l'interface graphique (Toujours APRÈS la 3D)
            gere_interface();

            DrawFPS(10, 10);//pour debug si le logiciel tourne bien

        EndDrawing();
        if(flag_changements){
            //si on a eu un changement on augmente le compteur
            compteurModifs++;
            //faut regenerer le code
            contenu = GenererCodeComplet(La_scene.SceneManager::GetNodes());//on donne à manger tous les noeuds de la scene
            flag_changements = false; //faut penser à le rebaisser le flag hein
            //std::cout << "changement : " << compteurModifs << std::endl;
        }
        if(compteurModifs >= limiteSauvgarde){
            //la je peut lancer la sauvgarde
            Sauvgarde("projet_exemple.cpp",contenu);
            compteurModifs = 0;
        }
    }

    // Nettoyage
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}


