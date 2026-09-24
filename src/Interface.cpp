#include "Interface.h"

void Dessiner_MenuPrincipale(SceneManager& La_scene, EditorContext& Les_variables, Parametres& Les_parametres){
    //recuperation des variables etc
    std::vector<SceneNode*> noeuds_selectione = La_scene.GetSelection();
    //pour le menu de modification quand on selectionne un objets
    if(!noeuds_selectione.empty()){
        if(ImGui::BeginPopupContextVoid("MenuActions", ImGuiPopupFlags_MouseButtonRight)){

            if(ImGui::MenuItem("Supprimer")){
                for (auto & element : noeuds_selectione){
                    La_scene.SupprimerNoeud(element);
                }
                La_scene.Deselectionne();
            }
            if(ImGui::MenuItem("Dupliquer")){
                for (auto & element : noeuds_selectione){
                    //faut dupliquer tout les elements selectionnées
                    La_scene.AjouterNoeud(element->Cloner());
                }
            }
            
            ImGui::EndPopup();
        }
    }
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
            Les_variables.flag_changements = true; //pour mettre à jour le code
        }
        
        ImGui::EndMainMenuBar();
    }
}

void Dessiner_Hierarchie(SceneManager& La_scene, EditorContext& Les_variables){
    ImGui::Begin("Hierarchie");
        bool est_selectione = false;
        std::vector<SceneNode*> selection = La_scene.GetSelection();//variable temporaire
        for (size_t i = 0; i < La_scene.GetNodes().size(); i++){
            // un label unique pour chaque objet
            std::string label = La_scene.GetNodes()[i]->nom + "##" + std::to_string(i);
            auto it = std::find(selection.begin(), selection.end(), La_scene.GetNodes()[i].get());

            est_selectione = (it != selection.end());
            // faut mettre a jour le pointeur selectioneur
            if (ImGui::Selectable(label.c_str(), est_selectione)){
                if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
                    La_scene.SceneManager::ToggleSelection(La_scene.GetNodes()[i].get());
                }else{
                    // faut désélectionner l'ancien et selectionner le nouveau mais c'est fait par la fonction setseleciton
                    La_scene.SceneManager::SetSelection(La_scene.GetNodes()[i].get());
                    Les_variables.flag_changements = true;
                }
            }
        }
    ImGui::End();
}

