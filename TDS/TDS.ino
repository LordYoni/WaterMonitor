#define TdsSensorPin 32 // Analog pin on ESP32
#define VREF 5          // ADC reference voltage on ESP32 (5V)
#define NUM_SAMPLES 30  // Number of sampling points

int analogBuffer[NUM_SAMPLES]; // Array to store the read analog values
int tempBuffer[NUM_SAMPLES];   // Temporary array for sorted values
int bufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;

void setup()
{
    Serial.begin(115200);
    pinMode(TdsSensorPin, INPUT);
}

void loop()
{
    static unsigned long samplingTime = millis();

    if (millis() - samplingTime > 40U)
    {
        samplingTime = millis();
        analogBuffer[bufferIndex] = analogRead(TdsSensorPin);
        bufferIndex++;

        if (bufferIndex == NUM_SAMPLES)
        {
            bufferIndex = 0;
        }
    }

    static unsigned long displayTime = millis();

    if (millis() - displayTime > 800U)
    {
        displayTime = millis();

        for (copyIndex = 0; copyIndex < NUM_SAMPLES; copyIndex++)
        {
            tempBuffer[copyIndex] = analogBuffer[copyIndex];
        }

        averageVoltage = getMedianValue(tempBuffer, NUM_SAMPLES) * (float)VREF / 4096.0;
        float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
        float compensatedVoltage = averageVoltage / compensationCoefficient;
        tdsValue = (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage -
                    255.86 * compensatedVoltage * compensatedVoltage + 857.39 * compensatedVoltage) *
                   0.5;
        Serial.print("TDS Value: ");
        Serial.print(tdsValue);
        Serial.println(" ppm");
    }
}

// Function to calculate the median value from integers
int getMedianValue(int array[], int filterLength)
{
    int tempArray[filterLength];

    for (byte i = 0; i < filterLength; i++)
    {
        tempArray[i] = array[i];
    }

    int i, j, temp;
    for (j = 0; j < filterLength - 1; j++)
    {
        for (i = 0; i < filterLength - j - 1; i++)
        {
            if (tempArray[i] > tempArray[i + 1])
            {
                temp = tempArray[i];
                tempArray[i] = tempArray[i + 1];
                tempArray[i + 1] = temp;
            }
        }
    }

    if ((filterLength & 1) > 0)
    {
        temp = tempArray[(filterLength - 1) / 2];
    }
    else
    {
        temp = (tempArray[filterLength / 2] + tempArray[filterLength / 2 - 1]) / 2;
    }

    return temp;
}