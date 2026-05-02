#include <raylib.h>
#include <stdbool.h>

#define WIDTH 90
#define HEIGHT 70
#define SIZE 10

bool grid[WIDTH * HEIGHT] = {0};
bool copy[WIDTH * HEIGHT] = {0};

bool playing = false;

#define cell(row, col) grid[row * WIDTH + col]
#define cellCopy(row, col) copy[row * WIDTH + col]

int getAliveCount(int y, int x) {
  int alive = 0;
  for (int r = y - 1; r <= y + 1; r++) {
    for (int c = x - 1; c <= x + 1; c++) {
      if (r == y && c == x) {
        continue;
      }
      if (cell(r, c)) {
        alive++;
      }
    }
  }

  return alive;
}

void updateGrid() {
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    grid[i] = copy[i];
  }
}

void updateState() {
  for (int r = 1; r < HEIGHT - 1; r++) {
    for (int c = 1; c < WIDTH - 1; c++) {
      int alive = getAliveCount(r, c);
      if (cell(r, c)) {
        if (alive < 2 || alive > 3) {
          cellCopy(r, c) = false;
          continue;
        }
      } else {
        if (alive == 3) {
          cellCopy(r, c) = true;
          continue;
        }
      }
    }
  }
  updateGrid();
}

void drawGrid() {
  for (int r = 1; r < HEIGHT - 1; r++) {
    for (int c = 1; c < WIDTH - 1; c++) {
      if (cell(r, c)) {
        DrawRectangle(c * SIZE, r * SIZE, SIZE, SIZE, WHITE);
      }
    }
  }
}

void drawActiveBorder(int thickness) {
  if (playing) {
    DrawLineEx((Vector2){0, 0}, (Vector2){WIDTH * SIZE, 0}, thickness, GREEN);
    DrawLineEx((Vector2){WIDTH * SIZE, 0},
               (Vector2){WIDTH * SIZE, HEIGHT * SIZE}, thickness, GREEN);
    DrawLineEx((Vector2){WIDTH * SIZE, HEIGHT * SIZE},
               (Vector2){0, HEIGHT * SIZE}, thickness, GREEN);
    DrawLineEx((Vector2){0, 0}, (Vector2){0, HEIGHT * SIZE}, thickness, GREEN);
  }
}

void highlightHoverCell(Vector2 mpos) {
  DrawRectangle((int)(mpos.x / SIZE) * SIZE, (int)(mpos.y / SIZE) * SIZE, SIZE,
                SIZE, GRAY);
}

int main() {
  InitWindow(WIDTH * SIZE, HEIGHT * SIZE, "Conway's Game of Life");
  SetTargetFPS(15);
  SetExitKey(KEY_Q);

  while (!WindowShouldClose()) {
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !playing) {
      cellCopy((int)(mousePos.y / SIZE), (int)(mousePos.x / SIZE)) =
          !cellCopy((int)(mousePos.y / SIZE), (int)(mousePos.x / SIZE));
      updateGrid();
    }

    if (IsKeyPressed(KEY_SPACE)) {
      playing = !playing;
    }

    if (playing) {
      updateState();
    }

    BeginDrawing();
    ClearBackground(BLACK);
    highlightHoverCell(mousePos);
    drawActiveBorder(4);
    drawGrid();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
