/*
============================================================================================================================
========================================================= LED CUBE V2 ======================================================
============================================================================================================================

James McKenna


===== Software Version: V0.1 =====

V0.1 - 23/04/2018 - Initial Program


===== Compatible PCB Revision: Rev A =====

- No Modifications Required

---TOP VEIW (Data):---

	07-15-23-31-39-47-55-63
	06-14-22-30-38-46-54-62
	05-13-21-29-37-45-53-61
	04-12-20-28-36-44-52-60
	03-11-19-27-35-43-51-59
	02-10-18-26-34-42-50-58
	01-09-17-25-33-41-49-57
	00-08-16-24-32-40-48-56
		     Front


---SIDE VIEW (Layer):---

	----7----(H)----
	----6----(G)----
	----5----(F)----
	----4----(E)----
	----3----(D)----
	----2----(C)----
	----1----(B)----
	----0----(A)----
	    Bottom


---Cord sys:---

    Z Y
    |/
    :--X
	
	//Main Data Array
	cubeData[Z][X][Y]


===== NOTES: =====

	---PINS:---
	- MCU_Data0 - D13 - PB.5
	- MCU_Data1 - D12 - PB.4
	- MCU_Data2 - D11 - PB.5
	- MCU_Data3 - D10 - PB.5
	- MCU_Data4 - D9 - PB.5
	- MCU_Data5 - D8 - PB.5
	- MCU_Data6 - D7 - PB.5
	- MCU_Data7 - D6 - PB.5
	
	- MCU_Addy0 - D5 - PD.5
	- MCU_Addy1 - D4 - PD.4
	- MCU_Addy2 - D3 - PD.3
	
	- MCU_CS	- D2 - PD.2
	
	- MCU_LayLatch - A0 (D14) - PC.0
	
	- MCU_VRail	- A1 (D15) - PC.1
	
	- Switch1	- A2 (D16) - PC.2
	- Switch2	- A3 (D17) - PC.3
	
	---TIMERS:---
	- Timer 0	- 8Bit  - Arduino
	- Timer 1	- 16Bit - Layer Update Timer
	- Timer 2	- 8Bit  - ?


*/
//============================================================================================================================
//========================================================= Include Files ====================================================
//============================================================================================================================

#include <arduino2.h>


//============================================================================================================================
//====================================================== Defines / ASCII Array ===============================================
//============================================================================================================================

#define SOFTWAREVER 0.1
//---Reference Constant Definitions---
#define CUBESIZE 8
#define TEXTPATHLENGTH 34
#define DIRDOWN true
#define DIRUP false
#define PIXELON true
#define PIXELOFF false
#define ZAXIS 0
#define XAXIS 1
#define YAXIS 2

