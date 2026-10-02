#include "SceneManager.h"

//une boucle qui parcours les noeuds et les dessines chacuns
void SceneManager::DrawScene(EditorContext& variables){
    //pour parcourir tous les noeuds a dessiner faut boucler sur la liste par reference pas copie
    for(auto& noeud : sceneNodes){
        noeud->Draw(variables);//->parce que faut aller acceder à la fonction de l'objet pointée par noeud
    }
    std::vector<SceneNode*> selection = GetSelection();
    if(!selection.empty()){
        for(const auto & elements : selection){//const parce qu'on le change pas
            if (elements == nullptr) continue;
            DrawBoundingBox(elements->GetBoiteCollision(), GREEN);
            //on dessine les gizmo voir si on affiche le gizmo ici
                Vector3 position_base = elements[0].position; //pour eviter les acces mémoire répétés
                DrawCylinderEx(position_base,(Vector3){position_base.x+4,position_base.y,position_base.z},0.20f,0.20f,10,RED);
                DrawCylinderEx(position_base,(Vector3){position_base.x,position_base.y+4,position_base.z},0.20f,0.20f,10,GREEN);
                DrawCylinderEx(position_base,(Vector3){position_base.x,position_base.y,position_base.z+4},0.20f,0.20f,10,BLUE);
        }
    }
    return;
}

void SceneManager::Update(Camera3D& camera_editeur, EditorContext& variables){
    variables.variables_globales["temps"] = (float)GetTime(); //normalement c'est un double mais osef
    variables.variables_globales["viewPos"] = camera_editeur.position;//le point de vue de la vraie camera
    //les obj
    
    for(auto& noeud : sceneNodes) {
        noeud->Update(variables);
    }
}

//ça ajoute un cube simplement
void SceneManager::AjouterCube(){
    //static unsigned int compteur_cube = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
    unsigned int compteur_cube = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Cube_" + std::to_string(compteur_cube);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_cube++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());

    for(auto & element : sceneNodes){
        if(element->isSelected){
            element->isSelected = false; //deselectionne
        }
    }
    sceneNodes.back()->isSelected = true;
}

//ça ajoute un cube simplement
void SceneManager::AjouterLight(){
    //static unsigned int compteur_cube = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
    unsigned int compteur_light = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Cube_" + std::to_string(compteur_light);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_light++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<LightNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());

    for(auto & element : sceneNodes){
        if(element->isSelected){
            element->isSelected = false; //deselectionne
        }
    }
    sceneNodes.back()->isSelected = true;
}

//idem pour une caméra3D
void SceneManager::AjouterCamera3D(){
    static unsigned int compteur = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
    sceneNodes.push_back(std::make_unique<CameraNode>());
    std::string nom_ancien = sceneNodes.back()->nom; //pour eviter les appels recurent chiant
    std::string identifiant = "_" + std::to_string(compteur);//un numéro de compteur
    sceneNodes.back()->nom = nom_ancien + identifiant; //nouveau nom c'est une concaténation des 2 chaines
    sceneNodes.back()->isSelected = true;
    compteur++;
}

//idem pour une caméra2D
void SceneManager::AjouterCamera2D(){
    static unsigned int compteur = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
    sceneNodes.push_back(std::make_unique<Camera2DNode>());
    std::string nom_ancien = sceneNodes.back()->nom; //pour eviter les appels recurent chiant
    std::string identifiant = "_" + std::to_string(compteur);//un numéro de compteur
    sceneNodes.back()->nom = nom_ancien + identifiant; //nouveau nom c'est une concaténation des 2 chaines
    sceneNodes.back()->isSelected = true;
    compteur++;
}

//pour enlever un noeud de la liste TODO faut faire attention à la mémoire
void SceneManager::SupprimerSelection(){
    //on doit utiliser le pointeur
    for (auto it = sceneNodes.begin(); it != sceneNodes.end(); ) {
        if ((*it)->isSelected) {
            if (noeudSelectionne[0] == it->get()) {
                noeudSelectionne[0] = nullptr;
            }
            it = sceneNodes.erase(it); //delete et récupère le nouvel itérateur valide
        } else {
            ++it;
        }
    }
}

std::vector<SceneNode*> SceneManager::GetSelection(){
    return noeudSelectionne;
}

