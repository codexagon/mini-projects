#include <raylib.h>

#define WIDTH 90
#define HEIGHT 70
#define SIZE 10

int grid[WIDTH * HEIGHT] = {0};
int copy[WIDTH * HEIGHT] = {0};

#define cell(row, col) grid[row * WIDTH + col]
#define cellCopy(row, col) copy[row * WIDTH + col]

void updateGrid() {
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    grid[i] = copy[i];
  }
}

void setGlider() {
	cellCopy(1, 2) = 1;
	cellCopy(2, 3) = 1;
	cellCopy(3, 3) = 1;
	cellCopy(3, 2) = 1;
	cellCopy(3, 1) = 1;
}

void setInitialState() {
	setGlider();
  updateGrid();
}

int getAliveCount(int y, int x) {
  int alive = 0;
  for (int r = y - 1; r <= y + 1; r++) {
    for (int c = x - 1; c <= x + 1; c++) {
      if (r == y && c == x) {
        continue;
      }
      if (cell(r, c) == 1) {
        alive++;
      }
    }
  }

  return alive;
}

void updateState() {
  for (int r = 1; r < HEIGHT - 1; r++) {
    for (int c = 1; c < WIDTH - 1; c++) {
      int alive = getAliveCount(r, c);
      if (cell(r, c) == 1) {
        if (alive < 2 || alive > 3) {
          cellCopy(r, c) = 0;
          continue;
        }
      } else if (cell(r, c) == 0) {
        if (alive == 3) {
          cellCopy(r, c) = 1;
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
      if (cell(r, c) == 1) {
        DrawRectangle(c * SIZE, r * SIZE, SIZE, SIZE, WHITE);
      } else {
        DrawRectangle(c * SIZE, r * SIZE, SIZE, SIZE, BLACK);
      }
    }
  }
}

int main() {
  InitWindow(WIDTH * SIZE, HEIGHT * SIZE, "Conway's Game of Life");
  SetTargetFPS(4);
  SetExitKey(KEY_Q);
  setInitialState();

  while (!WindowShouldClose()) {
    updateState();
    ClearBackground(BLACK);
    BeginDrawing();
    drawGrid();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
