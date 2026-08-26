#include "SceneNode.h"
#include "json.hpp"
#include"imgui.h"
#pragma once
/*
classe pour gerer la scene qui va permettre d'ajouter ou suprimer des noeuds

*/
class SceneManager{
    private:
        //pour stoquer les noeuds
        std::vector<std::unique_ptr<SceneNode>> sceneNodes;
        //le pointeur pour choisir un noeuds specifique
        std::vector<SceneNode*> noeudSelectionne{nullptr};
    public:
        void       DrawScene(); //une boucle qui parcours les noeuds et les dessines chacuns
        void       AjouterCube(); //ça ajoute un cube simplement
        void       AjouterCamera3D(); //idem pour une caméra3D
        void       AjouterCamera2D(); //idem pour une caméra2D
        void       SupprimerSelection(); //pour enlever un noeud de la liste TODO faut faire attention à la mémoire
        
        //des getters pour que l'interface graphique accede aux bon noeuds etc
        SceneNode* GetSelection();
        void       SetSelection(SceneNode* noeud); //Permet à ImGui de dire "l'utilisateur a cliqué sur cet objet"
        void       Deselectionne();//pour déséléctionner l'element courrant dans le vecteur
        const std::vector<std::unique_ptr<SceneNode>>& GetNodes() const;//renvoie une reference const vers le vecteur pour eviter que generercodecomplet copie tout
        //pour la sauvgarde
        void SauvegarderProjet(std::string cheminFichier);//parcour le vecteur d'objets et remplis le json
        void ChargerProjet(std::string cheminFichier);//vide la scene actuelle et remplis avec le json lu
        void Gerer_pointeur(Camera3D camera_editeur);
};