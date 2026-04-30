#include <math.h>
#include <raylib.h>

#define WIDTH 900
#define HEIGHT 700
#define ASPECT_RATIO (double)HEIGHT / WIDTH;

double xl, xm, yl, ym, step;
Vector2 origin;

double func1(double x) { return x * sin(x); }
double func2(double x) { return x; }
double func3(double x) { return -x; }

void init() {
  xl = -10;
  xm = 10;
  yl = xl * ASPECT_RATIO;
  ym = xm * ASPECT_RATIO;
  step = 0.05;
  origin = (Vector2){(float)WIDTH / 2, (float)HEIGHT / 2};
}

double scale(double val, double a, double min, double max) {
  return val * a / (max - min);
}

Vector2 shiftPoint(Vector2 p) {
  return (Vector2){p.x + origin.x, p.y + origin.y};
}

void drawGraphAxes() {
  DrawLineEx((Vector2){origin.x, 0}, (Vector2){origin.x, HEIGHT}, 2, DARKGRAY);
  DrawLineEx((Vector2){0, origin.y}, (Vector2){WIDTH, origin.y}, 2, DARKGRAY);
  DrawText(TextFormat("%.2f", ym), origin.x + 2, 2, 10, GRAY);
  DrawText(TextFormat("%.2f", yl), origin.x + 2, HEIGHT - 10, 10, GRAY);
  DrawText(TextFormat("%.2f", xl), 2, origin.y - 11, 10, GRAY);
  DrawText(TextFormat("%.2f", xm), WIDTH - 30, origin.y - 11, 10, GRAY);
}

void plotFunction(double (*f)(double x), Color c) {
  for (double i = xl; i < xm; i += step) {
    double xp = scale(i, WIDTH, xl, xm);
    double yp = -scale(f(i), HEIGHT, yl, ym);
    double nxp = scale(i + step, WIDTH, xl, xm);
    double nyp = -scale(f(i + step), HEIGHT, yl, ym);
    Vector2 p1 = {xp, yp};
    Vector2 p2 = {nxp, nyp};
    DrawLineEx(shiftPoint(p1), shiftPoint(p2), 2, c);
  }
}

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WIDTH, HEIGHT, "Graph Plotter");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  init();
  while (!WindowShouldClose()) {
    double move = 2 * step;
    if (IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) {
      xl -= move;
      xm += move;
    }
    if (IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD)) {
      xl += move;
      xm -= move;
    }

    yl = xl * ASPECT_RATIO;
    ym = xm * ASPECT_RATIO;

    BeginDrawing();
    ClearBackground(BLACK);
    drawGraphAxes();
    plotFunction(&func1, SKYBLUE);
    plotFunction(&func2, GREEN);
    plotFunction(&func3, GREEN);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
