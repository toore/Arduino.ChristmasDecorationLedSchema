unsigned long startMillis;
unsigned long currentMillis;
const unsigned long Interval = 30;
const float T = 1000;
const float Pi = 3.1415f;
const float TwoPi = 2*Pi;
const float e = 2.71828;

const int NumberOfEntities = 3;
const int EntityLedPin[NumberOfEntities] = { 9, 7, 5};
const int NumberOfSchemas[NumberOfEntities] = { 2, 2, 2};
int currentSchemaIndex[NumberOfEntities];
float x[NumberOfEntities];

enum LightFunction {
  Dark, 
  Light, 
  Wave, 
  StartingFire, 
  IntenseFire, 
  ThinBell
};

struct Schema {
  LightFunction Light;
  float Period;
};

const static Schema Schemas[][NumberOfEntities] = 
{
  {{ThinBell, T*2}, {Dark, T}},
  {{Dark, T},  {ThinBell, T*2}}, 
  {{StartingFire, T*10}, {IntenseFire, T*10}}
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
      int schemaIndex = currentSchemaIndex[i];
      const Schema* schema = &(Schemas[i][schemaIndex]);

      int brightness = (int)(Calculate(schema->Light, x[i]) * 255);
      analogWrite(EntityLedPin[i], brightness);

      x[i] += 1 / schema->Period * Interval;
      if(x[i] >= 1.0)
      {
        x[i] = 0.0f;
        currentSchemaIndex[i]++;
        if(currentSchemaIndex[i] >= NumberOfSchemas[i])
        {
          currentSchemaIndex[i] = 0;
        }
      }
    }

    startMillis = currentMillis;
  }
}

// Calculates a value between 0 and 1 (exclusive).
// x value is a value between 0 and 1.
float Calculate(LightFunction light, float x)
{
  switch (light)
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
    case ThinBell:
      return pow(e, -100*pow(x-0.5, 2));
  }
  
  return 0;
}