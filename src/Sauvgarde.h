/*
ici je defini des fonction pour les sauvgardes pour aleger le code dans le main
*/

#pragma once //empeche les boucles
#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"
#include "json.hpp"

void Sauvgarde(std::string nom_fichier,std::string contenu);

//pour les parametres
void ChargerConfig(Parametres& config);
void SauvegarderConfig(const Parametres& config);