//Array contains characters stored in 5 x 8bit sections
volatile const unsigned char chracterStorageArray[455] = {
	0x00,0x00,0x00,0x00,0x00, 0x00,0x5f,0x5f,0x00,0x00,	//  !
	0x00,0x03,0x00,0x03,0x00, 0x14,0x7f,0x14,0x7f,0x14,	// "#
	0x24,0x2a,0x7f,0x2a,0x12, 0x23,0x13,0x08,0x64,0x62,	// $%
	0x36,0x49,0x55,0x22,0x50, 0x00,0x05,0x03,0x00,0x00,	// &'
	0x00,0x1c,0x22,0x41,0x00, 0x00,0x41,0x22,0x1c,0x00,	// ()`
	0x14,0x08,0x3e,0x08,0x14, 0x08,0x08,0x3e,0x08,0x08,	// *+
	0x00,0x50,0x30,0x00,0x00, 0x08,0x08,0x08,0x08,0x08,	// ,-
	0x00,0x60,0x60,0x00,0x00, 0x20,0x10,0x08,0x04,0x02,	// ./
	0x3e,0x51,0x49,0x45,0x3e, 0x00,0x42,0x7f,0x40,0x00,	// 01
	0x42,0x61,0x51,0x49,0x46, 0x21,0x41,0x45,0x4b,0x31,	// 23
	0x18,0x14,0x12,0x7f,0x10, 0x27,0x45,0x45,0x45,0x39,	// 45
	0x3c,0x4a,0x49,0x49,0x30, 0x01,0x71,0x09,0x05,0x03,	// 67
	0x36,0x49,0x49,0x49,0x36, 0x06,0x49,0x49,0x29,0x1e,	// 89
	0x00,0x36,0x36,0x00,0x00, 0x00,0x56,0x36,0x00,0x00,	// :;
	0x08,0x14,0x22,0x41,0x00, 0x14,0x14,0x14,0x14,0x14,	// <=
	0x00,0x41,0x22,0x14,0x08, 0x02,0x01,0x51,0x09,0x06,	// >?
	0x32,0x49,0x79,0x41,0x3e, 0x7e,0x11,0x11,0x11,0x7e,	// @A
	0x7f,0x49,0x49,0x49,0x36, 0x3e,0x41,0x41,0x41,0x22,	// BC
	0x7f,0x41,0x41,0x22,0x1c, 0x7f,0x49,0x49,0x49,0x41,	// DE
	0x7f,0x09,0x09,0x09,0x01, 0x3e,0x41,0x49,0x49,0x7a,	// FG
	0x7f,0x08,0x08,0x08,0x7f, 0x00,0x41,0x7f,0x41,0x00,	// HI
	0x20,0x40,0x41,0x3f,0x01, 0x7f,0x08,0x14,0x22,0x41,	// JK
	0x7f,0x40,0x40,0x40,0x40, 0x7f,0x02,0x0c,0x02,0x7f,	// LM
	0x7f,0x04,0x08,0x10,0x7f, 0x3e,0x41,0x41,0x41,0x3e,	// NO
	0x7f,0x09,0x09,0x09,0x06, 0x3e,0x41,0x51,0x21,0x5e,	// PQ
	0x7f,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31,	// RS
	0x01,0x01,0x7f,0x01,0x01, 0x3f,0x40,0x40,0x40,0x3f,	// TU
	0x1f,0x20,0x40,0x20,0x1f, 0x3f,0x40,0x38,0x40,0x3f,	// VW
	0x63,0x14,0x08,0x14,0x63, 0x07,0x08,0x70,0x08,0x07,	// XY
	0x61,0x51,0x49,0x45,0x43, 0x00,0x7f,0x41,0x41,0x00,	// Z[
	0x02,0x04,0x08,0x10,0x20, 0x00,0x41,0x41,0x7f,0x00,	// \]
	0x04,0x02,0x01,0x02,0x04, 0x40,0x40,0x40,0x40,0x40,	// ^_
	0x00,0x01,0x02,0x04,0x00, 0x20,0x54,0x54,0x54,0x78,	// `a
	0x7f,0x48,0x44,0x44,0x38, 0x38,0x44,0x44,0x44,0x20,	// bc
	0x38,0x44,0x44,0x48,0x7f, 0x38,0x54,0x54,0x54,0x18,	// de
	0x08,0x7e,0x09,0x01,0x02, 0x0c,0x52,0x52,0x52,0x3e,	// fg
	0x7f,0x08,0x04,0x04,0x78, 0x00,0x44,0x7d,0x40,0x00,	// hi
	0x20,0x40,0x44,0x3d,0x00, 0x7f,0x10,0x28,0x44,0x00,	// jk
	0x00,0x41,0x7f,0x40,0x00, 0x7c,0x04,0x18,0x04,0x78,	// lm
	0x7c,0x08,0x04,0x04,0x78, 0x38,0x44,0x44,0x44,0x38,	// no
	0x7c,0x14,0x14,0x14,0x08, 0x08,0x14,0x14,0x18,0x7c,	// pq
	0x7c,0x08,0x04,0x04,0x08, 0x48,0x54,0x54,0x54,0x20,	// rs
	0x04,0x3f,0x44,0x40,0x20, 0x3c,0x40,0x40,0x20,0x7c,	// tu
	0x1c,0x20,0x40,0x20,0x1c, 0x3c,0x40,0x30,0x40,0x3c,	// vw
	0x44,0x28,0x10,0x28,0x44, 0x0c,0x50,0x50,0x50,0x3c,	// xy
	0x44,0x64,0x54,0x4c,0x44 				            // z
};


