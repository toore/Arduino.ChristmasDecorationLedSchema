unsigned long startMillis;
unsigned long currentMillis;
const unsigned long Interval = 30;
const float period = 1000;
const float Pi = 3.1415f;
const float TwoPi = 2*Pi;
const float NominalStep = 1 / period * Interval;

const int NumberOfEntities = 3;
const int EntityLedPin[NumberOfEntities] = { 9, 7, 5};
const int NumberOfSchemes[NumberOfEntities] = { 2, 2, 1};
int currentSchemeIndex[NumberOfEntities];
float radians[NumberOfEntities];

typedef enum {Dark, Light, Wave, Fire} CurveType;

const CurveType SchemeLightFunctions[][NumberOfEntities] = 
{
  {Wave, Dark},
  {Dark, Wave}, 
  {Light}
};

const float SchemeSteps[][NumberOfEntities] = 
{
  {NominalStep*.1, NominalStep*.2}, 
  {NominalStep*.2, NominalStep*.1}, 
  {NominalStep}
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
      int brightness = (int)(Calculate(SchemeLightFunctions[i][schemeIndex], radians[i]) * 255);
      //Serial.println(brightness);
      analogWrite(EntityLedPin[i], brightness);

      radians[i] += SchemeSteps[i][schemeIndex];
      if(radians[i] >= 1.0)
      {
        radians[i] = 0.0f;
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
// Independent value is a value between 0 and 1.
float Calculate(CurveType curve, float independent)
{
  switch (curve)
  {
    case Dark:
      return 0.0;
    case Light:
      return 1.0;
    case Wave:
      return sin(independent*TwoPi - Pi*.5) * .5 + .5;
    case Fire:
      return sin(independent*TwoPi - Pi*.5) * .5 + .5;
  }
  
  return 0;
}