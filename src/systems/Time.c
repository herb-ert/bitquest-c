#include "Time.h"
#include <math.h>
#include <stdio.h>

void initTimeSystem(TimeSystem* ts, float speed)
{
  ts->time = 6.0f; // start at 6 AM
  ts->day = 1;
  ts->month = 1;
  ts->year = 1;
  ts->speed = speed;
  ts->state = getTimeState(ts);
}

void updateTimeSystem(TimeSystem* ts, float deltaTime)
{
  ts->time += ts->speed * deltaTime;
  if (ts->time >= 24.0f)
  {
    ts->time -= 24.0f;
    ts->day++;

    if (ts->day > 28)
    {
      ts->day = 1;
      ts->month++;
    }

    if (ts->month > 12)
    {
      ts->month = 1;
      ts->year++;
    }
  }

  ts->state = getTimeState(ts);
}

TimeState getTimeState(const TimeSystem* ts)
{
  float t = ts->time;

  if (t >= 5.0f && t < 9.0f) return TIMESTATE_MORNING;
  if (t >= 9.0f && t < 17.0f) return TIMESTATE_DAY;
  if (t >= 17.0f && t < 21.0f) return TIMESTATE_EVENING;
  return TIMESTATE_NIGHT;
}

float getLightLevel(const TimeSystem* ts)
{
  float t = ts->time;

  if (t >= 6.0f && t <= 18.0f)
  {
    // Daylight: peak at 12.0
    return 1.0f - fabsf(t - 12.0f) / 6.0f;
  }
  else
  {
    // Night
    return 0.1f;
  }
}

const char* getTimeStateName(TimeState state)
{
  switch (state)
  {
  case TIMESTATE_NIGHT: return "Night";
  case TIMESTATE_MORNING: return "Morning";
  case TIMESTATE_DAY: return "Day";
  case TIMESTATE_EVENING: return "Evening";
  default: return "Unknown";
  }
}

const char* getDayName(const TimeSystem* ts)
{
  switch ((ts->day + 6) % 7)
  {
  case 0: return "Monday";
  case 1: return "Tuesday";
  case 2: return "Wednesday";
  case 3: return "Thursday";
  case 4: return "Friday";
  case 5: return "Saturday";
  case 6: return "Sunday";
  default: return "Unknown";
  }
}

const char* getMonthName(TimeSystem* ts)
{
  switch (ts->month)
  {
  case 1: return "January";
  case 2: return "February";
  case 3: return "March";
  case 4: return "April";
  case 5: return "May";
  case 6: return "June";
  case 7: return "July";
  case 8: return "August";
  case 9: return "September";
  case 10: return "October";
  case 11: return "November";
  case 12: return "December";
  default: return "Unknown";
  }
}

void formatTime(const TimeSystem* ts, char* out, int maxLen)
{
  int hour = (int)ts->time;
  int minute = (int)((ts->time - hour) * 60);
  snprintf(out, maxLen, "%02d:%02d", hour, minute);
}
