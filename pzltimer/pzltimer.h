#ifndef PZLTIMER_H
#define PZLTIMER_H

#include <raylib.h>

typedef struct Timer {
	float seconds;
} Timer;

void drawTimer(Timer *t, Vector2 pos, Color c);

typedef enum State {
  STATE_NORMAL,
  STATE_TIMER_START,
  STATE_TIMER_RUNNING
} State;

typedef struct Global {
	State state;
	Timer timer;
	float dt;

	struct {
		Timer startHoldTimer;
		float thresholdStart;
	};
} Global;

void initGlobal(Global *g);

#endif
