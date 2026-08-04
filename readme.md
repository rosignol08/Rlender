faut lancer 
```bash
#à la racine du projet
rm -rf build/*

cd build

cmake ..

make
```


[ Interface ImGui ] ---> modifie ---> [ Scene Manager ]
       |                                      |
       v                                      v
[ Code Generator ] <--- lit le --- [ Scene Graph (Noeuds) ]
       |                                      |
       v                                      v
[ Fenêtre Code C++ ]                  [ Viewport Raylib ]


1. Le Modèle de Données (SceneNode)
C'est la brique de base de tout élément dans ta scène. Tu utilises le polymorphisme pour que chaque type de forme (Cube, Sphère, Lumière) sache comment se dessiner et comment générer son propre code C++.

```C++
#include <string>
#include <sstream>
#include "raylib.h"

// Classe de base pour tous les objets de ta scène
class SceneNode {
public:
    std::string name;
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 rotation = {0.0f, 0.0f, 0.0f};
    Vector3 scale = {1.0f, 1.0f, 1.0f};
    Color color = WHITE;
    bool isSelected = false;

    virtual ~SceneNode() = default;

    // 1. Comment l'objet se dessine dans l'éditeur
    virtual void Draw() = 0; 

    // 2. Comment l'objet génère sa ligne de code C++
    virtual std::string ToCode() = 0; 
};

// Exemple concret : Un Cube
class CubeNode : public SceneNode {
public:
    CubeNode() { name = "Cube"; }

    void Draw() override {
        DrawCubeV(position, scale, color);
        if (isSelected) {
            DrawCubeWiresV(position, scale, YELLOW); // Contour si sélectionné
        }
    }

    std::string ToCode() override {
        std::stringstream ss;
        ss << "    DrawCubeV({" << position.x << "f, " << position.y << "f, " << position.z 
           << "f}, {" << scale.x << "f, " << scale.y << "f, " << scale.z 
           << "f}, {" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", 255});\n";
        return ss.str();
    }
};

```


## 2. Le Gestionnaire de Scène (SceneManager)
Une simple classe qui contient la liste de tous les objets actifs, gère l'objet actuellement sélectionné (pour pouvoir modifier ses coordonnées avec l'interface), et permet d'ajouter/supprimer des nœuds.

Contient : `std::vector<std::unique_ptr<SceneNode>> nodes;`

Méthodes : 
- AddNode(),
- RemoveNode(),
- GetSelectedNode(),
- DrawAll().

## 3. Le Générateur de Code (CodeGenerator)
C'est le traducteur.
Quand la scène est modifiée, il prend le SceneManager, crée l'en-tête du fichier (#include "raylib.h", le int main(), l'initialisation de la fenêtre), fait une boucle sur tous les nodes pour appeler leur ToCode(), et referme la boucle de rendu.

## 4. L'Interface Graphique (EditorUI)
Gérée par ImGui. Elle affiche :

- Un panneau Hiérarchie : la liste de tous les objets du SceneManager.

- Un panneau Inspecteur : des sliders (ImGui::DragFloat3, ImGui::ColorEdit4) reliés directement à la position et à la color de l'objet sélectionné.

- Un panneau Code Exporter : un grand champ de texte en lecture seule (ImGui::InputTextMultiline) qui affiche la std::string générée par le CodeGenerator, avec un bouton "Copier dans le presse-papier" (ImGui::SetClipboardText()).

## 1.Configurer Raylib et rlImGui :

(La base de l'application.)

Crée une fenêtre Raylib classique, initialise ImGui avec la bibliothèque open-source rlImGui.

Essayer d'afficher une simple fenêtre ImGui au-dessus d'un'fond d'écran Raylib.

## 2.Créer la structure SceneNode et la boucle de rendu :

(Afficher les objets en mémoire.)

Implémentation des classes `SceneNode`, `CubeNode` et `SphereNode`.

Ajout de 2 ou 3 objets en dur dans un `std::vector` et appelle de leur méthode `Draw()` dans un bloc `BeginMode3D(camera)` dans la boucle principale.

## 3.Connecter l'Inspecteur ImGui aux objets :

(Manipulation en temps réel.)

Dans ton interface ImGui, crée des curseurs reliés aux coordonnées de l'objet sélectionné. Quand tu bouges le slider ImGui de la coordonnée X, ton cube doit bouger instantanément dans ta fenêtre Raylib.

## 4.Créer le Générateur de Code et le Panneau de Sortie :

(L'export.)

Faut écrire la fonction qui assemble le texte du code C++ final à partir des objets de la scène. 
Affiche cette chaîne de caractères dans une fenêtre ImGui textuelle avec un bouton de copie.

## 5.Isoler le Viewport :
(Optionnel mais pro)

Le confort visuel style Blender.
Au lieu de dessiner la 3D directement sur le fond de la fenêtre, dessine ta scène Raylib dans une RenderTexture2D, puis affiche cette texture comme une image à l'intérieur d'une fenêtre ImGui dédiée. Cela te donne un véritable espace de travail modulable.

## Pour commencer :
rlImGui (Raylib + ImGui) : Le dépôt GitHub officiel de raylib-extras/rlImGui. C'est le pont le plus propre pour lier les deux bibliothèques.

Dear ImGui Demo : Quand tu auras ImGui d'installé, appelle la fonction ImGui::ShowDemoWindow(); dans ta boucle. C'est une mine d'or interactive qui montre tous les widgets possibles (boutons, sliders, arbres, onglets) avec le code C++ correspondant juste à côté !


remerciments :
pour le parser json
- https://github.com/nlohmann/json