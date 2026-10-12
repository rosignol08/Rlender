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
        }
    }
    return;
}

void SceneManager::Update(Camera3D& camera_editeur, EditorContext& variables){
    variables.compteur_lumieres = 0;
    variables.variables_globales["temps"] = (float)GetTime(); //normalement c'est un double mais osef
    variables.variables_globales["viewPos"] = camera_editeur.position;//le point de vue de la vraie camera
    //les obj
    
    for(auto& noeud : sceneNodes) {
        //std::cout << "maj neoud " << noeud->GetType() << std::endl;
        noeud->Update(variables);
    }
    variables.variables_globales["lightCount"] = variables.compteur_lumieres;
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
}

void SceneManager::AjouterSphere(){
    unsigned int compteur_sphere = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Sphere_" + std::to_string(compteur_sphere);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_sphere++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<SphereNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
}

void SceneManager::AjouterPlan(){
    unsigned int compteur_plan = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Plan_" + std::to_string(compteur_plan);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_plan++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<PlaneNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
}

void SceneManager::AjouterCyl(){
    unsigned int compteur_cyl = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Cylindre_" + std::to_string(compteur_cyl);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_cyl++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<CylinderNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
}


void SceneManager::AjouterCone(){
    unsigned int compteur_Cone = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Cone_" + std::to_string(compteur_Cone);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_Cone++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<ConeNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
}

//ça ajoute une light simplement
void SceneManager::AjouterLight(){
    unsigned int compteur_light = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Light_" + std::to_string(compteur_light);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_light++;
                break;
            }
        }
    }
    //on fait comme ça :
    sceneNodes.push_back(std::make_unique<LightNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
}

void SceneManager::AjouterSoleil(){
    unsigned int compteur_Soleil = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Soleil_" + std::to_string(compteur_Soleil);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_Soleil++;
                break;
            }
        }
    }
    //on fait comme ça :
    sceneNodes.push_back(std::make_unique<SoleilNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
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

void SceneManager::AjouterModele(){
    unsigned int compteur_modele = 0;
    bool nom_deja_pris = true;
    std::string nom_test;
    while(nom_deja_pris == true){
        nom_deja_pris = false;//je dit que de base on a un bon nom
        nom_test = "Modele_" + std::to_string(compteur_modele);
        for(const auto & elem : sceneNodes){
            if(elem->nom == nom_test){
                nom_deja_pris = true;
                compteur_modele++;
                break;
            }
        }
    }
    //on fait comme ça : sceneNodes.push_back(std::make_unique<CubeNode>());
    sceneNodes.push_back(std::make_unique<ModeleNode>());
    sceneNodes.back()->nom = nom_test; //nouveau nom
    SetSelection(sceneNodes.back().get());
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


void SceneManager::Gerer_pointeur(Camera3D camera_editeur){
    //raycasting
    if (ImGuizmo::IsOver()) return;
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
        if(objetTouche != nullptr){
            if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
                ToggleSelection(objetTouche);
            }else{
                SetSelection(objetTouche);
            }
        } else {
            // SI ON CLIQUE DANS LE VIDE
            if (!IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL)){
                Deselectionne();
            }
        }
    }
}

void SceneManager::ViderScene() {
    Deselectionne(); //clear la sélection avant
    sceneNodes.clear();
}