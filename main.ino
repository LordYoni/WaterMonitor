// Déclaration des broches des capteurs
const int pinOxygen = A0;          // Capteur d'oxygène dissous 
const int pinPH = A1;              // Capteur de pH 
const int pinConductivity = A2;    // Capteur de conductivité 
const int pinPressure = A3;        // Capteur de pression 
const int pinTurbidity = A4;       // Capteur de turbidité 
const int pinLevelNF = A5;         // Capteur de niveau NF

// Variables de conversion
float Vout, oxygenValue, pHValue, conductivityValue, pressureValue, turbidityValue, levelNFValue;

// Constantes pour chaque capteur
const float oxygenCalibrationFactor = 1.0;   // Facteur de calibration (à définir)
const float oxygenVmax = 3.3;                // Tension maximale pour l'oxygène dissous
const float oxygenVmin = 0.0;                // Tension minimale pour l'oxygène dissous
const float R1 = 1.0;                        // Résistance pour le capteur de conductivité
const float conductivityVin = 5.0;           // Tension d'alimentation pour le capteur de conductivité
const float pressureVoffset = 0.5;           // Offset pour le capteur de pression
const float pressureSensitivity = 0.02;      // Sensibilité du capteur de pression (à vérifier)

void setup() {
  Serial.begin(9600);
  // Initialisation des broches
  pinMode(pinOxygen, INPUT);
  pinMode(pinPH, INPUT);
  pinMode(pinConductivity, INPUT);
  pinMode(pinPressure, INPUT);
  pinMode(pinTurbidity, INPUT);
  pinMode(pinLevelNF, INPUT);
}

void loop() {
  // Lecture des capteurs
  readOxygen();
  readPH();
  readConductivity();
  readPressure();
  readTurbidity();
  readLevelNF();

  delay(1000);  
}

// Fonction pour lire le capteur d'oxygène dissous
void readOxygen() {
  Vout = analogRead(pinOxygen) * (3.3 / 1023.0); // Conversion de la tension (0-3.3V)
  oxygenValue = (Vout * 1000.0) / (oxygenVmax - oxygenVmin) * oxygenCalibrationFactor;
  Serial.print("Oxygène dissous (mg/L): ");
  Serial.println(oxygenValue);
}

// Fonction pour lire le capteur de pH
void readPH() {
  Vout = analogRead(pinPH) * (5.0 / 1023.0); // Conversion de la tension (0-5V)
  pHValue = 3.5 * Vout;
  Serial.print("pH: ");
  Serial.println(pHValue);
}

// Fonction pour lire le capteur de conductivité
void readConductivity() {
  Vout = analogRead(pinConductivity) * (3.3 / 1023.0); // Conversion de la tension (0-3.3V)
  conductivityValue = (Vout * 1000.0) / (R1 * conductivityVin - Vout);
  Serial.print("Conductivité (uS/cm): ");
  Serial.println(conductivityValue);
}

// Fonction pour lire le capteur de pression
void readPressure() {
  Vout = analogRead(pinPressure) * (5.0 / 1023.0); // Conversion de la tension (0-5V)
  pressureValue = (Vout - pressureVoffset) / pressureSensitivity;
  Serial.print("Pression (psi): ");
  Serial.println(pressureValue);
}

// Fonction pour lire le capteur de turbidité
void readTurbidity() {
  Vout = analogRead(pinTurbidity) * (4.5 / 1023.0); // Conversion de la tension (0-4.5V)
  turbidityValue = -1120.4 * pow(Vout, 2) + 5742.3 * Vout - 4352.9;
  Serial.print("Turbidité (NTU): ");
  Serial.println(turbidityValue);
}

// Fonction pour lire le capteur de niveau NF
void readLevelNF() {
  levelNFValue = analogRead(pinLevelNF) * (5.0 / 1023.0); // Conversion de la tension (0-5V)
  Serial.print("Niveau (V): ");
  Serial.println(levelNFValue);
}
