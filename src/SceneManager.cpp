#include "SceneManager.h"

//une boucle qui parcours les noeuds et les dessines chacuns
void SceneManager::DrawScene(){
    //pour parcourir tous les noeuds a dessiner faut boucler sur la liste par reference pas copie
    for(auto& noeud : sceneNodes){
        noeud->Draw();//->parce que faut aller acceder à la fonction de l'objet pointée par noeud
    }
    return;
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

SceneNode* SceneManager::GetSelection(){
    if (noeudSelectionne.empty()) {
        return nullptr; //rien selectionne
    }
    return noeudSelectionne[0];
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

const std::vector<std::unique_ptr<SceneNode>>& SceneManager::GetNodes() const{
    return sceneNodes;
}

void SceneManager::SauvegarderProjet(std::string cheminFichier) {
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


//fini la fonction bien frero

//vide la scene actuelle et remplis avec le json lu
void SceneManager::ChargerProjet(std::string cheminFichier){
    sceneNodes.clear();//on vide les noeuds d'avant
    std::ifstream file(cheminFichier);
    if (!file) {
        std::cerr << "Erreur : problème à l'ouverture du fichier " << cheminFichier << std::endl;
        return;
    }
    nlohmann::json projet_json;
    file >> projet_json; //on met le fichier dans le json et on le parse
    //normalement file est une liste de liste donc on peut faire ça pour boucler sur tous les elements
    //std::unique_ptr<SceneNode> noeud = std::make_unique<SceneNode>();
    if(projet_json.contains("noeuds")){
        //si on a un noeud
        for (auto& element_json : projet_json["noeuds"]) {
            //on cree un obj temporaire
            std::string le_type = element_json["type"].get<std::string>();

            std::unique_ptr<SceneNode> nouveau_noeud = nullptr;
            if(le_type == "cube"){
                nouveau_noeud = std::make_unique<CubeNode>();
            }
            else if(le_type == "sphere"){
                nouveau_noeud = std::make_unique<SphereNode>();
            }
            else if(le_type == "camera3D"){
                auto cam = std::make_unique<CameraNode>();
                
                //vu que c'est une camera faut aussi stoquer les info de la camera
                cam->target.x = element_json["target"][0];
                cam->target.y = element_json["target"][1];
                cam->target.z = element_json["target"][2];

                cam->fovy = element_json["fovy"];
                cam->mode_camera = element_json["mode_camera"];
                cam->projetction_cam = element_json["projetction_cam"];
                nouveau_noeud = std::move(cam);//faut le mettre dans le pointeur generique
            }
            else if(le_type == "camera2D"){
                auto cam = std::make_unique<Camera2DNode>();
                
                //vu que c'est une camera faut aussi stoquer les info de la camera
                cam->zoom_camera = element_json["zoom_camera"];
                
                nouveau_noeud = std::move(cam);
            }
            else{
                //par défaut si c'est rien de tout ça je skip ce noeud
                continue;
            }
            if (nouveau_noeud != nullptr) {
            nouveau_noeud->type = le_type;
            //faut remplir le nouveau noeud avec les info lue de base DRY
                
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
        }
    }
    file.close();
}