//============================================================================================================================
//======================================================= Pin Declerations ===================================================
//============================================================================================================================
// Latch Data Pins
const GPIO_pin_t Pin_MCU_Data0		= DP13;
const GPIO_pin_t Pin_MCU_Data1		= DP12;
const GPIO_pin_t Pin_MCU_Data2		= DP11;
const GPIO_pin_t Pin_MCU_Data3		= DP10;
const GPIO_pin_t Pin_MCU_Data4		= DP9;
const GPIO_pin_t Pin_MCU_Data5		= DP8;
const GPIO_pin_t Pin_MCU_Data6		= DP7;
const GPIO_pin_t Pin_MCU_Data7		= DP6;
// Multiplexer Addy Pins
const GPIO_pin_t Pin_MCU_Addy0		= DP5;
const GPIO_pin_t Pin_MCU_Addy1		= DP4;
const GPIO_pin_t Pin_MCU_Addy2		= DP3;
// Chip select for Latch's
const GPIO_pin_t Pin_MCU_CS			= DP2;
// Chip select for the Layer Power Latch
const GPIO_pin_t Pin_MCU_LayLatch	= DP14;
// 0.5 of +5V rail input
const GPIO_pin_t Pin_VRail			= DP15;
// Switches 
const GPIO_pin_t Pin_Switch1		= DP16;
const GPIO_pin_t Pin_Switch2		= DP17;

//Arrays to hold pin data so it can be looped through (THESE MAY NOT BE NEEDED, ARE HERE FOR THE MEAN TIME)
GPIO_pin_t PinDataArray[] = {
	Pin_MCU_Data0,
	Pin_MCU_Data1,
	Pin_MCU_Data2,
	Pin_MCU_Data3,
	Pin_MCU_Data4,
	Pin_MCU_Data5,
	Pin_MCU_Data6,
	Pin_MCU_Data7
};

GPIO_pin_t PinAddyArray[] = {
	Pin_MCU_Addy0,
	Pin_MCU_Addy1,
	Pin_MCU_Addy2
};

//============================================================================================================================
//======================================================== Global Variables ==================================================
//============================================================================================================================

volatile uint8_t layer = 1;
volatile uint8_t tempUInt = 0;

// Text storage array
volatile unsigned char textPath[TEXTPATHLENGTH] = {0};

// Main cube data array [Z][X][Y]
bool cubeData[8][8][8] = {false};


//============================================================================================================================
//============================================================ Setup() =======================================================
//============================================================================================================================

void setup()
{	
	int i = 0;
	
	// Setup Inputs
	for (i = 0; i < 8; i++)
	{
		pinMode2f(PinDataArray[i], OUTPUT);
	}
	for (i = 0; i < 3; i++)
	{
		pinMode2f(PinAddyArray[i], OUTPUT);
	}
	pinMode2f(Pin_MCU_CS, OUTPUT);
	pinMode2f(Pin_MCU_LayLatch, OUTPUT);
	
	//Setup Outputs
	pinMode2f(Pin_Switch1, INPUT_PULLUP);
	pinMode2f(Pin_Switch2, INPUT_PULLUP);
	pinMode2f(Pin_VRail, INPUT);
	
	// Start LED cube going by starting timer 1
	//62500 - 0.25Hz
	//15625 - 1Hz
	//3906 - 4Hz
	//1953 - 8Hz
	//977 - 16Hz
	//781 - 20Hz
	//500 - 31.25Hz
	//391 - 40Hz
	Setup_Timer1(40);
}


//============================================================================================================================
//============================================================= Loop() =======================================================
//============================================================================================================================

void loop()
{
	//do stuff
	
}


//============================================================================================================================
//============================================================ Set I/O =======================================================
//============================================================================================================================

void SetMultiplexerAddy(uint8_t addy)
{
	digitalWrite2f(Pin_MCU_Addy0, HIGH && (addy & 0x01));
	digitalWrite2f(Pin_MCU_Addy1, HIGH && (addy & 0x02));
	digitalWrite2f(Pin_MCU_Addy2, HIGH && (addy & 0x04));
}

void SetDataIO(uint8_t Ldata)
{
	digitalWrite2f(Pin_MCU_Data0, !(HIGH && (Ldata & 0x01)));
	digitalWrite2f(Pin_MCU_Data1, !(HIGH && (Ldata & 0x02)));
	digitalWrite2f(Pin_MCU_Data2, !(HIGH && (Ldata & 0x04)));
	digitalWrite2f(Pin_MCU_Data3, !(HIGH && (Ldata & 0x08)));
	digitalWrite2f(Pin_MCU_Data4, !(HIGH && (Ldata & 0x10)));
	digitalWrite2f(Pin_MCU_Data5, !(HIGH && (Ldata & 0x20)));
	digitalWrite2f(Pin_MCU_Data6, !(HIGH && (Ldata & 0x40)));
	digitalWrite2f(Pin_MCU_Data7, !(HIGH && (Ldata & 0x80)));
}


//============================================================================================================================
//======================================================== Set data Latches ==================================================
//============================================================================================================================

