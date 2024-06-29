#include <Arduino.h>
#include <Wire.h>
#include <AK4619VN.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
//#include <output_i2s.h>
//#include <input_i2s.h>
#include <input_tdm.h>
#include <output_tdm.h>
//#include <Audio.h>
#include <synth_sine.h>
#include <FreqCount.h>

AK4619VN codec(&Wire, AK4619VN_ADDR);

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=254,456
//AudioSynthWaveform       waveform1;
AudioInputTDM            tdm1;           //xy=451,277
AudioOutputTDM           tdm2;           //xy=448,545
// AudioOutputI2S           i2s1; 
//AudioConnection          patchCord1(waveform1, 0, tdm2, 0);
//AudioConnection          patchCord2(sine1, 0, tdm2, 0);
AudioConnection          patchCord3(tdm1, 0, tdm2, 0);
//AudioConnection          patchCord4(tdm1, 3, tdm2, 3);
// GUItool: end automatically generated code

// GUItool: begin automatically generated code
//AudioInputTDM            tdm2;           //xy=309,481
//AudioOutputTDM           tdm1;           //xy=469,481
//AudioConnection          patchCord1(tdm2, 0, tdm1, 0);
//AudioConnection          patchCord2(tdm2, 1, tdm1, 1);
//AudioConnection          patchCord3(tdm2, 2, tdm1, 2);
//AudioConnection          patchCord4(tdm2, 3, tdm1, 3);
// GUItool: end automatically generated code


// // GUItool: begin automatically generated code
//AudioInputI2S            i2s2;           //xy=309,481
//AudioOutputI2S           i2s1;           //xy=469,481
//AudioConnection          patchCord1(i2s2, 0, i2s1, 0);
//AudioConnection          patchCord2(i2s2, 1, i2s1, 1);
// // GUItool: end automatically generated code


void setup() {
  AudioMemory(100);
  //Serial.begin(115200);
  Serial.begin(9600);
  //waveform1.begin(0.95, 207.65, WAVEFORM_TRIANGLE);
  sine1.amplitude(0.9);
  sine1.frequency(100);
  delay(1000);

  // Enable the Audio codec
  pinMode(22, OUTPUT); //PWN on
  digitalWrite(22, HIGH);
  delay(200);
  
  codec.begin(18, 19); // SDA, SCL for Teensy 4.1
  /*    pin  6 : SDIN
        pin  7 : SDOUT
        pin 23 : MCLK
        pin 21 : BICK
        pin 20 : SYNC */
  
  //Set CODEC to reset state for initialization
  uint8_t error = 0;
  error = codec.setRstState(true); 
  if(error){
    Serial.println("Unable to set codec reset state");
  }
  //Enable PWR of DAC1&2 and ADC1&2
  error = codec.pwrMgm(true, true, true, true); 
  if(error){
    Serial.println("Unable to set codec PWR");
  }
  // Set Slot start (LR (false) or Slot length) and Slot Length for DAC 1&2 and ADC1&2
  error = codec.audioFormatSlotLen(AK4619VN::AK_SLOT, AK4619VN::AK_32BIT, AK4619VN::AK_24BIT);
  if(error){
    Serial.println("Unable to set slot length.");
  }
  // Set Mode I2S/TDM, BICK Edge falling/rising, SDOut speed slow/fast
  //error = codec.audioFormatMode(AK4619VN::AK_I2S_STEREO, false, false);
  //error = codec.audioFormatMode(AK4619VN::AK_TDM256_I2S_32B, false, false);
  error = codec.audioFormatMode(AK4619VN::AK_TDM128_I2S_32B, true, false);
  //error = codec.audioFormatMode(AK4619VN::AK_TDM128_MSB_32B, true, false);
  
  if(error){
    Serial.println("Unable to set audio format mode.");
  }  
  // Set sample rate to 96kHz
  //error = codec.sysClkSet(AK4619VN::AK_256FS_96KS);
  error = codec.sysClkSet(AK4619VN::AK_128FS_192KS);
  if(error){
    Serial.println("Unable to set system clock mode.");
  }
  // Set all 4 ADC gains to 0dB
  error = codec.micGain(AK4619VN::AK_MIC_GAIN_0DB, AK4619VN::AK_MIC_GAIN_0DB, AK4619VN::AK_MIC_GAIN_0DB, AK4619VN::AK_MIC_GAIN_0DB); 
  if(error){
    Serial.println("Unable to set codec mic input gain.");
  }
  // // Set all 4 ADC digital volume to 0dB
  // error = codec.inputGain(AK4619VN::AK_IN_GAIN_0DB, AK4619VN::AK_IN_GAIN_0DB, AK4619VN::AK_IN_GAIN_0DB, AK4619VN::AK_IN_GAIN_0DB); 
  // if(error){
  //   Serial.println("Unable to set codec digital input input gain.");
  // }
  //Set all DAC1 LR gains to 0dB
  error = codec.outputGain(false, AK4619VN::AK_DAC1B, AK4619VN::AK_OUT_GAIN_0DB); 
  if(error){
    Serial.println("Unable to set DAC1 gain.");
  }
  //Set all DAC2 LR gains to 0dB
  error = codec.outputGain(false, AK4619VN::AK_DAC2B, AK4619VN::AK_OUT_GAIN_0DB);
  if(error){ 
    Serial.println("Unable to set DAC2 gain.");
  }
  //Set Input config to Single ended 1 on both ADCs
  error = codec.inputConf(AK4619VN::AK_IN_SE1, AK4619VN::AK_IN_SE1, AK4619VN::AK_IN_SE1, AK4619VN::AK_IN_SE1);
  if(error){
    Serial.println("Unable to set DAC input configuration.");
  }
  //DAC2 to SDOUT2, DAC1 to SDOUT1
  error = codec.outputConf(AK4619VN::AK_OUT_SDIN2, AK4619VN::AK_OUT_SDIN1); 
  //error = codec.outputConf(AK4619VN::AK_OUT_SDOUT1, AK4619VN::AK_OUT_SDOUT1); 
  if(error){
    Serial.println("Unable to set DAC input configuration.");
  }
  
  //Release Reset state
  error = codec.setRstState(false); 
  if(error){
    Serial.println("Unable to clear codec reset state.");
  }
  delay(100);
  Serial.println("Audio Codec Setup done.");
  
  //Verify settings
  codec.printRegs(0x0, 21);
  
  //Update Time in microseconds
  FreqCount.begin(1000000);  
}

void loop() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    Serial.println(count);
  }
  
}

// put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
//}