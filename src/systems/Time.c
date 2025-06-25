#include "Time.h"
#include <math.h>
#include <stdio.h>

void initTimeSystem(TimeSystem *ts, float speed) {
	ts->time = 6.0f; // start at 6 AM
	ts->day = 1;
	ts->month = 1;
	ts->year = 1;
	ts->speed = speed;
	ts->state = getTimeState(ts);
}

void updateTimeSystem(TimeSystem *ts, float deltaTime) {
	ts->time += ts->speed * deltaTime;
	if (ts->time >= 24.0f) {
		ts->time -= 24.0f;
		ts->day++;

		// 30 days per month
		if (ts->day > 30) {
			ts->day = 1;
			ts->month++;
		}

		// 12 months per year
		if (ts->month > 12) {
			ts->month = 1;
			ts->year++;
		}
	}

	ts->state = getTimeState(ts);
}

TimeState getTimeState(const TimeSystem *ts) {
	float t = ts->time;

	if (t >= 5.0f && t < 9.0f) return TIMESTATE_MORNING;
	if (t >= 9.0f && t < 17.0f) return TIMESTATE_DAY;
	if (t >= 17.0f && t < 21.0f) return TIMESTATE_EVENING;
	return TIMESTATE_NIGHT;
}

float getLightLevel(const TimeSystem *ts) {
	float t = ts->time;

	if (t >= 6.0f && t <= 18.0f) {
		// Daylight: peak at 12.0
		return 1.0f - fabsf(t - 12.0f) / 6.0f;
	} else {
		// Night
		return 0.1f;
	}
}

const char *getTimeStateName(TimeState state) {
	switch (state) {
		case TIMESTATE_NIGHT: return "Night";
		case TIMESTATE_MORNING: return "Morning";
		case TIMESTATE_DAY: return "Day";
		case TIMESTATE_EVENING: return "Evening";
		default: return "Unknown";
	}
}

void formatTime(const TimeSystem *ts, char *out, int maxLen) {
	int hour = (int)ts->time;
	int minute = (int)((ts->time - hour) * 60);
	snprintf(out, maxLen, "%02d:%02d", hour, minute);
}