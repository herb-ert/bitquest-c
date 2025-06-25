#include <math.h>
#include <SDL.h>
#include <Time.h>

SDL_Color lerpColor(SDL_Color a, SDL_Color b, float t)
{
  SDL_Color result;
  result.r = (Uint8)(a.r + (b.r - a.r) * t);
  result.g = (Uint8)(a.g + (b.g - a.g) * t);
  result.b = (Uint8)(a.b + (b.b - a.b) * t);
  result.a = (Uint8)(a.a + (b.a - a.a) * t);
  return result;
}

SDL_Color getTimeTintColor(const TimeSystem* ts)
{
  float t = ts->time;

  // Define softer, dimmer versions
  SDL_Color night = {15, 20, 35, 180}; // Deep bluish charcoal (moonlit foggy night)
  SDL_Color sunrise = {80, 45, 30, 120}; // Burnt ochre and ash rose (cold sunrise)
  SDL_Color day = {210, 210, 200, 0}; // Pale gray daylight (overcast light)
  SDL_Color sunset = {70, 40, 55, 100}; // Plum smoke and moss rose (cool rural sunset)

  SDL_Color color;

  if (t >= 5.0f && t < 7.0f)
  {
    float f = (t - 5.0f) / 2.0f;
    color = lerpColor(night, sunrise, f);
  }
  else if (t >= 7.0f && t < 17.0f)
  {
    float f = (t - 7.0f) / 10.0f;
    color = lerpColor(sunrise, day, f);
  }
  else if (t >= 17.0f && t < 19.0f)
  {
    float f = (t - 17.0f) / 2.0f;
    color = lerpColor(day, sunset, f);
  }
  else if (t >= 19.0f && t < 24.0f)
  {
    float f = (t - 19.0f) / 5.0f;
    color = lerpColor(sunset, night, f);
  }
  else
  {
    // t in [0, 5)
    float f = t / 5.0f;
    color = lerpColor(night, night, f); // static night
  }

  return color;
}
