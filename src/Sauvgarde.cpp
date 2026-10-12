#include "Sauvgarde.h"
#include "Managers/SceneManager.h"
#include <fstream>
#include <iostream>
#include "json.hpp"
/*
fonction utilitaire pour enregistrer un flux de char dans un fichier sur le disque dur
on a besoin du nom du fichier à sauvgarder et du contenu à mettre dedans
TODO : voir si faut donner juste le nom ou l'arborescence ici
*/
void Exporter(std::string nom_fichier,std::string contenu){
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
    if (config_json.contains("ecran")){
        config.Plein_ecran = config_json["ecran"].get<int>();
    }
    if (config_json.contains("largeur")) {
        config.screenWidth = config_json["largeur"].get<int>();
        std::cout << "enregistrement taille :" << config.screenWidth << std::endl;
    }
    if (config_json.contains("hauteur")) {
        config.screenHeight = config_json["hauteur"].get<int>();
        std::cout << "enregistrement hauteur :" << config.screenHeight << std::endl;
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
    std::ofstream file("config.json");
    if (!file) {
        std::cerr << "Erreur : impossible de créer config.json" << std::endl;
        return; //return pour stop si erreur
    }
    //si on a le fichier nikel on ecrit dedans
    nlohmann::json config_json;
    config_json["seuil_sauvegarde"] = config.limiteSauvgarde;
    config_json["ecran"] = config.Plein_ecran;
    config_json["largeur"] = config.screenWidth;
    config_json["hauteur"] = config.screenHeight;
    file << std::setw(4) << config_json;
    file.close();
}


void SceneManager::SauvegarderProjet(std::string cheminFichier) {
    //check si on a l'extention json ou pas dans le nom
    if (cheminFichier.length() < 5 || cheminFichier.substr(cheminFichier.length() - 5) != ".json") {
        cheminFichier += ".json";
    }
    nlohmann::json projet_json;
    
    // On crée un tableau JSON pour stocker notre liste de noeuds
    projet_json["noeuds"] = nlohmann::json::array();

    for (auto &noeud : sceneNodes) {
        SceneNode* actuel = noeud.get();

        //objet JSON temporaire pour ce noeud là
        nlohmann::json noeud_json;
        noeud_json["type"] = actuel->type;
        noeud_json["nom"] = actuel->nom;
        //noeud_json["isSelected"] = actuel->isSelected;//on s'en fout

        //les Vector3 de Raylib en tableaux JSON = [x, y, z]
        noeud_json["position"] = { actuel->position.x, actuel->position.y, actuel->position.z };
        noeud_json["rotation"] = { actuel->rotation.x, actuel->rotation.y, actuel->rotation.z };
        noeud_json["taille"]   = { actuel->taille.x, actuel->taille.y, actuel->taille.z };
        noeud_json["id_materiau"] = actuel->id_materiau;
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
        //si c'est une lumière :
        if(actuel->type == "SoleilNode"){
            SoleilNode* sol = dynamic_cast<SoleilNode*>(actuel);//faut dire que le noeud peut etre un soleil
            if (sol != nullptr) {
                noeud_json["puissance_lumiere"] = sol->puissance;
                noeud_json["couleur_lumiere"] = {sol->couleur_lumiere.x,sol->couleur_lumiere.y,sol->couleur_lumiere.z};                
                noeud_json["active_lumiere"] = sol->est_allume;
            }
        }
        if (actuel->type == "LightNode") {
            LightNode* lumiere = dynamic_cast<LightNode*>(actuel);
            if (lumiere != nullptr) {
                noeud_json["puissance_lumiere"] = lumiere->puissance;
                noeud_json["couleur_lumiere"] = {lumiere->couleur_lumiere.x,lumiere->couleur_lumiere.y,lumiere->couleur_lumiere.z};
                noeud_json["active_lumiere"] = lumiere->est_allume;
                noeud_json["intensite_lumiere"] = lumiere->intensite;//rayon d'atténuation
            }
        }
        if(actuel->type == "ModeleNode"){
            ModeleNode* modele = dynamic_cast<ModeleNode*>(actuel);
            if(modele != nullptr){
                noeud_json["chemin_modele"] = modele->chemin_modele;
            }
        }
        //ajout du noeud à la sauvgarde du projet
        projet_json["noeuds"].push_back(noeud_json);
    }
    projet_json["materiaux"] = nlohmann::json::array();
    for(auto & materiel : banque_materiaux.GetTousLesMateriaux()){
        nlohmann::json materiel_json;//materiel temporaire
        materiel_json["identifiant"] = materiel.identifiant;
        materiel_json["nom"] = materiel.nom;

        materiel_json["couleurAlbedo"] = {materiel.couleurAlbedo.r,materiel.couleurAlbedo.g,materiel.couleurAlbedo.b,materiel.couleurAlbedo.a};
        nlohmann::json textures_json = nlohmann::json::object();

        for(auto const& [type, chemin] : materiel.chemins_textures){
            textures_json[type] = chemin; 
            // ça créera par exemple : "albedo": "assets/bois.png"
        }
        materiel_json["chemins_textures"] = textures_json;
        //pour le shader
        //Il faut ajouter "std::string nom_shader;" dans la struct DataMateriel
        materiel_json["nom_shader_actuel"] = materiel.nom_shader_actuel;
        projet_json["materiaux"].push_back(materiel_json);
        
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

//vide la scene actuelle et remplis avec le json lu
void SceneManager::ChargerProjet(std::string cheminFichier){
    std::ifstream file(cheminFichier);
    if (!file) {
        std::cerr << "Erreur : problème à l'ouverture du fichier " << cheminFichier << std::endl;
        return;
    }

    nlohmann::json projet_json;
    try {
        file >> projet_json; //on parse le fichier en mémoire avant d'écraser la scène
    } catch (const nlohmann::json::parse_error& erreur) {
        std::cerr << "Erreur : fichier JSON invalide pour " << cheminFichier
                  << " (" << erreur.what() << ")" << std::endl;
        file.close();
        return;
    }
    file.close();

    if (!projet_json.is_object() || !projet_json.contains("noeuds") || !projet_json["noeuds"].is_array()) {
        std::cerr << "Erreur : structure JSON invalide pour " << cheminFichier << std::endl;
        return;
    }

    banque_materiaux.NettoyerTout();

    for (auto& element_json : projet_json["materiaux"]) {
        if (!element_json.is_object()) {
            continue;
        }
        
        DataMateriel * pointeur_element = banque_materiaux.GetMateriau(banque_materiaux.CreerMateriau(element_json["nom"]));
        pointeur_element->identifiant = element_json["identifiant"];
        pointeur_element->couleurAlbedo.r = element_json["couleurAlbedo"][0];
        pointeur_element->couleurAlbedo.g = element_json["couleurAlbedo"][1];
        pointeur_element->couleurAlbedo.b = element_json["couleurAlbedo"][2];
        pointeur_element->couleurAlbedo.a = element_json["couleurAlbedo"][3];

        pointeur_element->chemins_textures = element_json["chemins_textures"];
        pointeur_element->nom_shader_actuel = element_json["nom_shader_actuel"];
        
    }
    for(auto & elem : banque_materiaux.GetTousLesMateriaux()){
        //load toutes les textures
        for (auto & kv : elem.chemins_textures) {
            banque_materiaux.ChargerTextureAlbedo(elem.identifiant, kv.second);
        }
    }

    std::vector<std::unique_ptr<SceneNode>> nouvelle_scene;
    nouvelle_scene.reserve(static_cast<size_t>(projet_json["noeuds"].size()));

    for (auto& element_json : projet_json["noeuds"]) {
        if (!element_json.is_object()) {
            continue;
        }

        if (!element_json.contains("type") || !element_json["type"].is_string()) {
            continue;
        }

        std::string le_type = element_json["type"].get<std::string>();

        std::unique_ptr<SceneNode> nouveau_noeud = nullptr;
        if(le_type == "cube"){
            nouveau_noeud = std::make_unique<CubeNode>();
        }
        else if(le_type == "sphere"){
            nouveau_noeud = std::make_unique<SphereNode>();
        }
        else if(le_type == "camera3D"){
            if (!element_json.contains("target") || !element_json["target"].is_array() ||
                !element_json.contains("fovy") || !element_json.contains("mode_camera") ||
                !element_json.contains("projetction_cam")) {
                continue;
            }

            auto cam = std::make_unique<CameraNode>();//vu que c'est une camera faut aussi stoquer les info de la camera
            cam->target.x = element_json["target"][0];
            cam->target.y = element_json["target"][1];
            cam->target.z = element_json["target"][2];

            cam->fovy = element_json["fovy"];
            cam->mode_camera = element_json["mode_camera"];
            cam->projetction_cam = element_json["projetction_cam"];
            nouveau_noeud = std::move(cam);//faut le mettre dans le pointeur generique
        }
        else if(le_type == "camera2D"){//pareil c'est une camera donc on stoque
            if (!element_json.contains("zoom_camera")) {
                continue;
            }

            auto cam = std::make_unique<Camera2DNode>();
            cam->zoom_camera = element_json["zoom_camera"];
            nouveau_noeud = std::move(cam);
        }
        else if (le_type == "SoleilNode") {
            auto sol = std::make_unique<SoleilNode>();
            if (element_json.contains("puissance_lumiere")) sol->puissance = element_json["puissance_lumiere"];
            if (element_json.contains("active_lumiere")) sol->est_allume = element_json["active_lumiere"];
            if (element_json.contains("couleur_lumiere")) {
                sol->couleur_lumiere = { element_json["couleur_lumiere"][0], element_json["couleur_lumiere"][1], element_json["couleur_lumiere"][2] };
            }
            nouveau_noeud = std::move(sol);
        }
        else if (le_type == "LightNode") {
            auto lum = std::make_unique<LightNode>();
            if (element_json.contains("puissance_lumiere")) lum->puissance = element_json["puissance_lumiere"];
            if (element_json.contains("intensite_lumiere")) lum->intensite = element_json["intensite_lumiere"];
            if (element_json.contains("active_lumiere")) lum->est_allume = element_json["active_lumiere"];
            if (element_json.contains("couleur_lumiere")) {
                lum->couleur_lumiere = { element_json["couleur_lumiere"][0], element_json["couleur_lumiere"][1], element_json["couleur_lumiere"][2] };
            }
            nouveau_noeud = std::move(lum);
        }else if(le_type == "ModeleNode"){
            auto mod = std::make_unique<ModeleNode>();
            if(element_json.contains("chemin_modele")){
                mod->chemin_modele = element_json["chemin_modele"];
                mod->ChargerFichier3D(mod->chemin_modele, &banque_modeles);
            }
            nouveau_noeud = std::move(mod);
            //je connais pas les autre propriétées specifiques au modele a charger et sauvgarder
        }
        else{//par défaut si c'est rien on skip le noeud
            continue;
        }

        if (nouveau_noeud == nullptr) {//faut remplir le noeud avec les info lue de base DRY
            continue;
        }

        if (!element_json.contains("nom") || !element_json.contains("isSelected") ||
            !element_json.contains("couleur") || !element_json.contains("position") ||
            !element_json.contains("rotation") || !element_json.contains("taille")) {
            continue;
        }

        nouveau_noeud->type = le_type;
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
        if (element_json.contains("id_materiau")) {
            nouveau_noeud->id_materiau = element_json["id_materiau"];
        } else {
            nouveau_noeud->id_materiau = 0; //le matériau par défaut
        }
        nouvelle_scene.push_back(std::move(nouveau_noeud));//move pour déplacer la propriété du pointeur
    }
    
    Deselectionne();
    sceneNodes = std::move(nouvelle_scene);
    //pour reselectioner les objets
    for (auto& noeud : sceneNodes) {
        if (noeud->isSelected) {
            noeudSelectionne.push_back(noeud.get());
        }
    }
}
