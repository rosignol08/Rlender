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

}
//idem pour une caméra3D
void SceneManager::AjouterCamera3D(){

}

//idem pour une caméra2D
void SceneManager::AjouterCamera2D(){

}

//pour enlever un noeud de la liste TODO faut faire attention à la mémoire
void SceneManager::SupprimerSelection(){

}
