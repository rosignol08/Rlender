#include "Sauvgarde.h"

/*
fonction utilitaire pour enregistrer un flux de char dans un fichier sur le disque dur
on a besoin du nom du fichier à sauvgarder et du contenu à mettre dedans
TODO : voir si faut donner juste le nom ou l'arborescence ici
*/
void Sauvgarde(std::string nom_fichier,std::string contenu){
    std::fstream mon_fichier;
    //mon_fichier.open(nom_fichier, std::ios::trunc);//pour effacer
    //if(!mon_fichier){
    //    std::cerr << "ERREUR : je peut pas ouvir le fichier " << std::endl;
    //}
    //mon_fichier.close();
    //ensuite on réécrit dedans maintenant qu'il est vide
    mon_fichier.open(nom_fichier, std::ios::out);//pour ecrire
    if(!mon_fichier){
        std::cerr << "ERREUR : je peut pas ouvir le fichier " << std::endl;
    }else{
        //si tout est nikel on peut ecrire dedans
        mon_fichier << contenu;
    }
    mon_fichier.close();
}