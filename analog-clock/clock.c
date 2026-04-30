#include <math.h>
#include <raylib.h>
#include <time.h>

#define WIDTH 720
#define HEIGHT 720
#define CLOCK_RADIUS 300

float hAngle, mAngle, sAngle;
time_t curTime;
struct tm *t;
Vector2 clockCenter = {(float)WIDTH / 2, (float)HEIGHT / 2 - 20};

void updateTimeAngles() {
  time(&curTime);
  t = localtime(&curTime);

  hAngle = (t->tm_hour + ((float)t->tm_min) / 60) * ((float)360 / 12);
  mAngle = (t->tm_min + ((float)t->tm_sec / 60)) * ((float)360 / 60);
  sAngle = t->tm_sec * ((float)360 / 60);
}

Vector2 rotateVec(Vector2 v, float angle) {
  return (Vector2){v.x * cosf(angle) - v.y * sinf(angle),
                   v.x * sinf(angle) + v.y * cosf(angle)};
}

void drawClockBody() {
  DrawCircleV(clockCenter, CLOCK_RADIUS, LIGHTGRAY);

  float angle = 0.0f;
  for (int i = 0; i < 60; i++) {
    Vector2 inner;
    if (i % 5 == 0) {
      inner =
          (Vector2){clockCenter.x + CLOCK_RADIUS * 0.9 * cosf(angle * DEG2RAD),
                    clockCenter.y + CLOCK_RADIUS * 0.9 * sinf(angle * DEG2RAD)};
    } else {
      inner = (Vector2){
          clockCenter.x + CLOCK_RADIUS * 0.94 * cosf(angle * DEG2RAD),
          clockCenter.y + CLOCK_RADIUS * 0.94 * sinf(angle * DEG2RAD)};
    }
    Vector2 outer = {
        clockCenter.x + CLOCK_RADIUS * 0.99 * cosf(angle * DEG2RAD),
        clockCenter.y + CLOCK_RADIUS * 0.99 * sinf(angle * DEG2RAD)};
    if (i % 5 == 0) {
      DrawLineEx(inner, outer, 7, RED);
    } else {
      DrawLineEx(inner, outer, 5, BLACK);
    }
    angle += (float)360 / 60;
  }
}

void drawClockHands(float hAngle, float mAngle, float sAngle) {
  Vector2 hoursHand = {0, -100};
  Vector2 minutesHand = {0, -200};
  Vector2 secondsHand = {0, -260};
  hoursHand = rotateVec(hoursHand, hAngle * DEG2RAD);
  minutesHand = rotateVec(minutesHand, mAngle * DEG2RAD);
  secondsHand = rotateVec(secondsHand, sAngle * DEG2RAD);
  Vector2 hoursHandEnd = {clockCenter.x + hoursHand.x,
                          clockCenter.y + hoursHand.y};
  Vector2 minutesHandEnd = {clockCenter.x + minutesHand.x,
                            clockCenter.y + minutesHand.y};
  Vector2 secondsHandEnd = {clockCenter.x + secondsHand.x,
                            clockCenter.y + secondsHand.y};
  DrawLineEx(clockCenter, hoursHandEnd, 10, BLACK);
  DrawLineEx(clockCenter, minutesHandEnd, 6, BLACK);
  DrawLineEx(clockCenter, secondsHandEnd, 4, RED);
}

int main() {
	SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WIDTH, HEIGHT, "Analog Clock");
  SetTargetFPS(10);
  SetExitKey(KEY_Q);

  updateTimeAngles();
  while (!WindowShouldClose()) {
    updateTimeAngles();

    BeginDrawing();
    ClearBackground(BLACK);
    drawClockBody();
    drawClockHands(hAngle, mAngle, sAngle);
    DrawText(ctime(&curTime), (float)WIDTH / 2 - 124, (float)HEIGHT - 40, 20,
             WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
