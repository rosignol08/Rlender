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