void SceneManager::Deselectionne(){
    //ça vide touts les noeuds selectioné
    for(auto & noeud : sceneNodes){
        if(noeud->isSelected){
            //on déséléctionne
            noeud->isSelected = false;
        }
    }
    noeudSelectionne.clear();
}

void SceneManager::SetSelection(SceneNode* noeud){
    //faut d'abord que je deselectionne
    Deselectionne();
    //ensuite on selectione le nouveau
    if (noeud != nullptr) {//check de securite
        noeud->isSelected = true;
        noeudSelectionne.push_back(noeud);
    }
}

void SceneManager::ToggleSelection(SceneNode* noeud){
    if(noeud == nullptr){
        return;
    }
    //cherche dans le vecteur si le noeud y es
    auto it = std::find(noeudSelectionne.begin(), noeudSelectionne.end(), noeud);

    //si trouvé
    if (it != noeudSelectionne.end()){
        noeud->isSelected = false; //eteint
        noeudSelectionne.erase(it); //delete çe noeud
    }else{
        //sinon on l'as pas trouve donc il etait pas selectione
        noeud->isSelected = true; //allume
        noeudSelectionne.push_back(noeud); //ajoute à la liste
    }
}

const std::vector<std::unique_ptr<SceneNode>>& SceneManager::GetNodes() const{
    return sceneNodes;
}


void SceneManager::AjouterNoeud(std::unique_ptr<SceneNode> nouveau_noeud) {
    sceneNodes.push_back(std::move(nouveau_noeud));
}

void SceneManager::SupprimerNoeud(SceneNode* cible){
    auto efface = std::remove_if(sceneNodes.begin(), sceneNodes.end(), [cible](const std::unique_ptr<SceneNode>& noeud){ return noeud.get() == cible; });
    sceneNodes.erase(efface, sceneNodes.end());
}


void SceneManager::SauvegarderProjet(std::string cheminFichier) {
    //check si on a l'extention json ou pas dans le nom
    if (cheminFichier.length() < 5 || cheminFichier.substr(cheminFichier.length() - 5) != ".json") {
        cheminFichier += ".json";
    }
    nlohmann::json projet_json;
    
    // On crée un tableau JSON pour stocker notre liste de noeuds
    projet_json["noeuds"] = nlohmann::json::array();

    for (auto &noeud : sceneNodes) {
        SceneNode* actuel = noeud.get();

        //objet JSON temporaire pour ce noeud là
        nlohmann::json noeud_json;
        noeud_json["type"] = actuel->type;
        noeud_json["nom"] = actuel->nom;
        noeud_json["isSelected"] = actuel->isSelected;

        //les Vector3 de Raylib en tableaux JSON = [x, y, z]
        noeud_json["position"] = { actuel->position.x, actuel->position.y, actuel->position.z };
        noeud_json["rotation"] = { actuel->rotation.x, actuel->rotation.y, actuel->rotation.z };
        noeud_json["taille"]   = { actuel->taille.x, actuel->taille.y, actuel->taille.z };

        //idem pour la couleur
        noeud_json["couleur"]  = { actuel->couleur.r, actuel->couleur.g, actuel->couleur.b, actuel->couleur.a };
        if(actuel->type == "camera3D"){
            CameraNode* cam = dynamic_cast<CameraNode*>(actuel);//faut dire que le noeud peut etre une camera
            if (cam != nullptr) {
                //si c'est une camera faut enregistrer les autre valeurs spécifiques
                //vu que c'est une camera faut aussi stoquer les info de la camera
                noeud_json["target"] = { cam->target.x, cam->target.y, cam->target.z };
                noeud_json["fovy"] = cam->fovy;
                noeud_json["mode_camera"] = cam->mode_camera;
                noeud_json["projetction_cam"] = cam->projetction_cam;
            }
        }
        if(actuel->type == "camera2D"){
            Camera2DNode* cam = dynamic_cast<Camera2DNode*>(actuel);//faut dire que le noeud peut etre une camera
            if (cam != nullptr) {
                noeud_json["zoom_camera"] = cam->zoom_camera;
            }
        }
        //ajout du noeud à la sauvgarde du projet
        projet_json["noeuds"].push_back(noeud_json);
    }

    std::ofstream file(cheminFichier);
    if (!file) {
        std::cerr << "Erreur : problème à l'ouverture du fichier " << cheminFichier << std::endl;
        return;
    }

    //std::setw(4) pour l'indentations. 
    file << std::setw(4) << projet_json << std::endl;
    file.close();
    
    std::cout << "Projet sauvegarde avec succes dans : " << cheminFichier << std::endl;
}

