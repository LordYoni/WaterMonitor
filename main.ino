

#include "sensors/Sensor.cpp"
#include "sensors/OxygenSensor.cpp"
#include "sensors/PhSensor.cpp"
#include "sensors/ConductivitySensor.cpp"
#include "sensors/PressureSensor.cpp"
#include "sensors/TurbiditySensor.cpp"
#include "sensors/NFlevelSensor.cpp"


// Déclaration des broches des capteurs
const uint8_t pinOxygen         = A0; // Capteur d'oxygène dissous
const uint8_t pinPH             = A1; // Capteur de pH
const uint8_t pinConductivity   = A2; // Capteur de conductivité
const uint8_t pinPressure       = A3; // Capteur de pression
const uint8_t pinTurbidity      = A4; // Capteur de turbidité
const uint8_t pinLevelNF        = A5; // Capteur de niveau NF


void setup() { Serial.begin(9600); }

void loop()
{
    OxygenSensor        oxygen          (pinOxygen);
    PhSensor            ph              (pinPH);
    ConductivitySensor  conductivity    (pinConductivity);
    PressureSensor      pressure        (pinPressure);
    TurbiditySensor     turbidity       (pinTurbidity);
    NFlevelSensor       NFlevel         (pinLevelNF);

    // Affichage de la valeur des capteurs
    oxygen.printValue();
    ph.printValue();
    conductivity.printValue();
    pressure.printValue();
    turbidity.printValue();
    NFlevel.printValue();

    delay(1000);
}
