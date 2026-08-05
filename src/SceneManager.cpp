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
    static u_int compteur = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
    // on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<CubeNode>());
    std::string nom_ancien = sceneNodes.back()->nom; //pour eviter les appels recurent chiant
    std::string identifiant = "_" + std::to_string(compteur);//un numéro de compteur
    sceneNodes.back()->nom = nom_ancien + identifiant; //nouveau nom c'est une concaténation des 2 chaines
    sceneNodes.back()->isSelected = true;
    compteur++;
}

//idem pour une caméra3D
void SceneManager::AjouterCamera3D(){
    static u_int compteur = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
    sceneNodes.push_back(std::make_unique<CameraNode>());
    std::string nom_ancien = sceneNodes.back()->nom; //pour eviter les appels recurent chiant
    std::string identifiant = "_" + std::to_string(compteur);//un numéro de compteur
    sceneNodes.back()->nom = nom_ancien + identifiant; //nouveau nom c'est une concaténation des 2 chaines
    sceneNodes.back()->isSelected = true;
    compteur++;
}

//idem pour une caméra2D
void SceneManager::AjouterCamera2D(){
    static u_int compteur = 0;//TODO checker si c'est une bonne idée de l'initialiser à 0 à chaque fois
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
            if (noeudSelectionne == it->get()) {
                noeudSelectionne = nullptr;
            }
            it = sceneNodes.erase(it); //delete et récupère le nouvel itérateur valide
        } else {
            ++it;
        }
    }
}

SceneNode* SceneManager::GetSelection(){
    return noeudSelectionne;
}

void SceneManager::Deselectionne(){
    for(auto & noeud : sceneNodes){
        if(noeud->isSelected){
            //on déséléctionne
            noeud->isSelected = false;
            return;
        }
    }
}

void SceneManager::SetSelection(SceneNode* noeud){
    //faut d'abord que je deselectionne
    Deselectionne();
    //ensuite on selectione le nouveau
    if (noeud != nullptr) {//check de securite
        noeud->isSelected = true;
        noeudSelectionne = noeud;
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
            if(le_type == "cube"){
                auto nouveau_noeud = std::make_unique<CubeNode>();
                //faut remplir le nouveau noeud avec les info lue
                nouveau_noeud->type = element_json["type"];
                
                nouveau_noeud->nom = element_json["nom"];

                nouveau_noeud->isSelected = element_json["isSelected"];

                //pour la couleur faut voir comment je fait passer ça on va dire un tableau de 4 float
                nouveau_noeud->couleur.a = element_json["couleur"][0];
                nouveau_noeud->couleur.r = element_json["couleur"][1];
                nouveau_noeud->couleur.g = element_json["couleur"][2];
                nouveau_noeud->couleur.b = element_json["couleur"][3];

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
                sceneNodes.push_back(nouveau_noeud);
            }
            else if(le_type == "sphere"){
                auto nouveau_noeud = std::make_unique<SphereNode>();
                //faut remplir le nouveau noeud avec les info lue
                nouveau_noeud->type = element_json["type"];
                
                nouveau_noeud->nom = element_json["nom"];

                nouveau_noeud->isSelected = element_json["isSelected"];

                nouveau_noeud->couleur.a = element_json["couleur"][0];
                nouveau_noeud->couleur.r = element_json["couleur"][1];
                nouveau_noeud->couleur.g = element_json["couleur"][2];
                nouveau_noeud->couleur.b = element_json["couleur"][3];

                nouveau_noeud->position.x = element_json["position"][0];
                nouveau_noeud->position.y = element_json["position"][1];
                nouveau_noeud->position.z = element_json["position"][2];

                nouveau_noeud->rotation.x = element_json["rotation"][0];
                nouveau_noeud->rotation.y = element_json["rotation"][1];
                nouveau_noeud->rotation.z = element_json["rotation"][2];

                nouveau_noeud->taille.x = element_json["taille"][0];
                nouveau_noeud->taille.y = element_json["taille"][1];
                nouveau_noeud->taille.z = element_json["taille"][2];
                
                sceneNodes.push_back(nouveau_noeud);

            }
            else if(le_type == "camera3D"){
                auto nouveau_noeud = std::make_unique<CameraNode>();
                //faut remplir le nouveau noeud avec les info lue
                nouveau_noeud->type = element_json["type"];
                
                nouveau_noeud->nom = element_json["nom"];

                nouveau_noeud->isSelected = element_json["isSelected"];

                //pour la couleur faut voir comment je fait passer ça on va dire un tableau de 4 float
                nouveau_noeud->couleur.a = element_json["couleur"][0];
                nouveau_noeud->couleur.r = element_json["couleur"][1];
                nouveau_noeud->couleur.g = element_json["couleur"][2];
                nouveau_noeud->couleur.b = element_json["couleur"][3];

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
                
                //vu que c'est une camera faut aussi stoquer les info de la camera
                nouveau_noeud->target.x = element_json["target"][0];
                nouveau_noeud->target.y = element_json["target"][1];
                nouveau_noeud->target.z = element_json["target"][2];

                nouveau_noeud->fovy = element_json["fovy"];
                nouveau_noeud->mode_camera = element_json["mode_camera"];
                nouveau_noeud->projetction_cam = element_json["projetction_cam"];
                sceneNodes.push_back(nouveau_noeud);
            }
            else if(le_type == "camera2D"){
                auto nouveau_noeud = std::make_unique<Camera2DNode>();
                //faut remplir le nouveau noeud avec les info lue
                nouveau_noeud->type = element_json["type"];
                
                nouveau_noeud->nom = element_json["nom"];

                nouveau_noeud->isSelected = element_json["isSelected"];

                //pour la couleur faut voir comment je fait passer ça on va dire un tableau de 4 float
                nouveau_noeud->couleur.a = element_json["couleur"][0];
                nouveau_noeud->couleur.r = element_json["couleur"][1];
                nouveau_noeud->couleur.g = element_json["couleur"][2];
                nouveau_noeud->couleur.b = element_json["couleur"][3];

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
                
                //vu que c'est une camera faut aussi stoquer les info de la camera
                nouveau_noeud->zoom_camera = element_json["zoom_camera"];
                
                sceneNodes.push_back(nouveau_noeud);
            }
            else{
                //par défaut si c'est rien de tout ça je skip ce noeud
                continue;
            }
        }
    }
    file.close();
}