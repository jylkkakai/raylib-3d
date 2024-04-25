#include <cmath>
#include <iostream>
#include <raylib.h>

Vector4 normQ(Vector4 q) {

  float d = std::sqrt(std::pow(q.w, 2) + std::pow(q.x, 2) + std::pow(q.y, 2) +
                      std::pow(q.z, 2));
  return Vector4({q.x / d, q.y / d, q.z / d, q.w / d});
}

Vector4 invQ(Vector4 q) { return Vector4({-q.x, -q.y, -q.z, q.w}); }

Vector4 mulQ(Vector4 a, Vector4 b) {
  return Vector4({a.w * b.x + a.x * b.w - a.y * b.z + a.z * b.y,
                  a.w * b.y + a.x * b.z + a.y * b.w - a.z * b.x,
                  a.w * b.z - a.x * b.y + a.y * b.x + a.z * b.w,
                  a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z});
}

Vector4 toQ(Vector3 r) {

  double cr = cos(r.x * 0.5);
  double sr = sin(r.x * 0.5);
  double cp = cos(0.0175f * 0.5);
  double sp = sin(0.0175f * 0.5);
  double cy = cos(r.z * 0.5);
  double sy = sin(r.z * 0.5);

  Vector4 q;
  q.w = cr * cp * cy + sr * sp * sy;
  q.x = sr * cp * cy - cr * sp * sy;
  q.y = cr * sp * cy + sr * cp * sy;
  q.z = cr * cp * sy - sr * sp * cy;

  return q;
}
Vector3 rotate(Vector3 r, Vector3 v, float a) {
  Vector4 q = toQ(r);
  // Vector4 q = {q.w = std::cos(a / 2), q.x = r.x * std::sin(a / 2),
  //              q.y = r.y * std::sin(a / 2), q.z = r.z * std::sin(a / 2)};
  // q = normQ(q);
  Vector4 p = {v.x, v.y, v.z, 0};
  Vector4 invq = invQ(q);
  // Vector4 qp = mulQ(q, p);
  // Vector4 result = mulQ(qp, invq);
  Vector4 qp = mulQ(p, q);
  Vector4 result = mulQ(invq, qp);
  return Vector3({result.x, result.y, result.z});
}

int main() {

  std::cout << "Hello, World!" << std::endl;
  const int screenWidth = 1200;
  const int screenHeight = 675;

  InitWindow(screenWidth, screenHeight, "Test3d");

  Camera camera = {{100.0f, 100.0f, 100.0f},
                   {0.0f, 0.0f, 0.0f},
                   {0.0f, 1.0f, 0.0f},
                   45.0f,
                   0};
  Vector3 boxPos = {20.0f, 0.0f, 0.0f};
  Vector3 boxSize = {10.0f, 10.0f, 10.0f};
  Vector3 sphPos[8];
  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      for (size_t k = 0; k < 2; k++) {
        Vector3 pos = {-15 + 30 * float(i), -15 + 30 * float(j),
                       -15 + 30 * float(k)};
        sphPos[i * 4 + j * 2 + k] = pos;
      }
    }
  }
  SetTargetFPS(60);

  int wait = 0;
  // camera.up = Vector3({0, 0, 1});
  while (!WindowShouldClose()) {

    // UpdateCamera(&camera, CAMERA_ORBITAL);
    // std::cout << camera.position.x << " " << camera.position.y << " "
    //           << camera.position.x << std::endl;
    // std::cout << camera.up.x << " " << camera.up.y << " " << camera.up.z
    //           << std::endl;

    // if (IsKeyDown(KEY_M)) {
    //   if (camera.projection == 1)
    //     camera.projection = 0;
    //   else
    //     camera.projection = 1;
    // }

    if (IsKeyDown(KEY_Q) && !wait) {
      wait = 5;
      std::cout << camera.position.x << " " << camera.position.y << " "
                << camera.position.x << std::endl;
      camera.up.x += 0.1;
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }
    if (IsKeyDown(KEY_E) && !wait) {
      wait = 5;
      camera.up.x -= 0.1;
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }
    if (IsKeyDown(KEY_A) && !wait) {
      wait = 5;
      camera.up.y -= 0.1;
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }
    if (IsKeyDown(KEY_D) && !wait) {
      wait = 5;
      camera.up.y += 0.1;
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }
    if (IsKeyDown(KEY_W) && !wait) {
      wait = 5;
      std::cout << camera.position.x << " " << camera.position.y << " "
                << camera.position.x << std::endl;
      camera.up.z += 0.1;
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }
    if (IsKeyDown(KEY_S) && !wait) {
      wait = 5;
      camera.up.z -= 0.1;
      UpdateCamera(&camera, CAMERA_ORBITAL);
    }

    BeginDrawing();

    BeginMode3D(camera);
    ClearBackground(RAYWHITE);

    Color color[8] = {RED, BLUE, GREEN, MAGENTA, YELLOW, BROWN, PURPLE, MAROON};
    for (size_t i = 0; i < 8; i++) {
      Vector3 ax = {0, 1, 0};
      // sphPos[i] = rotate(ax, sphPos[i], 0.0175f);
      sphPos[i] = rotate(ax, sphPos[i], 5.0f);
      DrawSphere(sphPos[i], 10, color[i]);
    }

    EndMode3D();
    EndDrawing();
    if (wait > 0)
      wait--;
  }
  return 0;
}
