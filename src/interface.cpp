#include "interface.h"

void gere_interface(SceneManager& La_scene, Camera3D& cameraEditeur, EditorContext& Les_variables, Parametres &Les_parametres){
    //recuperation des variables etc
    SceneNode* noeuds_selectione = La_scene.GetSelection();
    rlImGuiBegin();
    ImGui::Begin("Inspecteur");
    if (noeuds_selectione != nullptr){
        // affiche le nom de l'objet tout en haut
        ImGui::Text("Modification de : %s", noeuds_selectione->nom.c_str());
        ImGui::Separator();

        // sliders pour modifier dynamiquement les variables
        if (
            // TODO ajouter les bouton pour ajouter des objetsg ici aussi
            ImGui::DragFloat3("Position", &noeuds_selectione->position.x, 0.1f) || ImGui::DragFloat3("Rotation", &noeuds_selectione->rotation.x, 1.0f) || ImGui::DragFloat3("Taille", &noeuds_selectione->taille.x, 0.1f))
        {
            Les_variables.flag_changements = true;
        }

        // Pour la couleur, c'est un peu plus complexe car ImGui utilise des floats (0.0 à 1.0)
        // et Raylib des unsigned char (0 à 255), on fera ça plus tard si tu veux.
    }
    else
    {
        ImGui::Text("Cliquez sur un objet dans la hierarchie.");
    }

    //ImGui::Text("Salut !");
    //if (
    //    // slider qui modifie directement les coordonnées du cube j'utilise les effets de bords
    //    ImGui::DragFloat3("TEST Position Cube", cubePosition, 0.1f))
    //{
    //    les_parametres.flag_changements = true;
    //}
    ImGui::End();
    ImGui::Begin("Hierarchie");
    for (size_t i = 0; i < La_scene.GetNodes().size(); i++)
    {
        // un label unique pour chaque objet
        std::string label = La_scene.GetNodes()[i]->nom + "##" + std::to_string(i);

        // faut mettre a jour le pointeur selectioneur
        if (ImGui::Selectable(label.c_str(), noeuds_selectione == La_scene.GetNodes()[i].get()))
        {

            // faut désélectionner l'ancien et selectionner le nouveau mais c'est fait par la fonction setseleciton
            La_scene.SceneManager::SetSelection(La_scene.GetNodes()[i].get());

            Les_variables.flag_changements = true;
        }
    }
    ImGui::End();

    // la partie caméra
    // la progress bar stylée qui apparait si on commence à maintenir le clic
    if (!Les_variables.modeFlyActif && Les_variables.tempsMaintien > 0.0f)
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
        ImGui::ProgressBar(Les_variables.tempsMaintien / Les_variables.tempsExige, {150.0f, 15.0f});
        ImGui::End();
    }
    ImGui::Begin("Contrôle Caméra");
    ImGui::Text("Mode de déplacement :");
    if (Les_variables.modeFlyActif)
    {
        UpdateCamera(&cameraEditeur, Les_variables.modeCameraActif);
    }
    else
    {
        if (ImGui::RadioButton("Première Personne (FPS)", Les_variables.modeCameraActif == CAMERA_FIRST_PERSON))
        {
            Les_variables.modeCameraActif = CAMERA_FIRST_PERSON;
        }
        if (ImGui::RadioButton("Caméra Libre (Free)", Les_variables.modeCameraActif == CAMERA_FREE))
        {
            Les_variables.modeCameraActif = CAMERA_FREE;
        }
        if (ImGui::RadioButton("Caméra Orbitale (Orbital)", Les_variables.modeCameraActif == CAMERA_ORBITAL))
        {
            Les_variables.modeCameraActif = CAMERA_ORBITAL;
        }
        if (ImGui::RadioButton("Troisième Personne (TPS)", Les_variables.modeCameraActif == CAMERA_THIRD_PERSON))
        {
            Les_variables.modeCameraActif = CAMERA_THIRD_PERSON;
        }
    }

    ImGui::Separator();
    ImGui::Text("Maintiens le Clic Droit pour bouger");
    ImGui::End();

    // pour changer le type de la caméra
    ImGui::Begin("Type de caméra");
    if (ImGui::Button("Perspective", {30.0f, 30.0f}))
    {
        // si on clique sur ce bouton ça change le mode
        if (!Les_variables.perspect)
        {
            Les_variables.perspect = true;
            Les_variables.orto = false;
            Les_variables.type_projection_camera = CAMERA_PERSPECTIVE;
            // on met a jour la perspective
            cameraEditeur.projection = Les_variables.type_projection_camera;
        }
    }
    if (ImGui::Button("Ortogonal", {30.0f, 30.0f}))
    { // TODO issue #6
        // idem ici
        if (!Les_variables.orto)
        {
            Les_variables.orto = true;
            Les_variables.perspect = false;
            Les_variables.type_projection_camera = CAMERA_ORTHOGRAPHIC;
            // on met a jour la perspective
            cameraEditeur.projection = Les_variables.type_projection_camera;
        }
    }

    ImGui::End();

    //la barre d'en haut
    if(ImGui::BeginMainMenuBar()){

        if(ImGui::BeginMenu("Fichier")){
            //si c'est true c'est qu'on a cliqué dessus
            if (ImGui::MenuItem("Ouvrir...")) {
                const char* filtres[1] = { "*.json" };
                const char* cheminChoisi = tinyfd_openFileDialog(
                    "Ouvrir un projet", //titre
                    "",                 //chemin par défaut
                    1, filtres,        //filtres d'extension (*.json)
                    "Fichiers JSON",   //fescription
                    0                  //sélection multiple désactivée
                );
            
                if (cheminChoisi != NULL) {
                    La_scene.ChargerProjet(cheminChoisi);
                }
            }
            if (ImGui::MenuItem("Enregistrer")) {
                const char* filtres[1] = { "*.json" };
                const char* cheminChoisi = tinyfd_saveFileDialog(
                    "Choisir un nom de projet", //titre
                    "projet.json",                 //chemin par défaut
                    1, filtres,        //filtres d'extension (*.json)
                    "Fichiers JSON"   //fescription
                );
            
                if (cheminChoisi != NULL) {
                    La_scene.SauvegarderProjet(cheminChoisi);
                }
            }
            if (ImGui::MenuItem("Exporter")) {
                const char* filtres[1] = { "*.cpp" };
                const char* cheminChoisi = tinyfd_saveFileDialog(
                    "Choisir un nom de fichier", //titre
                    "exporte.cpp",                 //chemin par défaut
                    1, filtres,        //filtres d'extension (*.json)
                    "Fichiers CPP"
                );
            
                if (cheminChoisi != NULL) {
                    std::string contenu = GenererCodeComplet(La_scene.GetNodes());
                    Sauvgarde(cheminChoisi, contenu);
                }
            }
            //if(ImGui::MenuItem("Exporter","ctrl+e")){    
            //    std::string cheminFichier; //fonction pour selectioner un fichier ou un emplacement depuis l'editeur externe
            //    ChargerProjet(cheminFichier);
            //}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Paramètres")) {
            bool Changement = false;//c'est remis à false à chaque fois
    
            if (ImGui::SliderInt("Seuil de Sauvegarde", &Les_parametres.limiteSauvgarde, 10, 200)) {
                Changement = true;
            }
        
            if (ImGui::InputInt("Largeur", &Les_parametres.screenWidth)) {
                Changement = true;
            }
        
            // Si l'utilisateur tape un nouveau chiffre pour la hauteur
            if (ImGui::InputInt("Hauteur", &Les_parametres.screenHeight)) {
                Changement = true;
            }
            if(ImGui::Button("Plein ecran",{50.0f,50.0f})){
                //on met le mode plein ecrant ou on le desactive
                if (!IsWindowState(FLAG_WINDOW_MAXIMIZED)){
                    Les_parametres.Plein_ecran = 1;
                    //int taille_ecrant = GetCurrentMonitor();
                    //SetWindowSize(GetMonitorWidth(taille_ecrant),GetMonitorHeight(taille_ecrant));
                    //ToggleFullscreen
                    MaximizeWindow();
                    Changement = true;
                }else{
                    Les_parametres.Plein_ecran = 0;
                    SetWindowSize(Les_parametres.screenWidth,Les_parametres.screenHeight);
                    RestoreWindow();
                    //ToggleFullscreen();
                    Changement = true;
                }
                //std::cout << "largeur : " << GetMonitorWidth(GetCurrentMonitor()) << " hauteur : " << GetMonitorHeight(GetCurrentMonitor()) << std::endl;
            }

            if(Changement){
                //enregistre que quand la valeur change
                SauvegarderConfig(Les_parametres);
                //pour appliquer les changements
                //ChargerConfig(Les_parametres);
                SetWindowSize(Les_parametres.screenWidth, Les_parametres.screenHeight);
            }
            ImGui::EndMenu();

        }
        //exemple d'ajout de bouton
        if(ImGui::Button("cube",{50.0f,50.0f})){
            La_scene.AjouterCube();
        }
        
        ImGui::EndMainMenuBar();
    }
        
    rlImGuiEnd();
}