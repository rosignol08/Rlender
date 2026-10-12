/*
pour l'instant c'est pas la priorotée mais 
c'est pour mesurer le poid des ressources
*/
#include <stdio.h>
#include "raylib.h"
#include "Managers/SceneManager.h"

//poids des modeles
int poid_modeles(){
    size_t taille_modeles_octets = 0;
    for (const auto& [nom, modele] : banque_modeles.dictionnaire_modeles) {
        for (int i = 0; i < modele.meshCount; i++) {
            Mesh mesh = modele.meshes[i];
            // Positions (x,y,z = 3 floats)
            taille_modeles_octets += mesh.vertexCount * 3 * sizeof(float);
            
            // Normales (x,y,z)
            if (mesh.normals != nullptr) taille_modeles_octets += mesh.vertexCount * 3 * sizeof(float);
            
            // Coordonnées de texture (u,v = 2 floats)
            if (mesh.texcoords != nullptr) taille_modeles_octets += mesh.vertexCount * 2 * sizeof(float);
            
            // Indices (Triangles) -> un unsigned short pèse 2 octets
            if (mesh.indices != nullptr) taille_modeles_octets += mesh.triangleCount * 3 * sizeof(unsigned short);
        }
    }
}

int poids_textures(){
    size_t taille_textures_octets = 0;
    for (const auto& mat : banque_materiaux.GetTousLesMateriaux()) {
        if (mat.textureAlbedo.id != 0) {
            // Largeur * Hauteur * 4 canaux (Rouge, Vert, Bleu, Alpha)
            taille_textures_octets += mat.textureAlbedo.width * mat.textureAlbedo.height * 4;

            // Si tu utilises le mipmapping (GenTextureMipmaps), il faut rajouter ~33% au poids
            // taille_textures_octets += (taille_textures_octets / 3); 
        }
    }
}

int poids_shaders(){
    size_t taille_shaders_octets = shaderManager.dictionnaire_shaders.size() * 4096; // Estimation ~4 Ko par shader (voir autre technique)
}

/*
*** faudrait compiler dans un threads ? et 
#include <filesystem>
size_t taille_executable = 0;
try {
    // /proc/self/exe est un raccourci Linux vers l'exécutable en cours d'utilisation
    taille_executable = std::filesystem::file_size("/proc/self/exe"); 
} catch (...) {
    taille_executable = 0;
}
*/

//fonction utilitaire
float en_mo = (float)octets / (1024.0f * 1024.0f);

void affichage_taille(){
    ImGui::Begin("Profiler : Utilisation Mémoire");
    // Exemple d'affichage ImGui
    ImGui::Text("Exécutable (Moteur) : %.2f Mo", (float)taille_executable / 1048576.0f);
    ImGui::Separator();
    ImGui::Text("Mémoire VRAM (Carte Graphique) :");
    ImGui::Text(" - Modèles 3D : %.2f Mo", (float)taille_modeles_octets / 1048576.0f);
    ImGui::Text(" - Textures : %.2f Mo", (float)taille_textures_octets / 1048576.0f);
    ImGui::Text(" - Shaders : %.2f Ko", (float)taille_shaders_octets / 1024.0f); // En Ko car très petit

    size_t total_vram = taille_modeles_octets + taille_textures_octets + taille_shaders_octets;
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "TOTAL VRAM : %.2f Mo", (float)total_vram / 1048576.0f);

    // Barre de progression visuelle (en assumant un budget imaginaire de 512 Mo)
    ImGui::ProgressBar((float)total_vram / (512.0f * 1048576.0f), ImVec2(-1, 0), "Budget 512 Mo");

    ImGui::End();
}