#include "BLEDevice.h"

// used to write commands
static BLEUUID serviceUUID("22bb746f-2ba0-7554-2d6f-726568705327");
static BLEUUID charUUID("22bb746f-2ba1-7554-2d6f-726568705327");

// used to wake up ollie
static BLEUUID serviceUUID2("22bb746f-2bb0-7554-2d6f-726568705327");
static BLEUUID charUUID2("22bb746f-2bb2-7554-2d6f-726568705327");
static BLEUUID charUUID3("22bb746f-2bbd-7554-2d6f-726568705327");
static BLEUUID charUUID4("22bb746f-2bbf-7554-2d6f-726568705327");

BLEClient* pClient1 = BLEDevice::createClient();
BLEClient* pClient2 = BLEDevice::createClient();
BLEClient* pClient3 = BLEDevice::createClient();
BLEClient* pClient4 = BLEDevice::createClient();

BLEAddress Device1 = BLEAddress("cf:64:6f:d8:b9:fd");  // 2B-B9FD
BLEAddress Device2 = BLEAddress("df:3b:08:28:d0:ee"); // 2B-D0EE
BLEAddress Device3 = BLEAddress("c2:88:53:64:50:ed"); //2B-50ED
BLEAddress Device4 = BLEAddress("fe:95:0e:12:b8:7e");
BLEAddress Device5 = BLEAddress("f8:7e:3b:4b:2f:e7");
BLEAddress Device6 = BLEAddress("dd:20:4c:9f:ef:db"); // 2B-EFDB
BLEAddress Device7 = BLEAddress("d5:85:34:53:eb:2c");
BLEAddress Device8 = BLEAddress("d0:aa:a4:df:b8:8c");
BLEAddress Device9 = BLEAddress("e6:26:df:e6:97:97");
BLEAddress Device10 = BLEAddress("d5:80:8b:0e:58:fc");
BLEAddress Device11 = BLEAddress("d1:b7:4b:10:19:35");  // 2B-1935

BLERemoteCharacteristic* pRemoteCharacteristicWrite1;
BLERemoteCharacteristic* pRemoteCharacteristicWrite2;
BLERemoteCharacteristic* pRemoteCharacteristicWrite3;
BLERemoteCharacteristic* pRemoteCharacteristicWrite4;

BLERemoteCharacteristic* pRemoteCharacteristicComm5;
BLERemoteCharacteristic* pRemoteCharacteristicComm6;
BLERemoteCharacteristic* pRemoteCharacteristicComm7;
BLERemoteCharacteristic* pRemoteCharacteristicComm8;

static BLEAdvertisedDevice* myDevice1;
static BLEAdvertisedDevice* myDevice2;
static BLEAdvertisedDevice* myDevice3;
static BLEAdvertisedDevice* myDevice4;

static boolean doConnect1 = false;
static boolean doConnect2 = false;
static boolean doConnect3 = false;
static boolean doConnect4 = false;
static boolean connected1 = false;
static boolean connected2 = false;
static boolean connected3 = false;
static boolean connected4 = false;

static boolean Allconnected = false;

static boolean doScan = false;

int iRobots = 0;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    // Handle notifications
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.println("BLEClientcallbacks says robot is now connected");
  }

  void onDisconnect(BLEClient* pclient) {
    Serial.println("onDisconnect");
  }
};

