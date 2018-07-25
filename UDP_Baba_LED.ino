  /* 87.3 is F
116.54 is Bb
174 is high F
130 is C
*/

#define hF 19
#define hC  18
#define Bb 17
#define fF 16
//#define dampener 5
#define mic 36
#define firstDig  14
#define secondDig 25
#define thirdDig  32
#include <WiFi.h>
#include <WiFiUdp.h>

#define strikeconstant   50

int counter = 1;
float prevState;
float currentState;

const int threshold = 180;
const int sampleTime = 5;

unsigned int PTPAmp = 0;
int sampling = 0;

unsigned int maxAmp = 0;
unsigned int minAmp = 4095;
unsigned long sampleStartTime, firstTime;

bool looper = false;

const char* ssid     = "modlab1";
const char* password = "ESAP2017";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2808;
IPAddress myIPaddress(192, 168, 1, 60);

const int packetSize = 10;

byte packetBuffer[packetSize];


const float playmoment[] = {0, 50.08694,51.65216,52.1739,54.26086,55.82607,56.34781,58.43477,59.99999,60.52172,62.60868,64.1739,64.69564,66.78259,68.34781,68.86955,70.9565,72.52172,73.04346,75.13042,76.69563,77.21737,79.30433,80.86955,81.39128,83.47824,85.04346,85.5652,87.65215,89.21737,89.73911,91.82606,93.39128,93.91302,95.99998,97.56519,98.08693,100.1739,101.7391,102.2608,104.3478,105.913,106.4348,108.5217,110.0869,110.6087,112.6956,114.2608,114.7826,116.8695,118.4348,118.9565,121.0434,123.1304,125.2174,146.0869,147.6521,148.1739,150.2608,151.826,152.3478,154.4347,156,156.5217,158.6087,160.1739,160.6956,162.7826,164.3478,164.8695,166.9565,168.5217,169.0434,171.1304,172.6956,173.2173,175.3043,176.8695,177.3913,179.4782,180.7826,181.0434,181.5652,183.6521,184.9565,185.2173,185.7391,187.826,189.3913,189.913,192,193.5652,194.0869,196.1739,197.7391,198.2608,200.3478,201.913,202.4347,204.5217,206.0032,207.4846,208.9661,210.4476,213.4106,216.3735,219.3365,222.2994,225.2624,228.2254,231.1883,234.1513,237.1142,240.0772,260.421,260.7489,261.0768,261.4046,261.7325,262.0604,262.3882,262.7161,263.044,263.3719,263.6997,264.0276,264.3555,264.6833,265.0112,265.3391,265.6669,265.9948,266.3227,266.6505,266.9784,267.3063,267.6341,267.962,268.2899,268.6177,268.9456,269.2735,269.6013,269.9292,270.2571,270.5849,270.9128,271.2407,271.5686,271.8964,272.2243,272.5522,272.88,273.2079,273.5358,273.8636,274.1915,274.5194,274.8472,275.1751,275.503,275.8308,276.1587,276.4866,276.8144,277.1423,277.4702,277.798,278.1259,278.4538,278.7816,279.1095,279.4374,279.7653,280.0931,280.421,280.7489,281.0767,281.4046,281.7325,282.0603,282.3882,282.7161,283.0439,283.3718,283.6997,284.0275,284.3554,284.6833,285.0111,285.339,285.6669,285.9947,286.3226,286.6505,286.9783,287.3062,287.6341,287.962,288.2898,288.6177,288.9456,289.2734,289.6013,289.9292,290.257,290.5849,290.9128,291.2406,291.5685,291.8964,292.2242,292.5521,292.88,293.2078,293.5357,293.8636,294.1914,294.5193,294.8472,295.175,295.5029,295.8308,296.1587,296.4865,296.8144,297.1423,297.4701,297.798,298.1259,298.4537,298.7813,299.0906,299.3998,299.7091,300.0184,300.3277,300.637,300.7916,300.9462,301.1009,301.2555,301.4102,301.5648,301.7194,301.8741,302.0287,302.1833,302.338,302.4926};
const float note[] = {0, 174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,130.5,130.5,130.5,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,130.5,116.54,174.5,174.5,174.5,174.5,174.5,174.5,174.5,174.5,174.5,130.5,116.54,87.5,130.5,116.54,87.5,130.5,116.54,87.5,130.5,116.54,130.5,130.5,116.54,116.54,130.5,116.54,87.5,38.89087,130.5,116.54,87.5,38.89087,130.5,130.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,87.5,174.5,87.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5,130.5};

void setup()  {
  pinMode(18, OUTPUT); 
  pinMode(19, OUTPUT); 
  pinMode(17, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(16, OUTPUT);
  //pinMode(36, INPUT);
  pinMode(27,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(14,OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");   Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 255, 0));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("WiFi connected");

  UDPTestServer.begin(UDPPort);
}
void playnote()
{
  if(counter<300)
  {
    int waittime = (playmoment[counter]*1000) - (playmoment[counter-1]*1000);
    //   Serial.println(float(note[counter]));
    Serial.print("waiting ");
    Serial.println(waittime - strikeconstant);
    delay(waittime - strikeconstant);
    Serial.println(note[counter]);
    if (note[counter]> 174&&note[counter]<175 ){  //playing high f
      //Serial.println("High F");
      digitalWrite(26, HIGH);
      digitalWrite( hF, HIGH);
      delay(strikeconstant);
      digitalWrite( hF, LOW);
      digitalWrite(26, LOW);
    }
    else if (note[counter]> 116&&note[counter]<117){  //playing Bb
      //Serial.println("Bb");
      digitalWrite(32, HIGH);
      digitalWrite( Bb, HIGH);
      delay(strikeconstant);
      digitalWrite( Bb, LOW);
      digitalWrite(32, LOW);
    }
    else if (note[counter]> 130&&note[counter]<131 ){  //playing high C
      //Serial.println("High C");
      
      digitalWrite(33, HIGH);
      digitalWrite( hC, HIGH);
      delay(strikeconstant);
      digitalWrite( hC, LOW);
      digitalWrite(33, LOW);
    } 
    else if (note[counter]> 87&&note[counter]<88 ){  //playing fF
      //Serial.println("fF"); 
      digitalWrite(27, HIGH);
      digitalWrite( fF, HIGH);
      delay(strikeconstant);
      digitalWrite( fF, LOW);
      digitalWrite(27, LOW);
    }
    counter ++;
    Serial.println(counter);
  }
}

void loop() {
  handleUDPServer();
  delay(1);
}

void handleUDPServer() {
  if (looper == true) {
    playnote();
  }
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, packetSize);
    String myData = "";
    for (int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    Serial.print(millis()); Serial.print(", ");
    Serial.println (myData);
    if (myData == "A") {
      looper = true;
      Serial.println();

    }
  }
}
