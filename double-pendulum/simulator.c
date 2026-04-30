#include <math.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 750
#define TRAIL_LENGTH 128
#define TRAIL_DOT_SIZE 2.5
#define G 1250

Vector2 start = {(float)WIDTH / 2, (float)HEIGHT / 2};
Vector2 end;
float l1 = 150, l2 = 100, m1 = 15, m2 = 15, phi1, phi2;
float phi1d, phi2d, phi1dd, phi2dd;

Vector2 trailPositions[TRAIL_LENGTH] = {0};

bool paused = false;

Vector2 getEndVec(Vector2 start, float l, float phi) {
  return (Vector2){start.x + l * sinf(phi), start.y + l * cosf(phi)};
}

void init() {
  phi1 = GetRandomValue(-180, 180) * DEG2RAD;
  phi2 = GetRandomValue(-180, 180) * DEG2RAD;
  phi1d = 0;
  phi2d = 0;
  for (int i = 0; i < TRAIL_LENGTH; i++) {
    trailPositions[i] = (Vector2){0, 0};
  }
}

void update(float dt) {
  phi1dd = (-G * (2 * m1 + m2) * sinf(phi1) - m2 * G * sinf(phi1 - 2 * phi2) -
            2 * sinf(phi1 - phi2) * m2 *
                (phi2d * phi2d * l2 + phi1d * phi1d * l1 * cosf(phi1 - phi2))) /
           (l1 * (2 * m1 + m2 - m2 * cosf(2 * (phi1 - phi2))));
  phi2dd = (2 * sinf(phi1 - phi2) *
            (phi1d * phi1d * l1 * (m1 + m2) + G * (m1 + m2) * cosf(phi1) +
             phi2d * phi2d * l2 * m2 * cosf(phi1 - phi2))) /
           (l2 * (2 * m1 + m2 - m2 * cosf(2 * (phi1 - phi2))));

  phi1 += phi1d * dt;
  phi2 += phi2d * dt;
  phi1d += phi1dd * dt;
  phi2d += phi2dd * dt;
}

void updateTrailPositions(Vector2 currentPos) {
  for (int i = TRAIL_LENGTH - 2; i >= 0; i--) {
    trailPositions[i + 1] = trailPositions[i];
  }
  trailPositions[0] = currentPos;
}

void drawPendulum(Vector2 start, float l, float r, float phi) {
  Vector2 end = getEndVec(start, l, phi);
  DrawLineEx(start, end, 3, GRAY);
  DrawCircleV(end, r, WHITE);
}

void drawTrail() {
  for (int i = TRAIL_LENGTH - 1; i >= 0; i--) {
    DrawCircleV(trailPositions[i], TRAIL_DOT_SIZE,
                (Color){0, 0, 255, 255 - (i * 256 / TRAIL_LENGTH)});
  }
}

int main() {
	SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WIDTH, HEIGHT, "Double Pendulum");
  SetTargetFPS(180);
  SetExitKey(KEY_Q);

  init();
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_ENTER)) {
      init();
    } else if (IsKeyPressed(KEY_SPACE)) {
      paused = !paused;
    }

    float dt = GetFrameTime();
    if (!paused) {
      update(dt);
      updateTrailPositions(end);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    drawTrail();
    Vector2 mid = getEndVec(start, l1, phi1);
    end = getEndVec(mid, l2, phi2);
    drawPendulum(mid, l2, m2, phi2);
    drawPendulum(start, l1, m1, phi1);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