BLERemoteCharacteristic* initializeBLEClient(BLEClient* pClient, BLEAdvertisedDevice* myDevice, BLEUUID serviceUUID, BLEUUID charUUID, BLEUUID serviceUUID2, BLEUUID charUUID2, BLEUUID charUUID3, BLEUUID charUUID4) {
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());
  Serial.println(" - Client callbacks set");

  if (myDevice != nullptr) {
    if (!pClient->connect(myDevice)) {
      Serial.println("Failed to connect to the device.");
      return nullptr;
    }
    Serial.println(" - Connected to device");
  } else {
    Serial.println("No device to connect to.");
    return nullptr;
  }

  pClient->setMTU(517);
  Serial.println(" - MTU set to 517");

  BLERemoteService* pRemoteServiceComm1 = pClient->getService(serviceUUID);
  if (pRemoteServiceComm1 == nullptr) {
    Serial.print("Failed to find communication service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return nullptr;
  }

  BLERemoteService* pRemoteServiceComm2 = pClient->getService(serviceUUID2);
  if (pRemoteServiceComm2 == nullptr) {
    Serial.print("Failed to find communication service UUID: ");
    Serial.println(serviceUUID2.toString().c_str());
    pClient->disconnect();
    return nullptr;
  }

  // Obtain and validate characteristics for the first device
  BLERemoteCharacteristic* pRemoteCharacteristicComm1 = pRemoteServiceComm1->getCharacteristic(charUUID);
  if (pRemoteCharacteristicComm1 == nullptr) {
    Serial.print("Failed to find communication characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return nullptr;
  }

  
  BLERemoteCharacteristic* pRemoteCharacteristicComm2 = pRemoteServiceComm2->getCharacteristic(charUUID2);
  if (pRemoteCharacteristicComm2 == nullptr) {
    Serial.print("Failed to find communication characteristic UUID: ");
    Serial.println(charUUID2.toString().c_str());
    pClient->disconnect();
    return nullptr;
  }

  // Obtain and validate characteristics for the second device
  BLERemoteCharacteristic* pRemoteCharacteristicComm3 = pRemoteServiceComm2->getCharacteristic(charUUID3);
  if (pRemoteCharacteristicComm3 == nullptr) {
    Serial.print("Failed to find communication characteristic UUID: ");
    Serial.println(charUUID3.toString().c_str());
    pClient->disconnect();
    return nullptr;
  }

  BLERemoteCharacteristic* pRemoteCharacteristicComm4 = pRemoteServiceComm2->getCharacteristic(charUUID4);
  if (pRemoteCharacteristicComm4 == nullptr) {
    Serial.print("Failed to find communication characteristic UUID: ");
    Serial.println(charUUID4.toString().c_str());
    pClient->disconnect();
    return nullptr;
  }
  if(pRemoteCharacteristicComm1->canNotify())
      pRemoteCharacteristicComm1->registerForNotify(notifyCallback);

  Serial.println(" - Found communication characteristics");
  setDeviceSettings(pRemoteCharacteristicComm3, pRemoteCharacteristicComm2, pRemoteCharacteristicComm4);
 
  //   delay(500);
  //return true;
  return pRemoteCharacteristicComm1;

}
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if ( myDevice1 == nullptr) {
       if (advertisedDevice.getAddress().equals(Device1)) {
          BLEDevice::getScan()->stop();
          myDevice1 = new BLEAdvertisedDevice(advertisedDevice);
          Serial.print("in MyAdvertisedDeviceCallbacks Device 1 found: ");
          doConnect1 = true;
        }
    }    
    if ( myDevice2 == nullptr) {
       if (advertisedDevice.getAddress().equals(Device2)) {
          BLEDevice::getScan()->stop();
          myDevice2 = new BLEAdvertisedDevice(advertisedDevice);
          Serial.print("in MyAdvertisedDeviceCallbacks Device 2 found: ");
          doConnect2 = true;
       }
    }
    if ( myDevice3 == nullptr) {
       if (advertisedDevice.getAddress().equals(Device8)) {
          BLEDevice::getScan()->stop();
          myDevice3 = new BLEAdvertisedDevice(advertisedDevice);
          Serial.print("in MyAdvertisedDeviceCallbacks Device 3 found: ");
          doConnect3 = true;
       }
    }
    if ( myDevice4 == nullptr) {
      if (advertisedDevice.getAddress().equals(Device9)) {
         BLEDevice::getScan()->stop();
         myDevice4 = new BLEAdvertisedDevice(advertisedDevice);
         Serial.print("in MyAdvertisedDeviceCallbacks Device 4 found: ");
         doConnect4 = true;
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//  pBLEScan->setInterval(2500);
//  pBLEScan->setWindow(5000);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(0, true);
//  BLEScanResults foundDevices = pBLEScan->start(100, false);
//  Serial.println(foundDevices.getCount());

  pBLEScan->clearResults(); 
}
bool connectToServer() {
    Serial.print("Forming a connection to ");
  //  Serial.println(myDevice1->getAddress().toString().c_str());
    
pRemoteCharacteristicWrite1 = initializeBLEClient(pClient1, myDevice1, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4);
pRemoteCharacteristicWrite2 = initializeBLEClient(pClient2, myDevice2, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4);
pRemoteCharacteristicWrite3 = initializeBLEClient(pClient3, myDevice3, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4);
pRemoteCharacteristicWrite4 = initializeBLEClient(pClient4, myDevice4, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4);

    Allconnected = true;
    return true;
}
void setDeviceSettings(BLERemoteCharacteristic* antiChar, BLERemoteCharacteristic* txChar, BLERemoteCharacteristic* wakeChar) {
/*
waking up an Ollie this is the order

011i3 to antichar
x07 to txChar
01 to wakechar

*/
  bool bResponse;
  Serial.println("Setting anti");
//  String str = "011i3";
// byte unicodeArray[str.length()];

  // Convert the characters to Unicode code points and store them in the array
//  for (int i = 0; i < str.length(); i++) {
//    unicodeArray[i] = str.charAt(i);
//  }

  // Create a Uint8Array equivalent (byte array)
//  byte result[str.length()];
//  for (int i = 0; i < str.length(); i++) {
//    result[i] = unicodeArray[i];
//  }

  // Setting anti
  uint8_t commandanti[] = {48, 49, 49, 105, 51};
  antiChar->writeValue(commandanti, sizeof(commandanti),bResponse);
  if (bResponse) {
   delay(1000);
   Serial.println("failed to write Set anti");
  }

  // Setting tx
  uint8_t commandp[] = {0x07};
  txChar->writeValue(commandp, sizeof(commandp));
  delay(1000);
  Serial.println("Set tx");

  // Setting wake
  uint8_t commandw[] = {0x01};
  wakeChar->writeValue(commandw, sizeof(commandw));
  delay(1000);
  Serial.println("Set wake");

}
void WriteCommand(BLERemoteCharacteristic* rCharacteristic, uint8_t* command, int aSize) {
  bool bResponse;
    if (rCharacteristic == nullptr) {
        Serial.println("Failed to find communication characteristic for writing.");
        return;
    }
    // sizeof doesnt work for dynamic arrays
   rCharacteristic->writeValue(command, aSize,bResponse);
    if (bResponse) {
       Serial.println("failed to write to the device");
    }
     Serial.println("command written.");
    
     
}
void WriteCommands(uint8_t* command, int aSize) {
    WriteCommand(pRemoteCharacteristicWrite1, command, aSize);
    WriteCommand(pRemoteCharacteristicWrite2, command, aSize);
    WriteCommand(pRemoteCharacteristicWrite3, command, aSize);
    WriteCommand(pRemoteCharacteristicWrite4, command, aSize);
}


void loop() {
  
  if (doConnect1 == true && connected1 == false) {
//    if (connectToServer(pClient1, myDevice1, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4)) {
      Serial.println("Device 1 connected");
      connected1 = true;
      iRobots = iRobots+1 ;
  }

  if (doConnect2 == true && connected2 == false) {
//    if (connectToServer(pClient2, myDevice2, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4)) {
      Serial.println("Device 2 connected");
      connected2 = true;
      iRobots = iRobots+1 ;
  }

  if (doConnect3 == true && connected3 == false) {
//    if (connectToServer(pClient3, myDevice3, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4)) {
      Serial.println("Device 3 connected");
      connected3 = true;
      iRobots = iRobots+1 ;
  }

  if (doConnect4 == true && connected4 == false) {
//    if (connectToServer(pClient4, myDevice4, serviceUUID, charUUID, serviceUUID2, charUUID2, charUUID3, charUUID4)) {
      Serial.println("Device 4 connected");
      connected4 = true;
      iRobots = iRobots+1 ;
  }

  if (iRobots < 4 ) {
     Serial.println("waiting to connect to all 4");
     setup();
     delay(500);
     return;
 }

  connectToServer();

/*
  if (connected) {
    // Perform actions when connected
  } else if (doScan) {
    Serial.println("not connected");
    BLEDevice::getScan()->start(0);
  }
*/ 
    uint8_t pingcommand[] = {0xff, 0xff, 0x00, 0x01, 0x37, 0x01, 0xc7};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    WriteCommands(pingcommand, sizeof(pingcommand));

    uint8_t commandA[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0xFF, 0x00, 0x00, 0x00 ,0xD9};  
    WriteCommands(commandA, sizeof(commandA));

    delay(1000);

    Serial.println("Setting new led to green");
    uint8_t command1[] = {0xff, 0xff, 0x02, 0x20, 0x37, 0x05, 0x00, 0xFF, 0xFF, 0x01 ,0xa2};  
    WriteCommands(command1, sizeof(command1));
;
    delay(1000);

    Serial.println("rolling"); 
    uint8_t command2[] = {0xFF, 0xFF, 0x02, 0x30, 0x37, 0x05, 0x80, 0x01, 0x0E, 0x01, 0x01};
    WriteCommands(command2,sizeof(command2));
    delay(500);
  
    Serial.println("Stopping");
    uint8_t command3[] = {0xff, 0xff, 0x02, 0x30, 0x0, 0x05, 0x00, 0x00, 0x00, 0x00, 0xC8};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    WriteCommands(command3,sizeof(command3));   
    delay(1000);

    
    Serial.println("Setting back led to green");
    uint8_t command4[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0x00, 0xFF, 0x00, 0x00 ,0xD9};
    // Set the characteristic's value to be the array of bytes that is actually a string.
    WriteCommands(command4,sizeof(command4));   
    delay(1000);

  
    Serial.println("Setting back led to yelllow");
    uint8_t command5[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0xFF, 0xFF, 0x00, 0x00 ,0xDA};
    WriteCommands(command5,sizeof(command5));   
    delay(1000);

    Serial.println("Setting back led to red");
    uint8_t command6[] = {0xff, 0xff, 0x02, 0x20, 0x00, 0x05, 0xFF, 0x00, 0x00, 0x00 ,0xD9};
    WriteCommands(command6,sizeof(command6));   
   // always 0xff and 0xff or 0xfe to start with 0x02 is the DID 0x20 is the CID
   //0x00 is the seq no 0 if no response required  0x05 is the data length 
   // 0xff,0x00,0x00 is the R,G,B 0x00 always 0xD9 is the checksum 
    // Set the characteristic's value to be the array of bytes that is actually a string.
  
  delay(5000);
}



