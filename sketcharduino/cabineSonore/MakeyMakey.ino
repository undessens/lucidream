//
//
//
///////////////////////////
//// DEBUG DEFINITIONS ////
///////////////////////////
////see Seerial.h
//
//
//
//
////#include "settings.h"
//
//
//
//
///////////////////////////
//// STRUCT ///////////////
///////////////////////////
//typedef struct {
//  byte pinNumber;
//  byte measurementBuffer[BUFFER_LENGTH];
//  boolean oldestMeasurement;
//  byte bufferSum;
//  boolean pressed;
//  boolean prevPressed;
//
//}
//MakeyMakeyInput;
//
//MakeyMakeyInput inputs[NUM_INPUTS];
//
/////////////////////////////////////
//// VARIABLES //////////////////////
/////////////////////////////////////
//int bufferIndex = 0;
//byte byteCounter = 0;
//byte bitCounter = 0;
//
//
//int pressThreshold;
//int releaseThreshold;
//boolean inputChanged;
//
//
//// timing
//int loopTime = 0;
//int prevTime = 0;
//int loopCounter = 0;
//
//
/////////////////////////////
//// FUNCTIONS //////////////
/////////////////////////////
//void initializeArduino();
//void initializeInputs();
//void updateMeasurementBuffers();
//void updateBufferSums();
//void updateBufferIndex();
//void updateInputStates();
//void addDelay();
//void makeyCB(int i, boolean state);
//
//
//
//
////////////////////////
//// SETUP /////////////
////////////////////////
//void initMakey()
//{
//
//  initializeArduino();
//  initializeInputs();
//
//
//}
//
//////////////////////
//// MAIN LOOP ///////
//////////////////////
//void updateMakey()
//{
//  updateMeasurementBuffers();
//  updateBufferSums();
//  updateBufferIndex();
//  updateInputStates();
//  addDelay();
//
//
//
//}
//
////////////////////////////
//// INITIALIZE ARDUINO
////////////////////////////
//void initializeArduino() {
//
//
//  /* Set up input pins
//   DEactivate the internal pull-ups, since we're using external resistors */
//  for (int i = 0; i < NUM_INPUTS; i++)
//  {
//    pinMode(pinNumbers[i], INPUT);
//    digitalWrite(pinNumbers[i], LOW);
//  }
//
//#ifdef DEBUG
//  delay(1000); // allow us time to reprogram in case things are freaking out
//#endif
//
//}
//
/////////////////////////////
//// INITIALIZE INPUTS
/////////////////////////////
//void initializeInputs() {
//
//  float thresholdPerc = SWITCH_THRESHOLD_OFFSET_PERC;
//  float thresholdCenterBias = SWITCH_THRESHOLD_CENTER_BIAS / 50.0;
//  float pressThresholdAmount = (BUFFER_LENGTH * 8) * (thresholdPerc / 100.0);
//  float thresholdCenter = ( (BUFFER_LENGTH * 8) / 2.0 ) * (thresholdCenterBias);
//  pressThreshold = int(thresholdCenter + pressThresholdAmount);
//  releaseThreshold = int(thresholdCenter - pressThresholdAmount);
//
//#ifdef DEBUG
//  Serial.println(pressThreshold);
//  Serial.println(releaseThreshold);
//#endif
//
//  for (int i = 0; i < NUM_INPUTS; i++) {
//    inputs[i].pinNumber = pinNumbers[i];
//
//    for (int j = 0; j < BUFFER_LENGTH; j++) {
//      inputs[i].measurementBuffer[j] = 0;
//    }
//    inputs[i].oldestMeasurement = 0;
//    inputs[i].bufferSum = 0;
//
//    inputs[i].pressed = false;
//    inputs[i].prevPressed = false;
//
//  }
//}
//
//
////////////////////////////////
//// UPDATE MEASUREMENT BUFFERS
////////////////////////////////
//void updateMeasurementBuffers() {
//
//  for (int i = 0; i < NUM_INPUTS; i++) {
//
//    // store the oldest measurement, which is the one at the current index,
//    // before we update it to the new one
//    // we use oldest measurement in updateBufferSums
//    byte currentByte = inputs[i].measurementBuffer[byteCounter];
//    inputs[i].oldestMeasurement = (currentByte >> bitCounter) & 0x01;
//
//    // make the new measurement
//    boolean newMeasurement = digitalRead(inputs[i].pinNumber);
//
//    // invert so that true means the switch is closed
//    newMeasurement = !newMeasurement;
//
//    // store it
//    if (newMeasurement) {
//      currentByte |= (1 << bitCounter);
//    }
//    else {
//      currentByte &= ~(1 << bitCounter);
//    }
//    inputs[i].measurementBuffer[byteCounter] = currentByte;
//  }
//}
//
/////////////////////////////
//// UPDATE BUFFER SUMS
/////////////////////////////
//void updateBufferSums() {
//
//  // the bufferSum is a running tally of the entire measurementBuffer
//  // add the new measurement and subtract the old one
//
//  for (int i = 0; i < NUM_INPUTS; i++) {
//    byte currentByte = inputs[i].measurementBuffer[byteCounter];
//    boolean currentMeasurement = (currentByte >> bitCounter) & 0x01;
//    if (currentMeasurement) {
//      inputs[i].bufferSum++;
//    }
//    if (inputs[i].oldestMeasurement) {
//      inputs[i].bufferSum--;
//    }
//  }
//}
//
/////////////////////////////
//// UPDATE BUFFER INDEX
/////////////////////////////
//void updateBufferIndex() {
//  bitCounter++;
//  if (bitCounter == 8) {
//    bitCounter = 0;
//    byteCounter++;
//    if (byteCounter == BUFFER_LENGTH) {
//      byteCounter = 0;
//    }
//  }
//}
//
/////////////////////////////
//// UPDATE INPUT STATES
/////////////////////////////
//void updateInputStates() {
//  inputChanged = false;
//  for (int i = 0; i < NUM_INPUTS; i++) {
//    inputs[i].prevPressed = inputs[i].pressed; // store previous pressed state (only used for mouse buttons)
//    if (inputs[i].pressed) {
//      if (inputs[i].bufferSum < releaseThreshold) {
//        inputChanged = true;
//        inputs[i].pressed = false;
//        makeyCB(i, false);
//      }
//
//    }
//    else if (!inputs[i].pressed) {
//      if (inputs[i].bufferSum > pressThreshold) {  // input becomes pressed
//        inputChanged = true;
//        inputs[i].pressed = true;
//        makeyCB(i, true);
//      }
//    }
//  }
//
//}
//
//
/////////////////////////////
//// ADD DELAY
/////////////////////////////
//void addDelay() {
//
//  loopTime = micros() - prevTime;
//  if (loopTime < TARGET_LOOP_TIME) {
//    int wait = TARGET_LOOP_TIME - loopTime;
//    delayMicroseconds(wait);
//  }
//
//  prevTime = micros();
//
//#ifdef DEBUG_TIMING
//  if (loopCounter == 0) {
//    int t = micros() - prevTime;
//    Serial.println(t);
//  }
//  loopCounter++;
//  loopCounter %= 999;
//#endif
//
//}
//
//void makeyCB(int i, boolean state) {
////  Serial.print ((char)(i+48));
////Serial.println("makey");
//  sendMsg((char)(i+48), (char)(state ? 1 : 0));
//
//}
//

