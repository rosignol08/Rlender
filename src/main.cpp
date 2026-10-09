#include <vector>
#include <iostream>
#include <memory>//pour les unique ptr
#include "Noeuds/SceneNode.h"
#include "Sauvgarde.h"
#include "Managers/SceneManager.h"
#include "Interface.h"

int main(void) {

    /*
    - la partie sauvgade
    en gros on peut utiliser un compteur pour dire apres 5 ou 10 modif on enregistre le fichier sinon on garde en ram
    pour eviter la sur utilisation du disque dur ssd etc
    ça doit etre modifiable dans les parametres du logiciel plus tard
    limiteSauvgarde c'est la limite dynamique du compteur c'est ça que faut changer pour reduite au augmenter le nombre de sauvgardes etc
    */
    
    std::string contenu = ""; //c'est un pointeur sur la stack le vrai texte est sur le tas donc pas de soucis de taille c'est dans la ram :)

    //la fenêtre Raylib pour voir le rendu
    
    Parametres Les_parametres;
    ChargerConfig(Les_parametres);//au debut du programme on met a jour les parametres
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);//pour pouvoir changer la taille
    InitWindow(Les_parametres.screenWidth, Les_parametres.screenHeight, "Rlender - Test ImGui");
    if (Les_parametres.Plein_ecran == 1) {
        MaximizeWindow();//si on a le plein ecran a mettre au demarage
    }
    SetTargetFPS(60);
    
    //pour gerer les objets de la scene:

    SceneManager La_scene;

    EditorContext Les_variables;
    Les_variables.banque_materiaux = &La_scene.banque_materiaux;//connexion des deux

    //les logs
    Initialiser_Logs(&Les_variables);
    SetTraceLogCallback(Fonction_Log);
    
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
    
    std::cout << "TEST FBO - Preview X: " << Les_parametres.res_preview_x << " Y: " << Les_parametres.res_preview_y << std::endl;
    RenderTexture2D CameratexturePreview = LoadRenderTexture(640, 360);
    std::cout << "TEST FBO - REUSSI !" << std::endl;

    rlImGuiSetup(true);
    // Boucle principale
        while (!WindowShouldClose()) {
            //gestion de la camera de l'editeur
            //si le clic droit est maintenu
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                //on compte le temps
                Les_variables.tempsMaintien += GetFrameTime();
                //si on a appuié assez de temps et qu'on est pas déjà entrain de voler
                if(Les_variables.tempsMaintien >= Les_variables.tempsExige && !Les_variables.modeFlyActif){
                    Les_variables.modeFlyActif = true;
                    DisableCursor();
                }
                if(Les_variables.modeFlyActif){
                    UpdateCamera(&cameraEditeur, modeCameraActif);
                }
            }

            //detec du moment où on relache le clic droit
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                Les_variables.tempsMaintien = 0.0f;//on reset le temps
                if(Les_variables.modeFlyActif){
                    EnableCursor(); //ça libere la souris une seule fois
                    Les_variables.modeFlyActif = false;
                }
            }
        La_scene.Update(cameraEditeur, Les_variables);
        // DESSIN idée de base
        
        std::vector<SceneNode*> selection = La_scene.GetSelection();
        if (!selection.empty() && selection[0]->type == "Camera3D") {
                CameraNode* camNode = dynamic_cast<CameraNode*>(selection[0]);
                if (camNode) {
                    BeginTextureMode(CameratexturePreview);
                        ClearBackground(SKYBLUE);
                        BeginMode3D(camNode->ObtenirCameraRaylib());
                            //draw la scene point de vue de la camera active
                            for (auto& noeud : La_scene.GetNodes()) {
                                if (noeud->type != "Camera3D"){
                                    noeud->Draw(Les_variables);
                                    //std::cout << "dessin" << std::endl;
                                }
                            }
                            EndMode3D();
                            EndTextureMode();
                        }
                    }
                    
        BeginDrawing();
            ClearBackground(DARKGRAY);
            //TODO faire un vrai truc ici
            BeginMode3D(cameraEditeur);
            //cette ligne dessine tout
            La_scene.DrawScene(Les_variables);
            DrawGrid(1000, 1.0f);
            EndMode3D();

            if(IsWindowResized()&& !IsWindowState(FLAG_WINDOW_MAXIMIZED)){
                //si la fenetre a ete changé de taille faut enregistrer les valeurs
                Les_parametres.screenWidth = GetScreenWidth();
                Les_parametres.screenHeight = GetScreenHeight();
                Les_parametres.Chronos_sauvegarde = 0.5f;// le temps avant de sauvgarder
                Les_parametres.attente_sauvegarde = true; //tant qu'on change la taille de la fenetre on enregistre pas
            }
            if (Les_parametres.attente_sauvegarde) {
                Les_parametres.Chronos_sauvegarde -= GetFrameTime();
            }
            if (Les_parametres.attente_sauvegarde && Les_parametres.Chronos_sauvegarde <= 0.0f) {
                //enregistrement
                SauvegarderConfig(Les_parametres);
                //reset du flag
                Les_parametres.attente_sauvegarde = false;
            }
            
            //l'interface graphique (apres la 3D)
            gere_interface(La_scene, cameraEditeur, Les_variables, Les_parametres, CameratexturePreview);

            DrawFPS(10, 10);//pour debug si le logiciel tourne bien

        EndDrawing();
        
    }

    //nettoyage
    rlImGuiShutdown();
    La_scene.shaderManager.Nettoyer_tout();
    La_scene.ViderScene();
    UnloadRenderTexture(CameratexturePreview);
    CloseWindow();

    return 0;
}


