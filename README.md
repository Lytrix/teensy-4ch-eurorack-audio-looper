# Teensy Audio Looper with eurorack-pmod

## Used libraries ##

AK4619 codec: https://forum.pjrc.com/index.php?threads/tdm-with-ak4619.75104/
Teensy TDM recorder to SD: https://github.com/MSWagner/teensy-tools/
24 bit audio codec board: https://github.com/whollender/SuperAudioBoard
Arduino Codec: https://github.com/TrimenZ/ak4619vn/tree/main
Teensy Codec for TDM: https://github.com/janscience/TeeRec/tree/main
Teensy Audio Library: https://github.com/PaulStoffregen/Audio/
Save audio to file: https://forum.pjrc.com/index.php?threads/build-a-teensy-based-cdj-can-it-be-done.75232/#post-345104

IMRXT1062 DRIVERS: https://github.com/nxp-mcuxpresso/mcux-sdk

Led Driver   PCA9635     :i2c address 0x03
Codec In/Out AK4619vn    :i2c address 0x10
Jack Detect  PCA9557PW   :i2c address 0x30
Cal Mem      24AA025UIDT :i2c address 0x0A


PMOD pinouts
3.3V 12 11 3.3V
GND  10  9 GND
MCLK  8  7 BICK
PDN   6  5 LRCK
SDA   4  3 SDOUT1
SCL   2  1 SDIN1

Teensy Pin setup for Audio clocks use ALT3 (see PDF in docs folder): 
pin 23: SAI1 MCLK
pin 21: SAI1 BCK
pin 20: SAI1 SYNC 

### 24bit 192kHz codec for Teensy 4.1 ###
fs = 192.000
Res = 24

Res = 32
Single-ended Inputs
No Digital HPF

TDM Format using only SDIN1 and SDOUT1

i2C control, max 400 KHz clock

Set controls in Reset mode?

CAD = LOW
PDN/Reset = 3.3V delay 1ms to prevent pops and ensure it is on/off
LRCK = fs
BICK =  64fs = 12.288 Mhz
MCLK = 128fs = 24.576 Mhz




256x96000= 24576000 = 24.576 crystal
128x192000= same


### usefull commands ###

imxrt.h
```
#define CCM_ANALOG_PLL_AUDIO 	    (IMXRT_CCM_ANALOG.offset070)
#define CCM_ANALOG_PLL_AUDIO_SET	(IMXRT_CCM_ANALOG.offset074)
#define CCM_ANALOG_PLL_AUDIO_CLR	(IMXRT_CCM_ANALOG.offset078)
#define CCM_ANALOG_PLL_AUDIO_TOG	(IMXRT_CCM_ANALOG.offset07C)
#define CCM_ANALOG_PLL_AUDIO_NUM	(IMXRT_CCM_ANALOG.offset080)
#define CCM_ANALOG_PLL_AUDIO_DENOM	(IMXRT_CCM_ANALOG.offset090)
```

https://github.com/PaulStoffregen/cores/blob/58224e5554d0cda93f92c52078a500a0d898a996/teensy4/core_pins.h#L550



### To also check/update later ###

audio memory friendly DMA PR:
https://github.com/Blackaddr/cores/commit/46b851b6fe36262bd406e5ef7071e0feb81f7e9d

https://github.com/PaulStoffregen/Audio/blob/74ff8f5f288a66250d898f59136a7f9c602e9e06/play_memory.cpp#L191

2*fs*slotlength 2*192000*32



https://community.nxp.com/t5/i-MX-Processors/How-to-generate-24-576MHz-CLKO1-from-24MHz-OSC-in-SABRE-SD-board/m-p/477214
one can try to configure PLL for 737.28MHz (24.576=737.28/30) with
```*(30 + (72/100))```
/5 /6  for MCLK 24.576 MHz
/4 /15 for BCLK 12.288 Mhz

DIV=30, NUM=72, DENOM=100 and dividing with ssi_clk_pred, with ssi_clk_podf,

output ssi_clk_root on CLKO.

another example testing jitter:
https://www.nxp.com/docs/en/application-note/AN4952.pdf

Word length = 24 bits
Slot length = 32 bits