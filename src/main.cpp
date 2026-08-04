#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include <vector>
#include <iostream>
#include <memory>//pour les unique ptr
#include "SceneNode.h"
#include "Sauvgarde.h"
#include "SceneManager.h"

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

    //la camera 3D pour voir la scene
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    //init rlImGui
    rlImGuiSetup(true);

    // Variable pour la position de notre cube test
    float cubePosition[3] = { 0.0f, 0.0f, 0.0f };

    // Boucle principale
    while (!WindowShouldClose()) {
        
        // DESSIN idée de base
        BeginDrawing();
            ClearBackground(DARKGRAY);
            //TODO faire un vrai truc ici
            BeginMode3D(camera);
            //cette ligne dessine tout
            La_scene.DrawScene();
            DrawGrid(10, 1.0f);
            EndMode3D();
            
            //variable temporaire pour les acces repeté du pointeur pour pas réécrire "La_scene.GetSelection()->" à chaque fois
            SceneNode* noeuds_selectione = La_scene.SceneManager::GetSelection();

            // B. Dessin de l'interface graphique (Toujours APRÈS la 3D)
            rlImGuiBegin();
                ImGui::Begin("Inspecteur");
                if (noeuds_selectione != nullptr){
                    //affiche le nom de l'objet tout en haut
                    ImGui::Text("Modification de : %s", noeuds_selectione->nom.c_str());
                    ImGui::Separator();
                
                    //sliders pour modifier dynamiquement les variables
                    if(
                        //TODO ajouter les bouton pour ajouter des objetsg ici aussi
                        ImGui::DragFloat3("Position", &noeuds_selectione->position.x, 0.1f)
                        ||ImGui::DragFloat3("Rotation", &noeuds_selectione->rotation.x, 1.0f)
                        ||ImGui::DragFloat3("Taille", &noeuds_selectione->taille.x, 0.1f)
                    ){
                        flag_changements = true;
                    }

                    // Pour la couleur, c'est un peu plus complexe car ImGui utilise des floats (0.0 à 1.0) 
                    // et Raylib des unsigned char (0 à 255), on fera ça plus tard si tu veux.
                } else {
                    ImGui::Text("Cliquez sur un objet dans la hierarchie.");
                }

                ImGui::Text("Bienvenue dans ton editeur !");
                if(

                    //slider qui modifie directement les coordonnées du cube j'utilise les effets de bords
                    ImGui::DragFloat3("Position Cube", cubePosition, 0.1f)
                ){
                    flag_changements = true;
                }
                ImGui::End();
                ImGui::Begin("Hierarchie");
                for (size_t i = 0; i < La_scene.SceneManager::GetNodes().size(); i++) {
                    //un label unique pour chaque objet
                    std::string label = La_scene.SceneManager::GetNodes()[i]->nom + "##" + std::to_string(i);

                    //faut mettre a jour le pointeur selectioneur
                    if (ImGui::Selectable(label.c_str(), noeuds_selectione == La_scene.GetNodes()[i].get())) {

                        //faut désélectionner l'ancien et selectionner le nouveau mais c'est fait par la fonction setseleciton
                        La_scene.SceneManager::SetSelection(La_scene.SceneManager::GetNodes()[i].get());
                        
                        flag_changements = true;
                    }
                    
                }
                ImGui::End();
            rlImGuiEnd();
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
