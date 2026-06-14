#include <raylib.h>

#include "pzltimer.h"

#define TIMER_POS (Vector2){480, 420}

Global g;
Font font;

void drawTimer(Timer *t, Vector2 pos, Color c) {
  int fontSize = 200;
  const char *text = TextFormat("%05.2f", t->seconds);

  DrawTextEx(font, text, pos, fontSize, 1, c);
}

void initGlobal(Global *g) {
  g->timer.seconds = 0;
  g->state = STATE_NORMAL;
  g->dt = 0;

  g->startHoldTimer.seconds = 0;
  g->thresholdStart = 0.55;
}

int main(void) {
  InitWindow(1400, 1000, "pzltimer");
  SetTargetFPS(100);
  SetExitKey(KEY_Q);

  initGlobal(&g);
  font = LoadFontEx(
      "/usr/share/fonts/TTF/JetBrainsMonoNerdFontMono-SemiBold.ttf", 200, 0, 0);

  while (!WindowShouldClose()) {
    g.dt = GetFrameTime();

    switch (g.state) {
    case STATE_NORMAL: {
      if (IsKeyPressed(KEY_SPACE)) {
        if (g.timer.seconds == 0) {
          g.state = STATE_TIMER_START;
        } else {
          g.timer.seconds = 0;
        }
      }
      break;
    }
    case STATE_TIMER_START: {
      if (IsKeyDown(KEY_SPACE)) {
        g.startHoldTimer.seconds += g.dt;
      } else {
        if (g.startHoldTimer.seconds >= g.thresholdStart) {
          g.state = STATE_TIMER_RUNNING;
        } else {
          g.state = STATE_NORMAL;
        }
        g.startHoldTimer.seconds = 0;
      }
      break;
    }
    case STATE_TIMER_RUNNING: {
      g.timer.seconds += g.dt;
      if (IsKeyPressed(KEY_SPACE)) {
        g.state = STATE_NORMAL;
      }
      break;
    }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    switch (g.state) {
    case STATE_NORMAL: {
      drawTimer(&g.timer, TIMER_POS, WHITE);
      break;
    }
    case STATE_TIMER_START: {
      if (g.startHoldTimer.seconds >= g.thresholdStart) {
        drawTimer(&g.timer, TIMER_POS, GREEN);
      } else {
        drawTimer(&g.timer, TIMER_POS, RED);
      }
      break;
    }
    case STATE_TIMER_RUNNING: {
      drawTimer(&g.timer, TIMER_POS, WHITE);
      break;
    }
    }

    EndDrawing();
  }

  UnloadFont(font);
  CloseWindow();
}
