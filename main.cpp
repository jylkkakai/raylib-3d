#include <iostream>
#include <raylib.h>

int main() {

  std::cout << "Hello, World!" << std::endl;
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Test3d");

  Camera camera = {
      {50.0f, 50.0f, 50.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};
  Vector3 boxPos = {0.0f, 5.0f, 0.0f};
  Vector3 boxSize = {10.0f, 10.0f, 10.0f};
  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    if (IsKeyDown(KEY_M)) {
      if (camera.projection == 1)
        camera.projection = 0;
      else
        camera.projection = 1;
    }
    if (IsKeyDown(KEY_Q)) {
      camera.fovy += 1;
    } else if (IsKeyDown(KEY_E)) {
      camera.fovy -= 1;
    }
    if (IsKeyDown(KEY_A)) {
      camera.position.x -= 1;
    }
    if (IsKeyDown(KEY_D)) {
      camera.position.x += 1;
    }
    if (IsKeyDown(KEY_W)) {
      camera.position.y += 1;
    }
    if (IsKeyDown(KEY_S)) {
      camera.position.y -= 1;
    }
    if (IsKeyDown(KEY_Z)) {
      camera.position.z += 1;
    }
    if (IsKeyDown(KEY_X)) {
      camera.position.z -= 1;
    }

    BeginDrawing();

    BeginMode3D(camera);
    ClearBackground(RAYWHITE);

    DrawCube(boxPos, boxSize.x, boxSize.y, boxSize.z, GRAY);
    DrawCubeWires(boxPos, boxSize.x, boxSize.y, boxSize.z, BLACK);
    DrawGrid(100, 1.0f);
    EndMode3D();
    EndDrawing();
  }
  return 0;
}
