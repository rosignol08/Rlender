#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include <vector>
#include <iostream>
#include <memory>//pour les unique ptr
#include "SceneNode.h"

int main(void) {

    /*
    - la partie sauvgade
    en gros on peut utiliser un compteur pour dire apres 5 ou 10 modif on enregistre le fichier sinon on garde en ram
    pour eviter la sur utilisation du disque dur ssd etc
    ça doit etre modifiable dans les parametres du logiciel plus tard
    limiteSauvgarde c'est la limite dynamique du compteur c'est ça que faut changer pour reduite au augmenter le nombre de sauvgardes etc
    */
    int compteurModifs = 0; //un compteur pour dire que
    int limiteSauvgarde = 5;
    bool flag_changements = false;//le flag pour dire si un changement a été fait

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

    //tous nos elements dans la scene
    std::vector<std::unique_ptr<SceneNode>> sceneNodes;
    
    //test ajout elements
    sceneNodes.push_back(std::make_unique<CubeNode>());

    sceneNodes.back()->position = {2.0f, 0.0f, 0.0f};
    //pointeur pour retenir quel objet on est en train de modifier
    SceneNode* noeudSelectionne = nullptr;
    // Boucle principale
    while (!WindowShouldClose()) {
        
        // DESSIN idée de base
        BeginDrawing();
            ClearBackground(DARKGRAY);
            //TODO faire un vrai truc ici
            // A. Dessin de la scène 3D
            BeginMode3D(camera);
            for (auto& node : sceneNodes) {
                node->Draw(); // Appellera le bon Draw() selon si c'est un Cube ou une Sphère !
            }
                DrawGrid(10, 1.0f);
            EndMode3D();

            // B. Dessin de l'interface graphique (Toujours APRÈS la 3D)
            rlImGuiBegin();
                ImGui::Begin("Inspecteur");
                if (noeudSelectionne != nullptr) {
                    //affiche le nom de l'objet tout en haut
                    ImGui::Text("Modification de : %s", noeudSelectionne->nom.c_str());
                    ImGui::Separator();
                
                    //sliders pour modifier dynamiquement les variables
                    if(
                        //TODO ajouter les bouton pour ajouter des objets ici aussi
                        ImGui::DragFloat3("Position", &noeudSelectionne->position.x, 0.1f);
                        ImGui::DragFloat3("Rotation", &noeudSelectionne->rotation.x, 1.0f);
                        ImGui::DragFloat3("Taille", &noeudSelectionne->taille.x, 0.1f);
                    ){
                        flag_changements = true;
                    }

                    // Pour la couleur, c'est un peu plus complexe car ImGui utilise des floats (0.0 à 1.0) 
                    // et Raylib des unsigned char (0 à 255), on fera ça plus tard si tu veux.
                } else {
                    ImGui::Text("Cliquez sur un objet dans la hierarchie.");
                }

                ImGui::Text("Bienvenue dans ton editeur !");
                // Ce slider va modifier directement les coordonnées du cube
                ImGui::DragFloat3("Position Cube", cubePosition, 0.1f);
                ImGui::Begin("Hierarchie");
                for (size_t i = 0; i < sceneNodes.size(); i++) {
                    //un label unique pour chaque objet
                    std::string label = sceneNodes[i]->nom + "##" + std::to_string(i);

                    //faut mettre a jour le pointeur selectioneur
                    if (ImGui::Selectable(label.c_str(), noeudSelectionne == sceneNodes[i].get())) {

                        //faut désélectionner l'ancien
                        if (noeudSelectionne != nullptr) noeudSelectionne->isSelected = false;

                        //et sélectionner le nouveau
                        noeudSelectionne = sceneNodes[i].get();
                        noeudSelectionne->isSelected = true;
                    }
                }
                ImGui::End();
            rlImGuiEnd();

        EndDrawing();
        if(flag_changements){
            //si on a eu un changement on augmente le compteur
            compteurModifs++;
        }
    }

    // Nettoyage
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