void LatchCubeData(uint8_t multiplexer, uint8_t data)
{
	SetMultiplexerAddy(multiplexer);
	digitalWrite2f(Pin_MCU_CS, HIGH);
	SetDataIO(data);
	digitalWrite2f(Pin_MCU_CS, LOW);
}

void LatchLayerPower(uint8_t data)
{
	digitalWrite2f(Pin_MCU_LayLatch, HIGH);
	SetDataIO(data); 
	digitalWrite2f(Pin_MCU_LayLatch, LOW);
}


//============================================================================================================================
//=================================================== Timer 1 / cube update func =============================================
//============================================================================================================================

void Setup_Timer1(uint16_t reloadVal)
{
	//Clear Timer 1
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCCR1C = 0x00;
	
	if (reloadVal == 0) { return; }
	
	//
	TCCR1B |= (1 << WGM12);
	OCR1A = reloadVal;
	TIMSK1 |= (1 << TOIE1);
	TCCR1B |= (1 << CS12) | (CS10);
}

// Cube Update ISR
// This controls the persistence of vision 
ISR(TIMER1_OVF_vect)
{
	// Turn All Layers OFF
	LatchLayerPower(0);
	
	// Increment to the next layer
	if (layer == 0x80)
	{
		layer = 0x01;
	}
	else
	{
		layer << 1;
	}
	
	// Pull the row data and set the latches for each row
	for (int i = 0; i < CUBESIZE + 1; i++)
	{
		//pull out cube data and set multiplexer
		tempUInt = CubeRowToUInt(layer, i);
		LatchCubeData(layer, tempUInt);
	}
	
	// Turn the power ON to the newly set layer
	LatchLayerPower(layer);
}

/*
---Cord sys:---

Z Y
|/
:--X

cubeData[Z][X][Y]
*/
uint8_t CubeRowToUInt(uint8_t layer, uint8_t row)
{
	uint8_t returnVal = 0;
	
	if(cubeData[layer][0][row]){returnVal += 1;}
	if(cubeData[layer][1][row]){returnVal += 2;}
	if(cubeData[layer][2][row]){returnVal += 4;}
	if(cubeData[layer][3][row]){returnVal += 8;}
	if(cubeData[layer][4][row]){returnVal += 16;}
	if(cubeData[layer][5][row]){returnVal += 32;}
	if(cubeData[layer][6][row]){returnVal += 64;}
	if(cubeData[layer][7][row]){returnVal += 128;}
	
	return returnVal;
}


//============================================================================================================================
//================================================ Background Functions for Effects ==========================================
//============================================================================================================================

#pragma region Background Functions for effects
bool InRange(uint8_t Z, uint8_t X, uint8_t Y)
{
		if ((Z >= 0) && (Z < CUBESIZE) && (X >= 0) && (X < CUBESIZE) && (Y >= 0) && (Y < CUBESIZE))
		{
			return true;
		}
		else
		{
			return false;
		}
}

void  SetPixel(uint8_t Z, uint8_t X, uint8_t Y, bool level)
{
	if (InRange(Z,X,Y))
	{
		cubeData[Z][X][Y] = level;
	}
}

bool GetPixel(uint8_t Z, uint8_t X, uint8_t Y)
{
	return cubeData[Z][X][Y];
}

// Set a line
/*
Z Y
|/
:--X

ZAXIS - (0)


*/
void SetLine(uint8_t axis, uint8_t layer, uint8_t pos, uint8_t data)
{
	bool tempBool = false;
	
	for (int i = 0; i < CUBESIZE; i++)
	{
		if (axis == ZAXIS)
		{
			tempBool = true && (data & 0x80);
			SetPixel(i, layer, pos, tempBool);
			data >> 1;
		}
		
		if (axis == XAXIS)
		{
			tempBool = true && (data & 0x80);
			SetPixel(layer, i, pos, tempBool);
			data >> 1;
		}
		
		if (axis == YAXIS)
		{
			tempBool = true && (data & 0x80);
			SetPixel(layer, pos, i, tempBool);
			data >> 1;
		}
	}
}

/*
Z Y
|/
:--X

cubeData[Z][X][Y]
*/
void SetCubeLayer(uint8_t axis, uint8_t layer, bool level)
{
	for (int i = 0; i < CUBESIZE; i++)
	{
		for (int j = 0; j < CUBESIZE; j++)
		{
			if (axis == ZAXIS)
			{
				SetPixel(layer, i, j, level);
			}
			
			if (axis == XAXIS)
			{
				SetPixel(i, layer, j, level);
			}
			
			if (axis == YAXIS)
			{
				SetPixel(i, j, layer, level);
			}
		}
	}
}

