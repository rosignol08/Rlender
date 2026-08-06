/*
ici je defini des fonction pour les sauvgardes pour aleger le code dans le main
*/
#include <iostream>
#include <fstream>
#pragma once //empeche les boucles
#include <string>
void Sauvgarde(std::string nom_fichier,std::string contenu);

//pour les parametres
void ChargerConfig(Parametres& config);
void SauvegarderConfig(const Parametres& config);
