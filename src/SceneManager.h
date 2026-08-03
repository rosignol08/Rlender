#include "SceneNode.h"

/*
classe pour gerer la scene qui va permettre d'ajouter ou suprimer des noeuds

*/
class SceneManager{
    private:
        //pour stoquer les noeuds
        std::vector<std::unique_ptr<SceneNode>> sceneNodes;
        //le pointeur pour choisir un noeuds specifique
        SceneNode* noeudSelectionne = nullptr;
    public:
        void DrawScene(); //une boucle qui parcours les noeuds et les dessines chacuns
        void AjouterCube(); //ça ajoute un cube simplement
        void AjouterCamera3D(); //idem pour une caméra3D
        void AjouterCamera2D(); //idem pour une caméra2D
        void SupprimerSelection(); //pour enlever un noeud de la liste TODO faut faire attention à la mémoire
        //des getters pour que l'interface graphique accede aux bon noeuds etc
}