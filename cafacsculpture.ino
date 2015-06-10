
// -----------------------------------
// cafacsculpture
// -----------------------------------

// curl command to talk to it:
// curl https://api.particle.io/v1/devices/<secret>/color
// -d access_token=<secret> 
// -d "[128,128,000]" or -d"on"

// name the pins
int led1 = D7;

// This routine runs only once upon reset
void setup()
{
   //Register our Spark function here
   Spark.function("color", commandproc);

   // Configure the pins to be outputs
   pinMode(led1, OUTPUT);

   // Initialize both the LEDs to be OFF
   digitalWrite(led1, LOW);

   Serial.begin(9600);
   
   Serial.println("cafacsculpture starting");
}


// This routine loops forever
void loop()
{
   // Nothing to do here
}


int commandproc(String command)
{
    Serial.println(command);
    if (command == "on")
        digitalWrite(led1, 1);
    if (command == "off")
        digitalWrite(led1, 0);
    return 1;
   //find out the pin number and convert the ascii to integer
   // int pinNumber = (command.charAt(1) - '0') - 1;
   //Sanity check to see if the pin numbers are within limits
   //if(command.substring(3,7) == "HIGH") state = 1;
   //else if(command.substring(3,6) == "LOW") state = 0;
}
