/*

*/

/* Includes ------------------------------------------------------------------*/
#include <SoftwareSerial.h>

/* Private define ------------------------------------------------------------*/
#define SOFT_SERIAL_RX_PIN      2   //3
#define SOFT_SERIAL_TX_PIN      3   //2
#define MAX485_TX_RX_CONTROL    7    //notuse

#define RS485Transmit           HIGH         //not use
#define RS485Receive            LOW          //not use
#define RS485_BAUDRATE          9600
/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SoftwareSerial softSerial(SOFT_SERIAL_RX_PIN, SOFT_SERIAL_TX_PIN);

/* Private user code ---------------------------------------------------------*/
void communication_init() {
  softSerial.begin(RS485_BAUDRATE);
  pinMode(MAX485_TX_RX_CONTROL, OUTPUT);
  digitalWrite ( MAX485_TX_RX_CONTROL, RS485Receive);
}

void communication() {
  if (softSerial.available() ) {
    byte tmp = softSerial.read();
    switch (tmp) {
      case 'w':
        softSerial.write(thisNode.weight);
        break;
      case 'h':
        softSerial.write(thisNode.heartBeat);
        break;
      case 'o':
        softSerial.write(thisNode.SpO2);
        break;
      case 'a':
        softSerial.write(thisNode.nodeStatus);
        break;
      case 's': //set scale scale
        break;
      case 'O': //set scale offset
        break;
      default:

        break;
    }
  }
}


//void rs_485_send(float dataToSend) {
//  softSerial.write(dataToSend);
//}
//
//void rs_485_send_byte(byte dataToSend) {
//  softSerial.write(dataToSend);
//  delay(1);
//}

//void rs485_send(byte byteToSend) {
//  digitalWrite(MAX485_TX_RX_CONTROL, RS485Transmit);  // cho phép truyền
//  softSerial.write(byteToSend); // gửi dữ liệu ngược lại
//  delay(10);
//  digitalWrite(MAX485_TX_RX_CONTROL, RS485Receive);  // Không cho phép truyền
//}


void rs485_send(byte byteToSend) {
  //  digitalWrite(MAX485_TX_RX_CONTROL, RS485Transmit);  // cho phép truyền
  softSerial.write(byteToSend); // gửi dữ liệu ngược lại
  //  delay(10);
  //  digitalWrite(MAX485_TX_RX_CONTROL, RS485Receive);  // Không cho phép truyền
}



void send_weight_to_computer(float weight) {
  union uweight tmpWeight;
  tmpWeight.fweight = weight;
  if (millis() - lastTimeSendToMain > 500) {
    //    if (tmpWeight.fweight > 2) {
    rs485_send('*');
    rs485_send(tmpWeight.bweight[3]);
    rs485_send(tmpWeight.bweight[2]);
    rs485_send(tmpWeight.bweight[1]);
    rs485_send(tmpWeight.bweight[0]);
    rs485_send('#');
    //    }
    lastTimeSendToMain = millis();
  }
}


//float lastWeightSend;
//void send_weight_to_computer(float weight) {
//  union uweight tmpWeight;
//  tmpWeight.fweight = weight;
//  if (lastWeightSend == 0 && weight > 0) {
//    rs485_send('*');
//    rs485_send(tmpWeight.bweight[3]);
//    rs485_send(tmpWeight.bweight[2]);
//    rs485_send(tmpWeight.bweight[1]);
//    rs485_send(tmpWeight.bweight[0]);
//    rs485_send('#');
//  }
//  else if (lastWeightSend == 0 && weight == 0) {
//    if (millis() - lastTimeSendToMain > 2000) {
//      //    if (tmpWeight.fweight > 2) {
//      rs485_send('*');
//      rs485_send(tmpWeight.bweight[3]);
//      rs485_send(tmpWeight.bweight[2]);
//      rs485_send(tmpWeight.bweight[1]);
//      rs485_send(tmpWeight.bweight[0]);
//      rs485_send('#');
//      //    }
//      lastTimeSendToMain = millis();
//    }
//  }
//  else {
//    if (millis() - lastTimeSendToMain > 500) {
//      //    if (tmpWeight.fweight > 2) {
//      rs485_send('*');
//      rs485_send(tmpWeight.bweight[3]);
//      rs485_send(tmpWeight.bweight[2]);
//      rs485_send(tmpWeight.bweight[1]);
//      rs485_send(tmpWeight.bweight[0]);
//      rs485_send('#');
//      //    }
//      lastTimeSendToMain = millis();
//    }
//  }
//
//  if (weight > 2 ) lastWeightSend = weight;
//  else lastWeightSend = 0;
//}