//
void SetAll()
{
	for(int i = 0; i < CUBESIZE ; i++)
	{
		SetCubeLayer(ZAXIS, i, PIXELON);
	}
}

//
void ClearAll()
{
		for(int i = 0; i < CUBESIZE ; i++)
		{
			SetCubeLayer(ZAXIS, i, PIXELOFF);
		}
}

// shift all the data by 1 position along the selected axis
// in the specified direction
//
// dir:
// true (1) 7 -> 0 (down)
// false (0) 0 -> 7 (up)
void ShiftLayerData(uint8_t axis, bool dir)
{
	bool tempPix = false;
	uint8_t layer1;
	uint8_t layer2;
	
	for (int i = 0; i < CUBESIZE; i++)
	{
		if (dir)
		{
			layer1 = i;
		} 
		else
		{
			layer1 = (7 - i);
		}
		
		for (int j = 0; j < CUBESIZE; j++)
		{
			if (dir)
			{
				layer2 = layer1 - 1;
			} 
			else
			{
				layer2 = layer1 + 1;
			}
			
			for (int k = 0; k < CUBESIZE; k++)
			{
				if (axis == ZAXIS)
				{
					tempPix = GetPixel(layer1, j, k);
					SetPixel(layer2, j, k, tempPix);
				}
				
				if (axis == XAXIS)
				{
					tempPix = GetPixel(j, layer1, k);
					SetPixel(j, layer2, k, tempPix);
				}
				
				if (axis == YAXIS)
				{
					tempPix = GetPixel(j, k, layer1);
					SetPixel(j, k, layer2, tempPix);
				}
			}
		}
	}
}

//
void CheckArgOrder(int in1, int in2, int *out1, int *out2)
{
	if (in1 > in2)
	{
		int tempVal = in1;
		in1 = in2;
		in2 = tempVal;
	}
	
	*out1 = in1;
	*out2 = in2;
}

//
void DrawWireframe(int ZPos1, int XPos1, int YPos1, int ZPos2, int XPos2, int YPos2)
{
	int i = 0;
	
	//
	CheckArgOrder(XPos1, XPos2, &XPos1, &XPos2);
	CheckArgOrder(YPos1, YPos2, &YPos1, &YPos2);
	CheckArgOrder(ZPos1, ZPos2, &ZPos1, &ZPos2);
	
	for (i = 0; i < CUBESIZE; i++)
	{
		//SetPixel();
	}
}

//
void GetCharPattern(char chr, unsigned char rtnChr[5])
{
	chr -= 32;
	int addr = (int)chr * 5;
	int z = 0;
	
	for (int x = addr ; x < addr+5 ; x++)
	{
		rtnChr[z] = chracterStorageArray[x];
		z++;
	}
}

//
void ResetTextPath()
{
	for (int i = 0 ; i < TEXTPATHLENGTH ; i ++)
	{
		textPath[i] = 0;
	}
}

//
void AddChrToPath(unsigned char inputChr, int pos)
{
	textPath[pos] = inputChr;
}

//
void IncrementPath()
{
	for (int i = TEXTPATHLENGTH - 2 ; i >= 0 ; i--)
	{
		textPath[i+1] = textPath[i];
	}
	textPath[0] = 0;
}

