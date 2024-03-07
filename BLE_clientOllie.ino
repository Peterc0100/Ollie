/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("22bb746f-2ba0-7554-2d6f-726568705327");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("22bb746f-2ba1-7554-2d6f-726568705327");
static BLEUUID  sUUID("22bb746f-2bb0-7554-2d6f-726568705327");
static BLEUUID    poweUUID("22bb746f-2bb2-7554-2d6f-726568705327");
static BLEUUID    txUUID("22bb746f-2bb2-7554-2d6f-726568705327");
static BLEUUID    wakeUUID("22bb746f-2bbf-7554-2d6f-726568705327");
static BLEUUID    antiUUID("22bb746f-2bbd-7554-2d6f-726568705327");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLERemoteCharacteristic* pRemoteCharacteristicp;
static BLERemoteCharacteristic* pRemoteCharacteristict;
static BLERemoteCharacteristic* pRemoteCharacteristicw;
static BLERemoteCharacteristic* pRemoteCharacteristica;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.write(pData, length);
    Serial.println();
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.println("BLEClientcallbacks says robot is now connected");
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


     // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    BLERemoteService* pRemoteServicew = pClient->getService(sUUID);
    if (pRemoteServicew == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(sUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");

 // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristica = pRemoteServicew->getCharacteristic(antiUUID);
    if (pRemoteCharacteristica == nullptr) {
      Serial.print("Failed to find our anti  characteristic UUID: ");
      Serial.println(antiUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our anti characteristic");

     // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicp = pRemoteServicew->getCharacteristic(poweUUID);
    if (pRemoteCharacteristicp == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(poweUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our power characteristic");

     // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristict = pRemoteServicew->getCharacteristic(txUUID);
    if (pRemoteCharacteristict == nullptr) {
      Serial.print("Failed to find tx characteristic UUID: ");
      Serial.println(txUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found tx characteristic");

     // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicw = pRemoteServicew->getCharacteristic(wakeUUID);
    if (pRemoteCharacteristicw == nullptr) {
      Serial.print("Failed to find wake characteristic UUID: ");
      Serial.println(wakeUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found wake characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

     Serial.println("Setting anti");
     String str = "011i3";
     byte unicodeArray[str.length()];

// Convert the characters to Unicode code points and store them in the array
     for (int i = 0; i < str.length(); i++) {
    unicodeArray[i] = str.charAt(i);
     }

// Create a Uint8Array equivalent (byte array)
     byte result[str.length()];
     for (int i = 0; i < str.length(); i++) {
          result[i] = unicodeArray[i];
      }
     uint8_t commanda[] = {0x30, 0x31, 0x31, 0x69, 0x33};
     pRemoteCharacteristica->writeValue(result, sizeof(result));
    // pRemoteCharacteristict->writeValue('011i3', 5);
     delay(1000);
    Serial.println("Set anti");

   Serial.println("Setting tx");
    //memset(command, 0, sizeof(command));
    uint8_t commandp[] = {0x07};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristict->writeValue(commandp, sizeof(commandp));
    delay(1000);
    Serial.println("Set tx");

    Serial.println("Setting wake");
    //memset(command, 0, sizeof(command));
    uint8_t commandw[] = {0x01};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristicw->writeValue(commandw, sizeof(commandw));
    delay(1000);
    Serial.println("Set wake");


    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting ping");

/* to work out the check sum 
  // Define the byte sequence
  remove the 0xff,0xff and the last x before doing th echeck sum then and it to the string
  byte bytes[] = {0x02, 0x20, 0x00, 0x05, 0xFF, 0x00, 0x00, 0x00};
  int sum = 0;

  // Add up the values of the bytes
  for (int i = 0; i < sizeof(bytes); i++) {
    sum += bytes[i];
  }

  // Compute the modulo 256
  int checksum = 255 - (sum % 256) + 1;

  // Perform the bitwise complement
  byte complement = ~checksum;

  // Convert to hexadecimal and print
  Serial.print("Checksum: 0x");
  if (complement < 0x10) {
    Serial.print("0"); // Add leading zero if necessary
  }
  Serial.println(complement, HEX);
  
  delay(1000); // Wait for 1 second before repeating
}
*/


    uint8_t pingcommand[] = {0xff, 0xff, 0x00, 0x01, 0x37, 0x01, 0xc7};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(pingcommand, sizeof(pingcommand));
    delay(1000);
    Serial.println("Senyyt ping");
  
    Serial.println("Setting new led to green");
    uint8_t command[] = {0xff, 0xff, 0x02, 0x20, 0x37, 0x05, 0x00, 0xFF, 0xFF, 0x01 ,0xa2};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(command, sizeof(command));
    delay(3000);
    
    Serial.println("rolling");
  
    uint8_t command1[] = {0xFF, 0xFF, 0x02, 0x30, 0x37, 0x05, 0x80, 0x01, 0x0E, 0x01, 0x01};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(command1, sizeof(command1));
    delay(100);
   
      Serial.println("Stopping");
//    uint8_t command2[] = {0xff, 0xff, 0x02, 0x20, 0x37, 0x05, 0x00, 0x00, 0x00, 0x00, 0x91};
    uint8_t command2[] = {0xff, 0xff, 0x02, 0x30, 0x0, 0x05, 0x00, 0x00, 0x00, 0x00, 0xC8};
    // Set the characteristic's value to be the array of bytes that is actually a string.
  //  pRemoteCharacteristic->writeValue(command2, sizeof(command2));
    delay(1000);
   
    Serial.println("Setting back led to blue");
    uint8_t command4[] = {0xff, 0xff, 0x02, 0x20, 0x37, 0x05, 0x00, 0x00, 0xff, 0x00 ,0xa2};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(command4, sizeof(command4));
    delay(1000);
    Serial.println("Setting back led to green");
    uint8_t command3[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0x00, 0xFF, 0x00, 0x00 ,0xD9};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(command3, sizeof(command3));
    delay(2000);
    
    Serial.println("Setting back led to yelllow");
    uint8_t command5[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0xFF, 0xFF, 0x00, 0x00 ,0xDA};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(command5, sizeof(command5));
    delay(2000);
    
    Serial.println("Setting back led to red");
   uint8_t command6[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0xFF, 0x00, 0x00, 0x00 ,0xD9};
   // always 0xff and 0xff or 0xfe to start with 0x02 is the DID 0x20 is the CID
   //0x00 is the seq no 0 if no response required  0x05 is the data length 
   // 0xff,0x00,0x00 is the R,G,B 0x00 always 0xD9 is the checksum 
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(command6, sizeof(command6));
    delay(2000);
    
  }else if(doScan){
    Serial.println("not connected");
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(5000); // Delay a second between loops.
} // End of loop
