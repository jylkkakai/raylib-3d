#include <cmath>
#include <iostream>
#include <raylib.h>
#include <string>

Vector4 normQ(Vector4 q) {

  float d = std::sqrt(std::pow(q.w, 2) + std::pow(q.x, 2) + std::pow(q.y, 2) +
                      std::pow(q.z, 2));
  return Vector4({q.x / d, q.y / d, q.z / d, q.w / d});
}

Vector3 normV3(Vector3 q) {

  float d = std::sqrt(std::pow(q.x, 2) + std::pow(q.y, 2) + std::pow(q.z, 2));
  return Vector3({q.x / d, q.y / d, q.z / d});
}
Vector4 invQ(Vector4 q) { return Vector4({-q.x, -q.y, -q.z, q.w}); }
bool isEqual(Vector2 a, Vector2 b) { return a.x == b.x && a.y == b.y; }
Vector3 mulScalar(Vector3 q, float s) {
  return Vector3({q.x * s, q.y * s, q.z * s});
}
Vector2 subV2(Vector2 a, Vector2 b) { return Vector2({a.x - b.x, a.y - b.y}); }

Vector3 crossProduct(Vector3 a, Vector3 b) {

  return Vector3(
      {a.y * b.z - a.z * b.y, a.x * b.z - a.z * b.x, a.x * b.y - a.y * b.x});
}

Vector4 mulQ(Vector4 a, Vector4 b) {
  return Vector4({a.w * b.x + a.x * b.w - a.y * b.z + a.z * b.y,
                  a.w * b.y + a.x * b.z + a.y * b.w - a.z * b.x,
                  a.w * b.z - a.x * b.y + a.y * b.x + a.z * b.w,
                  a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z});
}

Vector4 toQ(Vector3 r) {

  double cr = cos(r.x * 0.5);
  double sr = sin(r.x * 0.5);
  double cp = cos(r.y * 0.5);
  double sp = sin(r.y * 0.5);
  double cy = cos(r.z * 0.5);
  double sy = sin(r.z * 0.5);

  Vector4 q;
  q.w = cr * cp * cy + sr * sp * sy;
  q.x = sr * cp * cy - cr * sp * sy;
  q.y = cr * sp * cy + sr * cp * sy;
  q.z = cr * cp * sy - sr * sp * cy;

  return q;
}
Vector3 rotate(Vector3 r, Vector3 v) {
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

  Camera camera = {
      {0, 0.0f, 100.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};
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

  Vector2 center = {float(screenWidth) / 2, float(screenHeight) / 2};
  Vector2 mousePos;
  Vector2 prevMousePos;
  Vector3 ax = {0, 0, 0};
  int wait = 0;
  while (!WindowShouldClose()) {

    mousePos = subV2(GetMousePosition(), center);
    ax = {0, 0, 0};
    if (IsKeyDown(KEY_Q) && !wait) {
      ax.z = -0.0175f;
    }
    if (IsKeyDown(KEY_E) && !wait) {
      ax.z = 0.0175f;
    }
    if (IsKeyDown(KEY_A) && !wait) {
      ax.y = -0.0175f;
    }
    if (IsKeyDown(KEY_D) && !wait) {
      ax.y = 0.0175f;
    }
    if (IsKeyDown(KEY_W) && !wait) {
      ax.x = -0.0175f;
    }
    if (IsKeyDown(KEY_S) && !wait) {
      ax.x = 0.0175f;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (!isEqual(mousePos, prevMousePos)) {
        ax = mulScalar(
            normV3(crossProduct(
                Vector3({mousePos.x, mousePos.y, camera.position.z}),
                Vector3({prevMousePos.x, prevMousePos.y, camera.position.z}))),
            0.03f);
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    std::string strMousePos = std::to_string(ax.x) + " " +
                              std::to_string(ax.y) + " " + std::to_string(ax.z);
    DrawText(strMousePos.c_str(), 20, 20, 20, GRAY);

    BeginMode3D(camera);

    Color color[8] = {RED, BLUE, GREEN, MAGENTA, YELLOW, BROWN, PURPLE, MAROON};
    for (size_t i = 0; i < 8; i++) {
      sphPos[i] = rotate(ax, sphPos[i]);
      DrawSphere(sphPos[i], 10, color[i]);
    }

    EndMode3D();
    EndDrawing();
    if (wait > 0)
      wait--;
    prevMousePos = mousePos;
  }
  return 0;
}