/*
	07-15-23-31-39-47-55-63  | Start
	06-14-22-30-38-46-54-62  |
	05-13-21-29-37-45-53-61  |
	04-12-20-28-36-44-52-60  \/
	03-11-19-27-35-43-51-59
	02-10-18-26-34-42-50-58
	01-09-17-25-33-41-49-57
	00-08-16-24-32-40-48-56
			Front
	
	
	---Cord sys:---
	
	Z Y
	|/
	:--X
	
	//Main Data Array
	cubeData[Z][X][Y]
	
	SetLine(uint8_t axis, uint8_t layer, uint8_t pos, uint8_t data)
*/
void AddPathToCube()
{
	SetLine(ZAXIS,7,7,textPath[0]);//63
	SetLine(ZAXIS,7,6,textPath[1]);//62
	SetLine(ZAXIS,7,5,textPath[2]);//61
	SetLine(ZAXIS,7,4,textPath[3]);//60
	SetLine(ZAXIS,7,3,textPath[4]);//59
	SetLine(ZAXIS,7,2,textPath[5]);//58
	SetLine(ZAXIS,7,1,textPath[6]);//57
	SetLine(ZAXIS,7,0,textPath[7]);//56
	
	SetLine(ZAXIS,6,0,textPath[8]);//48
	SetLine(ZAXIS,5,0,textPath[9]);//40
	SetLine(ZAXIS,4,0,textPath[10]);//40
	SetLine(ZAXIS,3,0,textPath[11]);//32
	SetLine(ZAXIS,2,0,textPath[12]);//24
	SetLine(ZAXIS,1,0,textPath[13]);//16
	
	
	SetLine(ZAXIS,0,0,textPath[14]);//00
	SetLine(ZAXIS,0,1,textPath[15]);//01
	SetLine(ZAXIS,0,2,textPath[16]);//02
	SetLine(ZAXIS,0,3,textPath[17]);//03
	SetLine(ZAXIS,0,4,textPath[18]);//04
	SetLine(ZAXIS,0,5,textPath[19]);//05
	SetLine(ZAXIS,0,6,textPath[20]);//06
	SetLine(ZAXIS,0,7,textPath[21]);//07
	
	SetLine(ZAXIS,1,7,textPath[22]);//15
	SetLine(ZAXIS,2,7,textPath[23]);//23
	SetLine(ZAXIS,3,7,textPath[24]);//31
	SetLine(ZAXIS,4,7,textPath[25]);//39
	SetLine(ZAXIS,5,7,textPath[26]);//47
	SetLine(ZAXIS,6,7,textPath[27]);//55
}

void AddTextToCubeLayer(char inputChar, int axis)
{
	unsigned char chrPattern[5] = {0};
	GetCharPattern(inputChar, chrPattern);
	
	int loopCount = 0;
	
	switch(axis)
	{
		// Z-axis
		case ZAXIS:
			for (loopCount = 0 ; loopCount < 5 ; loopCount++)
			{
				SetLine(2, 0, loopCount + 1, chrPattern[loopCount]);
			}
			break;
		
		//X-axis
		case XAXIS:
			for (loopCount = 0 ; loopCount < 5 ; loopCount++)
			{
				SetLine(0, 0, loopCount +1 , chrPattern[ 4 - loopCount]);
			}
			break;
		
		//Y-axis
		case YAXIS:
			for (loopCount = 0 ; loopCount < 5 ; loopCount++)
			{
				SetLine(0, loopCount + 1, 0, chrPattern[loopCount]);
			}
			break;
		
		default:
			//FUUUCCCCKKKKK
			break;
	}
}
#pragma endregion


//============================================================================================================================
//====================================================== Main Effect Functions ===============================================
//============================================================================================================================

void Effect_Rain(uint16_t iterations, uint16_t iterationDelay)
{
	uint8_t xx = 0;
	uint8_t yy = 0;
	
	// Clear the cube data
	ClearAll();
	
	for (int i = 0; i < iterations; i++)
	{
		// Shift all the data down the cube by 1 along the Z axis
		ShiftLayerData(ZAXIS, DIRDOWN);
		
		// Clear the top layer
		SetCubeLayer(ZAXIS, 7, PIXELOFF);
		
		// Generate a random number of 'rain' drops (10-20)
		uint8_t tempVal = random(10,20);
		
		for (int j = 0; j < tempVal; j++)
		{
			xx = random(0, 8);
			yy = random(0, 8);
			
			SetPixel(7, xx, yy, PIXELON);
		}
		
		delay(iterationDelay);
	}
	
	for (int i = 0; i < CUBESIZE; i++)
	{
		ShiftLayerData(ZAXIS, DIRDOWN);
	}
}

//
void Effect_TextScroll(uint16_t iterations, String inputString, uint16_t scrollDelay)
{
	// Clear the cube data
	ClearAll();
	
	String inpStr = inputString;
	uint8_t strLength = inpStr.length();
	
	for (int i = 0; i < iterations; i++)
	{
		for (int j = 0; j < strLength; j++)
		{
			char currentChr = inpStr[j];
			unsigned char chrPattern[5] = {0};
			
			// Get the pattern of the current char
			GetCharPattern(currentChr, chrPattern);
			
			for (int k = 0; k < 5; k++)
			{
				AddChrToPath(chrPattern[k], 0);
				AddPathToCube();
				delay(scrollDelay);
				IncrementPath();
				AddPathToCube();//?
			}
			
			// Add a space between Chars
			delay(scrollDelay);
			IncrementPath();
			AddPathToCube();
		}
		
		for (int k = 0; k < TEXTPATHLENGTH - 4; k++)
		{
			delay(scrollDelay);
			IncrementPath();
			AddPathToCube();
		}
	}
}