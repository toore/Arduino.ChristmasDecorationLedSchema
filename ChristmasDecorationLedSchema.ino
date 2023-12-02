unsigned long startMillis;
unsigned long currentMillis;
const unsigned long UpdateInterval = 30;
const int T = 1000;
const int T2 = 2*T;
const int T3 = 3*T;
const int T4 = 4*T;
const int T5 = 5*T;
const int T6 = 6*T;
const int T7 = 7*T;
const int T10 = 10*T;
const int T15 = 15*T;
const int T20 = 20*T;
const float Pi = 3.1415f;
const float TwoPi = 2*Pi;
const float e = 2.71828;

const int NumberOfEntities = 3;
const int EntityLedPin[NumberOfEntities] = {9, 7, 5};
const int NumberOfSchemas[NumberOfEntities] = {10, 10, 10};
int currentSchemaIndex[NumberOfEntities];
int t[NumberOfEntities];

enum LightFunction {
  Dark, 
  Light, 
  Wave, 
  StartingFire, 
  IntenseFire, 
  Bell
};

struct Schema {
  LightFunction Light;
  int Period;
};

const static Schema Schemas[NumberOfEntities][10] = 
{
  {{Bell, T5}, {Dark, T7},              {Dark, T10},         {Dark, T5}, {Wave, T10}, {Wave, T10}, {Wave, T10}, {Wave, T10}, {Wave, T10}, {Dark, T5}},
  {{Dark, T},  {Bell, T5}, {Dark, T6},  {Dark, T10},         {Wave, T10}, {Wave, T10}, {Wave, T10}, {Wave, T10}, {Wave, T10}, {Wave, T10}}, 
  {{Dark, T2}, {Bell, T5}, {Light, T5}, {StartingFire, T10}, {IntenseFire, T10}, {IntenseFire, T10}, {IntenseFire, T10}, {IntenseFire, T10}, {IntenseFire, T10}, {IntenseFire, T10}}                            
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
  if (currentMillis - startMillis >= UpdateInterval)
  {
    for(int i = 0; i < NumberOfEntities; i++)
    {
      int schemaIndex = currentSchemaIndex[i];
      const Schema* schema = &(Schemas[i][schemaIndex]);

      float independentValue = (float)t[i] / schema->Period;
      int brightness = (int)(Calculate(schema->Light, independentValue) * 255);
      analogWrite(EntityLedPin[i], brightness);

      t[i] += UpdateInterval;
      if(t[i] >= schema->Period)
      {
        t[i] %= schema->Period;
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
    case Bell:
      return pow(e, -100*pow(x-0.5, 2));
  }
  
  return 0;
}