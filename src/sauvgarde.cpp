/*
ici je defini des fonction pour les sauvgardes pour aleger le code dans le main
*/
#include <iostream>
#include <fstream>

/*
fonction utilitaire pour enregistrer un flux de char dans un fichier sur le disque dur
on a besoin du nom du fichier à sauvgarder et du contenu à mettre dedans
TODO : voir si faut donner juste le nom ou l'arborescence ici
*/
void sauvgarde(std::string nom_fichier,std::string contenu){
    std::fstream mon_fichier;
    mon_fichier.open("nom_fichier", ios::out);
}