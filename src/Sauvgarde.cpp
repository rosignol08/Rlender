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


//ça remplis les variables parametre par les info du fichier config.json
void ChargerConfig(Parametres& config){
    std::ifstream file("config.json");
    if (!file) {
        std::cerr << "Erreur : problème à l'ouverture du fichier config.json" << std::endl;
        return;
    }
    nlohmann::json config_json;
    file >> config_json; //on met le fichier dans le json et on le parse
    if (config_json.contains("seuil_sauvegarde")) {
        config.limiteSauvgarde = config_json["seuil_sauvegarde"].get<float>();
    }
    if (config_json.contains("largeur")) {
        config.screenWidth = config_json["largeur"].get<int>();
    }
    if (config_json.contains("hauteur")) {
        config.screenHeight = config_json["hauteur"].get<int>();
    }
}

int Cree_Fichier(std::string nom_fichier){
    std::fstream my_file;
	my_file.open(nom_fichier, std::ios::out);
	if (!my_file) {
		std::cout << "DEBUG : fichier pas cree";
	}
	else {
		std::cout << "DEBUG : fichier cree";
		my_file.close(); 
	}
	return 0;
}

void SauvegarderConfig(const Parametres& config){
    std::ifstream file("config.json");
    if (!file) {
        //faut crée le fichier la comme ça c'est nikel
        std::fstream 
        std::cerr << "Erreur : problème à l'ouverture du fichier config.json" << std::endl;
        return;
    }
    nlohmann::json config_json;
    file >> config_json; //on met le fichier dans le json et on le parse
}
