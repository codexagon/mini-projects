#include <raylib.h>

#define WIDTH 90
#define HEIGHT 70
#define SIZE 10

typedef enum CellType {
  EMPTY,
  ELECTRON_HEAD,
  ELECTRON_TAIL,
  CONDUCTOR
} CellType;

CellType grid[WIDTH * HEIGHT] = {EMPTY};
CellType copy[WIDTH * HEIGHT] = {EMPTY};

bool playing = false;

#define cell(row, col) grid[row * WIDTH + col]
#define cellCopy(row, col) copy[row * WIDTH + col]

void setInitialState() {
	cell(2, 2) = ELECTRON_TAIL;
	cell(2, 3) = ELECTRON_HEAD;
	cell(2, 4) = CONDUCTOR;
	cell(2, 5) = CONDUCTOR;
	cell(2, 6) = CONDUCTOR;
	cell(2, 7) = CONDUCTOR;
	cell(2, 8) = CONDUCTOR;
	cell(2, 9) = CONDUCTOR;
	cell(2, 10) = CONDUCTOR;
	cell(2, 11) = CONDUCTOR;
	cell(2, 12) = CONDUCTOR;
	cell(2, 13) = CONDUCTOR;
	cell(2, 14) = CONDUCTOR;
	cell(2, 15) = CONDUCTOR;

	cell(3, 1) = CONDUCTOR;
	cell(3, 16) = CONDUCTOR;

	cell(4, 2) = CONDUCTOR;
	cell(4, 3) = CONDUCTOR;
	cell(4, 4) = CONDUCTOR;
	cell(4, 5) = CONDUCTOR;
	cell(4, 6) = CONDUCTOR;
	cell(4, 7) = CONDUCTOR;
	cell(4, 8) = CONDUCTOR;
	cell(4, 9) = CONDUCTOR;
	cell(4, 10) = CONDUCTOR;
	cell(4, 11) = CONDUCTOR;
	cell(4, 12) = CONDUCTOR;
	cell(4, 13) = CONDUCTOR;
	cell(4, 14) = CONDUCTOR;
	cell(4, 15) = CONDUCTOR;
}

int getCellCount(int y, int x, CellType cell) {
  int count = 0;
  for (int r = y - 1; r <= y + 1; r++) {
    for (int c = x - 1; c <= x + 1; c++) {
      if (r == y && c == x) {
        continue;
      }
      if (cell(r, c) == cell) {
        count++;
      }
    }
  }

  return count;
}

void updateGrid() {
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    grid[i] = copy[i];
  }
}

void updateState() {
  for (int r = 1; r < HEIGHT - 1; r++) {
    for (int c = 1; c < WIDTH - 1; c++) {
      int electronHeadCount = getCellCount(r, c, ELECTRON_HEAD);
      if (cell(r, c) == ELECTRON_HEAD) {
        cellCopy(r, c) = ELECTRON_TAIL;
      } else if (cell(r, c) == ELECTRON_TAIL) {
        cellCopy(r, c) = CONDUCTOR;
      } else if (cell(r, c) == CONDUCTOR) {
        if (electronHeadCount == 1 || electronHeadCount == 2) {
          cellCopy(r, c) = ELECTRON_HEAD;
        } else {
					cellCopy(r, c) = CONDUCTOR;
				}
      }
    }
  }
  updateGrid();
}

void drawGrid() {
  Color color;
  for (int r = 1; r < HEIGHT - 1; r++) {
    for (int c = 1; c < WIDTH - 1; c++) {
      switch (cell(r, c)) {
      case EMPTY:
        color = BLACK;
        break;
      case ELECTRON_HEAD:
        color = BLUE;
        break;
      case ELECTRON_TAIL:
        color = RED;
        break;
      case CONDUCTOR:
        color = YELLOW;
        break;
      }
      DrawRectangle(c * SIZE, r * SIZE, SIZE, SIZE, color);
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

int main() {
  InitWindow(WIDTH * SIZE, HEIGHT * SIZE, "Wireworld");
  SetTargetFPS(15);
  SetExitKey(KEY_Q);
	
	setInitialState();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_SPACE)) {
      playing = !playing;
    }

    if (playing) {
      updateState();
    }

    BeginDrawing();
    ClearBackground(BLACK);
    drawActiveBorder(4);
    drawGrid();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
