#ifndef TIME_H
#define TIME_H

typedef enum
{
  TIMESTATE_NIGHT,
  TIMESTATE_MORNING,
  TIMESTATE_DAY,
  TIMESTATE_EVENING
} TimeState;

typedef struct
{
  float time;
  int day;
  int month;
  int year;
  float speed;
  TimeState state;
} TimeSystem;

void initTimeSystem(TimeSystem* ts, float speed);
void updateTimeSystem(TimeSystem* ts, float deltaTime);
TimeState getTimeState(const TimeSystem* ts);
const char* getTimeStateName(TimeState state);
void formatTime(const TimeSystem* ts, char* out, int maxLen);

#endif
