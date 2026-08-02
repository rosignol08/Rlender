#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

int main(void) {
    // 1. Initialisation de la fenêtre Raylib
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Rlender - Test ImGui");
    SetTargetFPS(60);

    // 2. Initialisation de la caméra 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // 3. Initialisation de rlImGui
    rlImGuiSetup(true);

    // Variable pour la position de notre cube test
    float cubePosition[3] = { 0.0f, 0.0f, 0.0f };

    // Boucle principale
    while (!WindowShouldClose()) {
        
        // DESSIN
        BeginDrawing();
            ClearBackground(DARKGRAY);

            // A. Dessin de la scène 3D
            BeginMode3D(camera);
                DrawCube((Vector3){cubePosition[0], cubePosition[1], cubePosition[2]}, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires((Vector3){cubePosition[0], cubePosition[1], cubePosition[2]}, 2.0f, 2.0f, 2.0f, MAROON);
                DrawGrid(10, 1.0f);
            EndMode3D();

            // B. Dessin de l'interface graphique (Toujours APRÈS la 3D)
            rlImGuiBegin();
                ImGui::Begin("Inspecteur");
                ImGui::Text("Bienvenue dans ton editeur !");
                // Ce slider va modifier directement les coordonnées du cube
                ImGui::DragFloat3("Position Cube", cubePosition, 0.1f);
                ImGui::End();
            rlImGuiEnd();

        EndDrawing();
    }

    // Nettoyage
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