//vide la scene actuelle et remplis avec le json lu
void SceneManager::ChargerProjet(std::string cheminFichier){
    std::ifstream file(cheminFichier);
    if (!file) {
        std::cerr << "Erreur : problème à l'ouverture du fichier " << cheminFichier << std::endl;
        return;
    }

    nlohmann::json projet_json;
    try {
        file >> projet_json; //on parse le fichier en mémoire avant d'écraser la scène
    } catch (const nlohmann::json::parse_error& erreur) {
        std::cerr << "Erreur : fichier JSON invalide pour " << cheminFichier
                  << " (" << erreur.what() << ")" << std::endl;
        file.close();
        return;
    }
    file.close();

    if (!projet_json.is_object() || !projet_json.contains("noeuds") || !projet_json["noeuds"].is_array()) {
        std::cerr << "Erreur : structure JSON invalide pour " << cheminFichier << std::endl;
        return;
    }

    std::vector<std::unique_ptr<SceneNode>> nouvelle_scene;
    nouvelle_scene.reserve(static_cast<size_t>(projet_json["noeuds"].size()));

    for (auto& element_json : projet_json["noeuds"]) {
        if (!element_json.is_object()) {
            continue;
        }

        if (!element_json.contains("type") || !element_json["type"].is_string()) {
            continue;
        }

        std::string le_type = element_json["type"].get<std::string>();

        std::unique_ptr<SceneNode> nouveau_noeud = nullptr;
        if(le_type == "cube"){
            nouveau_noeud = std::make_unique<CubeNode>();
        }
        else if(le_type == "sphere"){
            nouveau_noeud = std::make_unique<SphereNode>();
        }
        else if(le_type == "camera3D"){
            if (!element_json.contains("target") || !element_json["target"].is_array() ||
                !element_json.contains("fovy") || !element_json.contains("mode_camera") ||
                !element_json.contains("projetction_cam")) {
                continue;
            }

            auto cam = std::make_unique<CameraNode>();//vu que c'est une camera faut aussi stoquer les info de la camera
            cam->target.x = element_json["target"][0];
            cam->target.y = element_json["target"][1];
            cam->target.z = element_json["target"][2];

            cam->fovy = element_json["fovy"];
            cam->mode_camera = element_json["mode_camera"];
            cam->projetction_cam = element_json["projetction_cam"];
            nouveau_noeud = std::move(cam);//faut le mettre dans le pointeur generique
        }
        else if(le_type == "camera2D"){//pareil c'est une camera donc on stoque
            if (!element_json.contains("zoom_camera")) {
                continue;
            }

            auto cam = std::make_unique<Camera2DNode>();
            cam->zoom_camera = element_json["zoom_camera"];
            nouveau_noeud = std::move(cam);
        }
        else{//par défaut si c'est rien on skip le noeud
            continue;
        }

        if (nouveau_noeud == nullptr) {//faut remplir le noeud avec les info lue de base DRY
            continue;
        }

        if (!element_json.contains("nom") || !element_json.contains("isSelected") ||
            !element_json.contains("couleur") || !element_json.contains("position") ||
            !element_json.contains("rotation") || !element_json.contains("taille")) {
            continue;
        }

        nouveau_noeud->type = le_type;
        nouveau_noeud->nom = element_json["nom"];
        nouveau_noeud->isSelected = element_json["isSelected"];
        //pour la couleur faut voir comment je fait passer ça on va dire un tableau de 4 float
        nouveau_noeud->couleur.r = element_json["couleur"][0];
        nouveau_noeud->couleur.g = element_json["couleur"][1];
        nouveau_noeud->couleur.b = element_json["couleur"][2];
        nouveau_noeud->couleur.a = element_json["couleur"][3];
        //pour la position c'est un vecteur pareil
        nouveau_noeud->position.x = element_json["position"][0];
        nouveau_noeud->position.y = element_json["position"][1];
        nouveau_noeud->position.z = element_json["position"][2];
        nouveau_noeud->rotation.x = element_json["rotation"][0];
        nouveau_noeud->rotation.y = element_json["rotation"][1];
        nouveau_noeud->rotation.z = element_json["rotation"][2];
        nouveau_noeud->taille.x = element_json["taille"][0];
        nouveau_noeud->taille.y = element_json["taille"][1];
        nouveau_noeud->taille.z = element_json["taille"][2];
        sceneNodes.push_back(std::move(nouveau_noeud));//move pour déplacer la propriété du pointeur
    }

    Deselectionne();
    sceneNodes = std::move(nouvelle_scene);
}

