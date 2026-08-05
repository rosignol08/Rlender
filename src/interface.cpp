#include "interface.h"

void gere_interface(SceneManager& La_scene, Camera3D& cameraEditeur, EditorContext& ctx){
    rlImGuiBegin();
    ImGui::Begin("Inspecteur");
    if (noeuds_selectione != nullptr)
    {
        // affiche le nom de l'objet tout en haut
        ImGui::Text("Modification de : %s", noeuds_selectione->nom.c_str());
        ImGui::Separator();

        // sliders pour modifier dynamiquement les variables
        if (
            // TODO ajouter les bouton pour ajouter des objetsg ici aussi
            ImGui::DragFloat3("Position", &noeuds_selectione->position.x, 0.1f) || ImGui::DragFloat3("Rotation", &noeuds_selectione->rotation.x, 1.0f) || ImGui::DragFloat3("Taille", &noeuds_selectione->taille.x, 0.1f))
        {
            flag_changements = true;
        }

        // Pour la couleur, c'est un peu plus complexe car ImGui utilise des floats (0.0 à 1.0)
        // et Raylib des unsigned char (0 à 255), on fera ça plus tard si tu veux.
    }
    else
    {
        ImGui::Text("Cliquez sur un objet dans la hierarchie.");
    }

    ImGui::Text("Salut !");
    if (
        // slider qui modifie directement les coordonnées du cube j'utilise les effets de bords
        ImGui::DragFloat3("TEST Position Cube", cubePosition, 0.1f))
    {
        flag_changements = true;
    }
    ImGui::End();
    ImGui::Begin("Hierarchie");
    for (size_t i = 0; i < La_scene.SceneManager::GetNodes().size(); i++)
    {
        // un label unique pour chaque objet
        std::string label = La_scene.SceneManager::GetNodes()[i]->nom + "##" + std::to_string(i);

        // faut mettre a jour le pointeur selectioneur
        if (ImGui::Selectable(label.c_str(), noeuds_selectione == La_scene.GetNodes()[i].get()))
        {

            // faut désélectionner l'ancien et selectionner le nouveau mais c'est fait par la fonction setseleciton
            La_scene.SceneManager::SetSelection(La_scene.SceneManager::GetNodes()[i].get());

            flag_changements = true;
        }
    }
    ImGui::End();

    // la partie caméra
    // la progress bar stylée qui apparait si on commence à maintenir le clic
    if (!modeFlyActif && tempsMaintien > 0.0f)
    {
        // recup le centre de l'ecant
        float centreX = GetScreenWidth() / 2.0f;
        float centreY = GetScreenHeight() / 2.0f;

        // ImGui place la prochaine fenêtre au centre
        ImGui::SetNextWindowPos({centreX, centreY}, ImGuiCond_Always, {0.5f, 0.5f});

        // desactivation de tous
        ImGuiWindowFlags flagsFlottant = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;

        // ouvre la fenêtre fantôme
        ImGui::Begin("BarreChargement", nullptr, flagsFlottant);

        // dessin de la barre issue #4
        ImGui::ProgressBar(tempsMaintien / tempsExige, {150.0f, 15.0f});
        ImGui::End();
    }
    ImGui::Begin("Contrôle Caméra");
    ImGui::Text("Mode de déplacement :");
    if (modeFlyActif)
    {
        UpdateCamera(&cameraEditeur, modeCameraActif);
    }
    else
    {
        if (ImGui::RadioButton("Première Personne (FPS)", modeCameraActif == CAMERA_FIRST_PERSON))
        {
            modeCameraActif = CAMERA_FIRST_PERSON;
        }
        if (ImGui::RadioButton("Caméra Libre (Free)", modeCameraActif == CAMERA_FREE))
        {
            modeCameraActif = CAMERA_FREE;
        }
        if (ImGui::RadioButton("Caméra Orbitale (Orbital)", modeCameraActif == CAMERA_ORBITAL))
        {
            modeCameraActif = CAMERA_ORBITAL;
        }
        if (ImGui::RadioButton("Troisième Personne (TPS)", modeCameraActif == CAMERA_THIRD_PERSON))
        {
            modeCameraActif = CAMERA_THIRD_PERSON;
        }
    }

    ImGui::Separator();
    ImGui::Text("Maintenez le Clic Droit pour naviguer !");
    ImGui::End();

    // pour changer le type de la caméra
    ImGui::Begin("Type de caméra");
    if (ImGui::Button("Perspective", {30.0f, 30.0f}))
    {
        // si on clique sur ce bouton ça change le mode
        if (!perspect)
        {
            perspect = true;
            orto = false;
            type_projection_camera = CAMERA_PERSPECTIVE;
            // on met a jour la perspective
            cameraEditeur.projection = type_projection_camera;
        }
    }
    if (ImGui::Button("Ortogonal", {30.0f, 30.0f}))
    { // TODO issue #6
        // idem ici
        if (!orto)
        {
            orto = true;
            perspect = false;
            type_projection_camera = CAMERA_ORTHOGRAPHIC;
            // on met a jour la perspective
            cameraEditeur.projection = type_projection_camera;
        }
    }

    ImGui::End();
    rlImGuiEnd();
}