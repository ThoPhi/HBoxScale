#include "HX711.h"

#define LOADCELL_DOUT_PIN  4
#define LOADCELL_SCK_PIN   5

HX711 scale;


void scale_init() {
  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  PRINT_DEBUG(Serial.println("Setting Scale"));
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  PRINT_DEBUG(Serial.println("Before setting up the scale:"));
  PRINT_DEBUG(Serial.print("read: \t\t"));
  PRINT_DEBUG(Serial.println(scale.read()));      // print a raw reading from the ADC

  PRINT_DEBUG(Serial.print("read average: \t\t"));
  PRINT_DEBUG(Serial.println(scale.read_average(20)));   // print the average of 20 readings from the ADC

  PRINT_DEBUG(Serial.print("get value: \t\t"));
  PRINT_DEBUG(Serial.println(scale.get_value(5)));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  PRINT_DEBUG(Serial.print("get units: \t\t"));
  PRINT_DEBUG(Serial.println(scale.get_units(5), 1));  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  // by the SCALE parameter (not set yet)
  scaleTareValue = scale.get_units(20);
  //  scale.set_scale(22800.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  //  scale.set_scale(26612.f);
  //  scale.set_scale(22831.f);
//  scale.set_scale(24000.f);
scale.set_scale(26000.f);
  scale.tare();               // reset the scale to 0

  PRINT_DEBUG(Serial.println("After setting up the scale:"));

  PRINT_DEBUG(Serial.print("read: \t\t"));
  PRINT_DEBUG(Serial.println(scale.read()));                 // print a raw reading from the ADC

  PRINT_DEBUG(Serial.print("read average: \t\t"));
  PRINT_DEBUG(Serial.println(scale.read_average(20)));       // print the average of 20 readings from the ADC

  PRINT_DEBUG(Serial.print("get value: \t\t"));
  PRINT_DEBUG(Serial.println(scale.get_value(5)));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  PRINT_DEBUG(Serial.print("get units: \t\t"));
  PRINT_DEBUG(Serial.println(scale.get_units(5), 1));        // print the average of 5 readings from the ADC minus tare weight, divided
  // by the SCALE parameter set with set_scale

  PRINT_DEBUG(Serial.println("Readings:"));

  thisScale.offset = 183000 ;
  thisScale.scale = 30000;
  thisScale.threshold = 2;//2kg
}

void set_offset_scale() {
  scale.tare();
}

boolean scale_check() {
  float returnValue = scale.get_units(4);
  scaleReadBuffer[scaleReadBufferIndex++] = returnValue;
  if (scaleReadBufferIndex >= SCALE_BUFFER_MAX)       scaleReadBufferIndex = 0;
  if (returnValue > 2 ) { //>1837xx
    return true;
  }
  return false;
}

long readHX711(void){
  return scale.read();
}


float scale_read(uint8_t nbrReadTimes) {
  float sum;
  byte i;
  //  PRINT_DEBUG(Serial.print("Start reading scale value at time:"));
  //  PRINT_DEBUG(Serial.println(millis()));
  //  scale.power_up();
  float returnValue = scale.get_units(1);;//= scale.get_units(nbrReadTimes);
  //  PRINT_DEBUG(Serial.println(scale.get_value(1)));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  PRINT_DEBUG(Serial.println(scale.read()));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  for ( i = 0; i < nbrReadTimes; i++) {
    returnValue = scale.get_units(1);
    scaleReadBuffer[scaleReadBufferIndex] = returnValue;
    scaleReadBufferIndex++;
    if (scaleReadBufferIndex >= SCALE_BUFFER_MAX) {
      scaleReadBufferIndex = 0;
    }
  }
  sum = 0;
  for ( i = 0; i < SCALE_BUFFER_MAX; i++) {
    sum += scaleReadBuffer[i] ;
  }
  thisNode.weight = sum / SCALE_BUFFER_MAX;
  //  }
  //  scale.power_down();
  //  PRINT_DEBUG(Serial.print("end reading from scale"));
  //  PRINT_DEBUG(Serial.println(millis()));
  return returnValue;
}