void SceneManager::Gerer_pointeur(Camera3D camera_editeur, EditorContext & variables){
    //raycasting
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse){
        Ray rayon = GetScreenToWorldRay(GetMousePosition(), camera_editeur);
        SceneNode* objetTouche = nullptr;
        float distanceMin = 999999.0f;
        //recherche de qui collisionne sa bounding box avec le rayon de la souris
        for(auto & element : GetNodes()){
            //localité spatiale normalement ça devrait etre optimisé par le compilo donc pas besoin de initialiser avant la boucle
            RayCollision collision = GetRayCollisionBox(rayon,element->GetBoiteCollision());
            if(collision.hit && collision.distance < distanceMin){
                //si c'est bon on met à jour
                objetTouche = element.get();
                distanceMin = collision.distance;
            }
        }

        //check si on a une selection
        if(!GetSelection().empty()){

            Vector3 position_base = GetSelection()[0]->position; //pour eviter les acces mémoire répétés
            BoundingBox box_x = { 
                (Vector3){position_base.x, position_base.y - 0.5f, position_base.z - 0.5f},
                (Vector3){position_base.x + 4.0f, position_base.y + 0.5f, position_base.z + 0.5f}
            };
            BoundingBox box_y = { 
                (Vector3){position_base.x - 0.5f, position_base.y, position_base.z - 0.5f},
                (Vector3){position_base.x + 0.5f, position_base.y + 4.0f, position_base.z + 0.5f}
            };
            BoundingBox box_z = { 
                (Vector3){position_base.x - 0.5f, position_base.y - 0.5f, position_base.z},
                (Vector3){position_base.x + 0.5f, position_base.y + 0.5f, position_base.z + 4.0f}
            };  
            RayCollision collisionx = GetRayCollisionBox(rayon,box_x);
            RayCollision collisiony = GetRayCollisionBox(rayon,box_y);
            RayCollision collisionz = GetRayCollisionBox(rayon,box_z);
                
            if (collisionx.hit) { variables.axe_en_cours = 'X'; 
                std::cout << "X touche" << std::endl;
                return;
            }
            if (collisiony.hit) { variables.axe_en_cours = 'Y';
                std::cout << "Y touche" << std::endl;
                return;
            }
            if (collisionz.hit) { variables.axe_en_cours = 'Z'; 
                std::cout << "Z touche" << std::endl;
                return;
            }   
        }
        variables.axe_en_cours = '0';

        if(objetTouche != nullptr){
            if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){    
                //ctrl avec plusieurs objets
                ToggleSelection(objetTouche);
            }else{
                SetSelection(objetTouche);
            }
        }else{
            if (!IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL)){
                Deselectionne();//si on clique dans le vide on déséléctionne
            }
        }
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && variables.axe_en_cours != '0'){
        //si on maintient le clic et qu'on touche a un axe
        if (!GetSelection().empty()) {
            Vector2 pos_souris = GetMouseDelta();
            switch (variables.axe_en_cours){
                case 'X':
                    GetSelection()[0]->position.x += pos_souris.x*0.02f; //psk en 2d la souris va vite TODO etaloner la valeurs
                    variables.flag_changements = true;
                    break;

                case 'Y':
                    GetSelection()[0]->position.y -= pos_souris.y*0.02f;//sur l'ecrant y descend et en 3D il monte c'est inversé donc -
                    variables.flag_changements = true;
                break;

                case 'Z':
                    GetSelection()[0]->position.z += pos_souris.x*0.01f + (pos_souris.y*0.01f);//les deux ? jsp au choix
                    variables.flag_changements = true;
                break;
            }
        }
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        variables.axe_en_cours = '0';
    }
}


void SceneManager::ViderScene() {
    Deselectionne(); //clear la sélection avant
    sceneNodes.clear();
}