void Dessiner_Inspecteur(SceneManager& La_scene, EditorContext& Les_variables){
    //recuperation des variables etc
    std::vector<SceneNode*> noeuds_selectione = La_scene.GetSelection();
    ImGui::Begin("Inspecteur");
    if (!noeuds_selectione.empty()){
        //affiche le ou les noms des objets tout en haut
        //faut voir on fait quoi si on a plusieurs objets on affiche la propriété d'un seul ?
        if(noeuds_selectione.size() > 1){
            //si on a plus d'un element alors faut faire le cas N elements
            ImGui::Text("%ld éléments Selectionné", noeuds_selectione.size());
            ImGui::Separator();    
        }else{
            if(noeuds_selectione[0] != nullptr){

                ImGui::Text("Modification de : %s", noeuds_selectione[0]->nom.c_str());
                ImGui::Separator();
                
                // sliders pour modifier dynamiquement les variables
            if (
                // TODO ajouter les bouton pour ajouter des objetsg ici aussi
                ImGui::DragFloat3("Position", &noeuds_selectione[0]->position.x, 0.1f) || ImGui::DragFloat3("Rotation", &noeuds_selectione[0]->rotation.x, 1.0f) || ImGui::DragFloat3("Taille", &noeuds_selectione[0]->taille.x, 0.1f))
                {
                    Les_variables.flag_changements = true;
                }
            ImGui::Separator();
            ImGui::Text("Materiau");

            SceneNode* objet = noeuds_selectione[0];
            if(ImGui::BeginCombo("Shader", objet->nom_shader_actuel.c_str())){
                for(auto const& [nom, shader_obj] : La_scene.shaderManager.dictionnaire_shaders){
                    bool est_selectione = (objet->nom_shader_actuel == nom);
                    if(ImGui::Selectable(nom.c_str(),est_selectione)){
                        //si on clique ça applique
                        objet->AppliquerShader(nom,shader_obj);
                        Les_variables.flag_changements = true;//pour l'autosave
                    }
                    if(est_selectione){
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            }else{
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "ERREUR FATALE : Pointeur NULL !");
            }
        }

        // Pour la couleur, c'est un peu plus complexe car ImGui utilise des floats (0.0 à 1.0)
        // et Raylib des unsigned char (0 à 255), on fera ça plus tard si tu veux.
    }
    else
    {
        ImGui::Text("Aucun objet selectionné");
    }
    ImGui::End();
}

void Dessiner_ControlesCamera(Camera3D& cameraEditeur, EditorContext& Les_variables){
    //la partie caméra
    // la progress bar stylée qui apparait si on commence à maintenir le clic faut la changer ?
    if (!Les_variables.modeFlyActif && Les_variables.tempsMaintien > 0.0f){
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
    if (Les_variables.modeFlyActif){
        UpdateCamera(&cameraEditeur, Les_variables.modeCameraActif);
    }
    else{
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
    //ImGui::Text("Maintiens le Clic Droit pour bouger"); plus besoin
    ImGui::End();

    // pour changer le type de la caméra
    ImGui::Begin("Type de caméra");
    if (ImGui::Button("Perspective", {30.0f, 30.0f})){
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
    if (ImGui::Button("Ortogonal", {30.0f, 30.0f})){ // TODO issue #6
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
}

void Dessiner_ApercuCode(SceneManager& La_scene, EditorContext& Les_variables, Parametres& Les_parametres){
    ImGui::Begin("Appercu code");
    //le code genere
    if(Les_variables.flag_changements){
        //si on a eu un changement on augmente le compteur
        Les_variables.compteurModifs++;
        std::cout << "Modifications : " << Les_variables.compteurModifs << "/" << Les_parametres.limiteSauvgarde << std::endl;    //faut regenerer le code
        Les_variables.code_preview = GenererCodeComplet(La_scene.SceneManager::GetNodes());//on donne à manger tous les noeuds de la scene
        Les_variables.flag_changements = false; //faut penser à le rebaisser le flag hein
    }
    if(Les_variables.compteurModifs >= Les_parametres.limiteSauvgarde){
        //la je peut lancer la sauvgarde
        La_scene.SauvegarderProjet("autosave.json");
        Les_variables.compteurModifs = 0;
    }
    std::string code_buf = Les_variables.code_preview;
    if(code_buf.empty()) code_buf = "";//le buffer pour l'api
    //TODO faut agrandire ça pour avoir le preview plus grands mais en ratio de la fenetre
    ImGui::InputTextMultiline(" ", &code_buf[0], code_buf.size()+1, ImVec2(GetScreenWidth()/5.0f,GetScreenHeight()/5.0f), ImGuiInputTextFlags_ReadOnly);
    ImGui::End();
}

void Dessiner_EditeurShader(SceneManager& La_scene, EditorContext& Les_variables){
    ImGui::Begin("Shader");
    
    if (ImGui::Button("ouvrir dossier shader",ImVec2(-1,20))) {
        const char* filtres[2] = { "*.fs","*.vs" };//voir pour envoyer 2 fichier en meme temps ou 1 si besoin
        const char* cheminChoisi = tinyfd_openFileDialog(
            "ouvrir un shader", //titre
            "",                 //chemin par défaut
            2, filtres,        //filtres d'extension
            "ShadeFichiers Shader (.fs, .vs)",   //fescription
            1                  //sélection multiple activéé
        );
        if (cheminChoisi != NULL) {
            std::string chemins(cheminChoisi);
            size_t position_separateur = chemins.find('|');//le separateur c'est | en cas de plusieurs fichiers
            if (position_separateur != std::string::npos) {
            //si un '|', on a plusieurs fichiers
            std::string fichier1 = chemins.substr(0, position_separateur);
            std::string fichier2 = chemins.substr(position_separateur + 1);
                    
            //tinyfd garantit pas l'ordre des fichiers fonc faut check lequel est le .vs et lequel est le .fs
            std::string cheminVS = (fichier1.find(".vs") != std::string::npos) ? fichier1 : fichier2;//un ternaire pour voir si il est dedans
            std::string cheminFS = (fichier1.find(".fs") != std::string::npos) ? fichier1 : fichier2;
                    
            La_scene.shaderManager.ChargerShaders(cheminVS, cheminFS);
            std::cout << "Shaders charges : " << cheminVS << " et " << cheminFS << std::endl;//debut pour moi
        } 
        else {
            //si pas de '|', l'utilisateur a choisi qu'un seul fichier donc l'ature c'est un shader pas dafaut
            if (chemins.find(".fs") != std::string::npos) {
                std::cout << "Fragment Shader seul charge : " << chemins << std::endl;
                //le Vertex par défaut de Raylib
                La_scene.shaderManager.ChargerShaders("", chemins); 
            } 
            else if (chemins.find(".vs") != std::string::npos) {
                std::cout << "Vertex Shader seul charge : " << chemins << std::endl;
                La_scene.shaderManager.ChargerShaders(chemins, "");
            }
        }
    }
    }
    ImGui::Text("Fragment shader :");
    
    ImGui::InputTextMultiline("###code_fs",&Les_variables.codeFragmentShader[0],4096,ImVec2(ImGui::GetContentRegionAvail().x,200)); //TODO voir si on peut pas augmenter la limite
    
    //rendu du champ de text
    ImGui::InputText("Nom du shader", &Les_variables.nom_nouveau_shader[0], 64);
    //si on clique sur le bouton :
    if(ImGui::Button("Compiler et Appliquer", ImVec2(-1,30))){
        const char * defaultVS = "";//c'est le shader vs de base de raylib
        La_scene.shaderManager.ChargerShaderDepuisTexte( Les_variables.nom_nouveau_shader, "", Les_variables.codeFragmentShader);
    }
    
    ImGui::End();
}

void gere_interface(SceneManager& La_scene, Camera3D& cameraEditeur, EditorContext& Les_variables, Parametres& Les_parametres){
    
    rlImGuiBegin();
    //raycasting
    La_scene.Gerer_pointeur(cameraEditeur, Les_variables);
    
    Dessiner_MenuPrincipale(La_scene, Les_variables, Les_parametres);
    Dessiner_Hierarchie(La_scene, Les_variables);
    Dessiner_Inspecteur(La_scene, Les_variables);
    Dessiner_ControlesCamera(cameraEditeur, Les_variables);
    Dessiner_ApercuCode(La_scene, Les_variables, Les_parametres);
    Dessiner_EditeurShader(La_scene,Les_variables);
    rlImGuiEnd();
}