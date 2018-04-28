/* 
	Editor: http://www.visualmicro.com
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Nano w/ ATmega328, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 105
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
//
//
void SetMultiplexerAddy(uint8_t addy);
void SetDataIO(uint8_t Ldata);
void LatchCubeData(uint8_t multiplexer, uint8_t data);
void LatchLayerPower(uint8_t data);
void Setup_Timer1(uint16_t reloadVal);
uint8_t CubeRowToUInt(uint8_t layer, uint8_t row);
bool InRange(uint8_t Z, uint8_t X, uint8_t Y);
void  SetPixel(uint8_t Z, uint8_t X, uint8_t Y, bool level);
bool GetPixel(uint8_t Z, uint8_t X, uint8_t Y);
void SetLine(uint8_t axis, uint8_t layer, uint8_t pos, uint8_t data);
void SetCubeLayer(uint8_t axis, uint8_t layer, bool level);
void SetAll();
void ClearAll();
void ShiftLayerData(uint8_t axis, bool dir);
void CheckArgOrder(int in1, int in2, int *out1, int *out2);
void DrawWireframe(int ZPos1, int XPos1, int YPos1, int ZPos2, int XPos2, int YPos2);
void GetCharPattern(char chr, unsigned char rtnChr[5]);
void ResetTextPath();
void AddChrToPath(unsigned char inputChr, int pos);
void IncrementPath();
void AddPathToCube();
void AddTextToCubeLayer(char inputChar, int axis);
void Effect_Rain(uint16_t iterations, uint16_t iterationDelay);
void Effect_TextScroll(uint16_t iterations, String inputString, uint16_t scrollDelay);

#include "pins2_arduino.h" 
#include "pins_arduino.h" 
#include "arduino.h"
#include "LED_Cube_V2.ino"
