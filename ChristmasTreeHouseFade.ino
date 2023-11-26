unsigned long startMillis;
unsigned long currentMillis;
const unsigned long Interval = 30;
const float T = 1000;
const float Pi = 3.1415f;
const float TwoPi = 2*Pi;

const int NumberOfEntities = 3;
const int EntityLedPin[NumberOfEntities] = { 9, 7, 5};
const int NumberOfSchemes[NumberOfEntities] = { 2, 2, 2};
int currentSchemeIndex[NumberOfEntities];
float x[NumberOfEntities];

typedef enum {Dark, Light, Wave, StartingFire, IntenseFire} CurveType;

const CurveType SchemeLightFunctions[][NumberOfEntities] = 
{
  {Wave, Dark},
  {Dark, Wave}, 
  {StartingFire, IntenseFire}
};

const float SchemePeriods[][NumberOfEntities] = 
{
  {T*2, T}, 
  {T, T*2}, 
  {T*10, T*10}
};

void setup() 
{
  Serial.begin(115200);
  for(int i = 0; i < NumberOfEntities; i++)
  {
    pinMode(EntityLedPin[i], OUTPUT);
  }
  startMillis = millis();
}

void loop() 
{
  currentMillis = millis();
  if (currentMillis - startMillis >= Interval)
  {
    for(int i = 0; i < NumberOfEntities; i++)
    {
      int schemeIndex = currentSchemeIndex[i];
      int brightness = (int)(Calculate(SchemeLightFunctions[i][schemeIndex], x[i]) * 255);
      // if(i==2)
      //   Serial.println(brightness);
      analogWrite(EntityLedPin[i], brightness);

      x[i] += 1 / SchemePeriods[i][schemeIndex] * Interval;
      if(x[i] >= 1.0)
      {
        x[i] = 0.0f;
        currentSchemeIndex[i]++;
        if(currentSchemeIndex[i] >= NumberOfSchemes[i])
        {
          currentSchemeIndex[i] = 0;
        }
      }
    }

    startMillis = currentMillis;
  }
}

// Calculates a value between 0 and 1 (exclusive).
// x value is a value between 0 and 1.
float Calculate(CurveType curve, float x)
{
  switch (curve)
  {
    case Dark:
      return 0.0;
    case Light:
      return 1.0;
    case Wave:
      return sin(x*TwoPi - Pi*.5) * .5 + .5;
    case StartingFire:
      return .3 + 1/16.0 * cos(8*Pi*x) - 1/8.0 * cos(16*Pi*x) + 1/8.0 * cos(32*Pi*x) - 1/8.0 * cos(64*Pi*x);
    case IntenseFire:
      return .7 + 1/4.0 * cos(8*Pi*x) - 1/8.0 * cos(16*Pi*x) + 1/16.0 * cos(32*Pi*x) - 1/32.0 * cos(64*Pi*x);
  }
  
  return 0;
}