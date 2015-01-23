// Ethernet
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// Define the LED digit patters, from 0 - 9
// Note that these patterns are for common cathode displays
// For common anode displays, change the 1's to 0's and 0's to 1's
// 1 = LED on, 0 = LED off, in this order:
//                                    Arduino pin: 2,3,4,5,6,7,8
byte seven_seg_digits[10][7] = { { 1,1,1,1,1,1,0 },  // = 0
                                 { 0,1,1,0,0,0,0 },  // = 1
                                 { 1,1,0,1,1,0,1 },  // = 2
                                 { 1,1,1,1,0,0,1 },  // = 3
                                 { 0,1,1,0,0,1,1 },  // = 4
                                 { 1,0,1,1,0,1,1 },  // = 5
                                 { 1,0,1,1,1,1,1 },  // = 6
                                 { 1,1,1,0,0,0,0 },  // = 7
                                 { 1,1,1,1,1,1,1 },  // = 8
                                 { 1,1,1,0,0,1,1 }   // = 9
                                 };

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain



// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// assign an IP address for the controller:
IPAddress ip(10,192,92,199);
IPAddress gateway(10,192,92,129);	
IPAddress subnet(255, 255, 255, 128);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Stuff for Humiture sensor
#include "DHT.h"
#define DHTPIN 40     // what pin we're connected to
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h = 0.0;
float t = 0.0;

// Time (NTP Server)  http://arduino.cc/en/Tutorial/UdpNtpClient
// UTC - 7 is Tucson time
unsigned int localPort = 8888;      // local port to listen for UDP packets
IPAddress timeServer(150,135,15,165); // ntp1.sista NTP server
const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 
                               // bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

String time = "";

void setup() {                
  Serial.begin(9600); 
  dht.begin();
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  server.begin();
  // Tens
  pinMode(22, OUTPUT);   
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  writeDot(0);  // start with the "dot" off
  // Ones
  pinMode(30, OUTPUT);   
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  writeDot(0);  // start with the "dot" off

  // give the sensor and Ethernet shield time to set up:
  delay(1000);
}

void writeDot(byte dot) {
  digitalWrite(29, dot);
  digitalWrite(38, dot);
}
    
void sevenSegWriteTens(byte digit) {
  byte pin = 22;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void sevenSegWriteOnes(byte digit) {
  byte pin = 30;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void listenForEthernetClients(int h, int t, String time) {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Got a client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          // print the current readings, in HTML format:
          client.print("<h1>GS913 Machine Room</h1>");
          client.print("<p><strong>Temperature:");
          client.print(t);
          client.print(" degrees F</strong><br/></p>");
          client.println("<p><strong>Humidity: ");
          client.print(h);
          client.print("%</strong><br/></p>");
          client.print("<br/>");
        client.print("Last updated: ");
      client.print(time);  
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
} 




void loop() {
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  time = "";
    // wait to see if a reply is available
  delay(1000);  
  if ( Udp.parsePacket() ) {  
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);               

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;     
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;  
    // print Unix time:
    Serial.println(epoch);                               

    // Begin of string + hour
    time = time + String((epoch  % 86400L) / 3600);

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
  time = time + ':';  
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
      time = time + '0';
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
   time = time + String((epoch  % 3600) / 60);
  time = time + ':'; 
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
      time = time + '0';
    }
    Serial.println(epoch %60); // print the second
    time = time + String(epoch %60);
    time = time + " UTC time."; 
  } 
  
  // Grab Tempurature and Humidity
  float h = dht.readHumidity();
   float t = dht.readTemperature();
   t = t*9/5+32;

  // Display to 7-segment
  int ten = t/10;
   int one = int(t)%10;
   sevenSegWriteTens(ten);
   sevenSegWriteOnes(one);

   // check if returns are valid, if they are NaN (not a number) then something went wrong!
   if (isnan(t) || isnan(h)) {
     Serial.println("Failed to read from DHT");
    } else {
     Serial.print("Humidity: "); 
     Serial.print(h);
     Serial.print(" %\t");
     Serial.print("Temperature: "); 
     Serial.print(t);
     Serial.println(" *C");
   }
   // listen for incoming Ethernet connections:
   listenForEthernetClients(h,t, time);
   delay(1000);
}

// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp: 		   
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

