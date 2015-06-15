#include "WS2801/WS2801.h"
#include "application.h"


// -----------------------------------
// cafacsculpture
// -----------------------------------

// curl command to talk to it:
// curl https://api.particle.io/v1/devices/<secret>/color
// -d access_token=<secret> 
// -d "[128,128,000]"

const int debug = 0;  //  0 - off, 1 - led, 2 - text 

const int led1 = D7;
const int numPixel = 24;
const int randomLimit = 18;
const int timeLoop = 50; // time for loop in ms
const int newOdds = 30; // % odds that new LED will be lit per loop
const int numSMS = 5; // number of LEDs to color when SMS received

// create strip data structure
Adafruit_WS2801 strip = Adafruit_WS2801(numPixel);
int commandproc(String command);

// data structures for color and state

short red [numPixel];
short blue [numPixel];
short green [numPixel];
short state [numPixel];
short intensity [ ] = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 256, 256, 256, 256, 256, 256, 256, 256, 240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20};
int numIntensity;

// ff colors
const int rff = 150;
const int gff = 255;
const int bff = 10;

int numOn = 0; // number of LEDs on

// for received SMS
int smsRed, smsGreen, smsBlue;
bool smsReceived = false; 

// This routine runs only once upon reset
void setup()
{
    //Register our Spark function here
    Spark.function("color", commandproc);

    // Configure the blue LED
    pinMode(led1, OUTPUT);
    digitalWrite(led1, LOW);
   
    //initialize the strip
    strip.begin();
    for (int i=0; i< numPixel; i++) {
        strip.setPixelColor(i, 100, 0, 0);
    }
    
    strip.show();
    
    for (int i=0; i< numPixel; i++) {
        state[i] = -1;
        red[i] = rff;
        green[i] = gff;
        blue[i] = bff;
    }
    
    numIntensity = sizeof(intensity)/sizeof(short);

    
    //setup debug
    Serial.begin(9600);
   
    Serial.println("cafacsculpture starting");
}


// This routine loops forever
void loop()
{
    
    int r, rr, gg, bb;
    
    // loop through states if > 0, lookup intensity, from array, increase state
    for (int i=0; i<numPixel; i++) {
        if (state[i] >= 0) {
            if (debug == 1) Serial.print("Adjusting ");
            if (debug == 1) Serial.print(i);
            if (state[i] >= numIntensity) {
                state[i] = -1;
                strip.setPixelColor(i, 0, 0, 0);
                numOn--;
                if (debug == 1) Serial.println("Turning off");
            } else {
                rr = red[i] * intensity[state[i]] / 256;
                gg = green[i] * intensity[state[i]] / 256;
                bb = blue[i] * intensity[state[i]] / 256;
                
                strip.setPixelColor(i, rr, gg, bb);
                
                if (debug == 1) {
                    Serial.print("rr, gg, bb, intensity, state=");
                    Serial.print(rr); Serial.print(" ");
                    Serial.print(gg); Serial.print(" ");
                    Serial.print(bb); Serial.print(" ");
                    Serial.print(intensity[state[i]]); Serial.print(" ");
                    Serial.println(state[i]);
                }

                state[i] += 1;
            }
        }
    }
    strip.show();
    
    if (random(100) < newOdds) {
        if (debug == 1) Serial.print("Creating numOn=");
        if (debug == 1) Serial.print(numOn);
        r = pickLED();
        if (state[r] == -1) numOn++; // covers forced case when already on
        state[r] = 0;
        red[r] = rff;
        green[r] = gff;
        blue[r] = bff;
    }
    if (smsReceived) {
        for (int j = 0; j < numSMS; j++) {
            r = pickLED();
            if (state[r] == -1) numOn++; // covers forced case when already on
            state[r] = 0;
            red[r] = smsRed;
            green[r] = smsGreen;
            blue[r] = smsBlue;
            smsReceived = false;
            if (debug == 1) {
                Serial.print("SMS LED ");
                Serial.println(r);
            }
        }
    }
    delay(timeLoop);
}

int pickLED() {
    int r;
    if (numOn > randomLimit) {
        r = random(numPixel);
        if (debug == 1) Serial.print(" forced ");
    } else {
        do {
            r = random(numPixel);
            } while (state[r] >= 0);
    }
    if (debug == 1) Serial.print(" r= ");
    if (debug == 1) Serial.println(r);
    return r;
}

int commandproc(String command)
{
    if (debug == 2) {
        Serial.print("Command received=");
        Serial.println(command);
    }
    if (command == "on")
        digitalWrite(led1, 1);
    else if (command == "off")
        digitalWrite(led1, 0);
    else if (command.length() == 13) {
        smsRed = command.substring(1,4).toInt();
        smsGreen = command.substring(5,8).toInt();
        smsBlue = command.substring(9,12).toInt();
        if (smsRed >= 0 && smsRed <= 255 && smsGreen >= 0 && smsGreen <= 255 && smsBlue >= 0 && smsBlue <= 255) {
            if (debug == 2) Serial.println("valid color");
            smsReceived = true;
        }
    }
    return 1;
}
