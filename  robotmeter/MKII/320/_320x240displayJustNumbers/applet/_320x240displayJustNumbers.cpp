/*COLOR mwhite ={255,255,255};
COLOR mblack ={1,1,1};*/

//simple sketch to check sensor functionality 

#include "WProgram.h"
void setup ();
void loop();
float getAValue();
float boost = 0;
float oilT = 0;
float temp1 = 0;
float temp2 = 0;
float accely = 0;
float accelx = 0;

POINT m_point;

void setup (){
  //communications with arduino basics
  Serial.begin(9600); 
  delay(3000);
  Serial.print('U');
  //canvas stuff
  //beginCanvas();
  background(0,0,0); 
  stroke(255,255,255);
  fill(255,255,255); 
}

void loop() {
  background(0,0,0); //blank the screen black
  //labels
  text("meter demo (work in progress disregard any readings!)", 0, 5);
  text("  temperature 1: ", 18, 58);
  text("  temperature 2: ", 18, 78);
  text(" boost pressure: ", 18, 18);
  text("oil temperature: ", 18, 38);
 
  text("accelerometer x: ", 18, 98);
  
  while (!(touch_get_cursor(&m_point))){
    Serial.print('F'); //request for data block
    temp1 = getAValue();
    temp2 = getAValue();
    boost = getAValue();
    oilT = getAValue(); 
    Serial.print('X'); //request a data block
    accelx = getAValue();
    //display data here
    text((float)temp1, 120, 58);
    text((float)temp2, 120, 78);
    text((float)boost, 120, 18);
    text((float)oilT, 120, 38);
    text((float)accelx, 120, 98);

    delay(100); //delay so things are readable
  }
  while (touch_get_cursor(&m_point)){}
  background(0,0,0);
  bmp_draw("mockbig",0,0);
  
  //wait until screen is touched
  while(!touch_get_cursor(&m_point)){}
  while(touch_get_cursor(&m_point)){}
}

float getAValue(){
  //while(Serial.available()){}
  int getValue = Serial.read();
  getValue = (getValue << 8) + Serial.read();
  float getValueF = (float)getValue / 10;
  //Serial.print('C');
  delay(20);
  //while(Serial.read()!='C'){}
  return getValueF;
}

//*******************************************************************************
//*	Detailed edit history
//*	<MLS> is Mark Sproul msproul@jove.rutgers.edu
//*			http://coewww.rutgers.edu/~msproul/
//*	Detailed edit history
//*******************************************************************************
//*	Dec 26,	2008	Mark Sproul <MLS> meet with Matt and Chris in New Haven
//*	Dec 26,	2008	<MLS> Will be taking over the development of the SubProcessing files
//*******************************************************************************

#include	"HardwareDef.h"
#include	"SubPOptions.h"
#include	"SubPGraphics.h"
#include	"graphics.h"

//*	if you need more code space in the STELTH or SLIDE
//*	comment out one or more of these lines to get rid of the start up splash
#define	_STARTUPSCREEN_VERSION_
//#define	_STARTUPSCREEN_LIQUIDWARE_
//#define	_STARTUPSCREEN_MATRIX_

//#define	_DEBUG_RECTS_

void	DisplaySplashScreen(void);



//*******************************************************************************
int main(void)
{


	init();

	DisplaySplashScreen();
	
	setup();
    
	for (;;)
		loop();
        
	return 0;
}

short	gTextYloc	=	45;
//*******************************************************************************
void	DebugText(char *textMsg)
{

	text(textMsg, 5, gTextYloc);
	gTextYloc	+=	15;
	if (gTextYloc > kSCREEN_Y_size)
	{
		gTextYloc	=	15;
	}
}

#ifdef _STARTUPSCREEN_LIQUIDWARE_
//*********************************************************
//*	Dec 26,	2008	Run Length Encoding for RGB
void	DisplayRLE_RGB(unsigned char *rleBuff, COLOR *colorFade, boolean fillBack, int startY)
{
COLOR	bgColor;
COLOR	rleColor;
int		ii, jj;
int		cc;
int		rlePixCount;
int		pixelX, pixelY;
int		rleColorValue;
int		pixlesWide, pixelsTall;
int		rowCount;
int		byte1, byte2, byte3, byte4;
int		adjusted_Red;
int		adjusted_Green;
int		adjusted_Blue;

	cc		=	0;
	
	byte1			=	rleBuff[cc++] & 0x00ff;
	byte2			=	rleBuff[cc++] & 0x00ff;
	byte3			=	rleBuff[cc++] & 0x00ff;
	byte4			=	rleBuff[cc++] & 0x00ff;
	pixlesWide		=	(byte1 << 8) + byte2;
	pixelsTall		=	(byte3 << 8) + byte4;


	adjusted_Red	=	(rleBuff[cc++] & 0x00ff)		- colorFade->red;
	adjusted_Green	=	(rleBuff[cc++] & 0x00ff)	- colorFade->green;
	adjusted_Blue	=	(rleBuff[cc++] & 0x00ff)	- colorFade->blue;
	cc++;			//*	1 filler char

	if (adjusted_Red < 0)	adjusted_Red	=	0;
	if (adjusted_Green < 0)	adjusted_Green	=	0;
	if (adjusted_Blue < 0)	adjusted_Blue	=	0;

	bgColor.red		=	adjusted_Red;
	bgColor.green	=	adjusted_Green;
	bgColor.blue	=	adjusted_Blue;


	pixelX		=	(kSCREEN_X_size / 2) - (pixlesWide / 2);
	pixelY		=	(kSCREEN_Y_size / 2) - (pixelsTall / 2);

	//*	is the background supposed to be filled in
	if (fillBack)
	{
   		dispColor(bgColor);
		for (jj=startY; jj<pixelY; jj++)
		{
			for (ii=0; ii<=kSCREEN_X_size; ii++)
			{
				dispPixel(ii, jj);
			}
		}
	}

	rowCount	=	0;
	
	while ((cc < 5000) && (rowCount < pixelsTall))
	{
		adjusted_Red	=	(rleBuff[cc] & 0x00ff)		- colorFade->red;
		adjusted_Green	=	(rleBuff[cc + 1] & 0x00ff)	- colorFade->green;
		adjusted_Blue	=	(rleBuff[cc + 2] & 0x00ff)	- colorFade->blue;
		if (adjusted_Red < 0)	adjusted_Red	=	0;
		if (adjusted_Green < 0)	adjusted_Green	=	0;
		if (adjusted_Blue < 0)	adjusted_Blue	=	0;

		rleColor.red	=	adjusted_Red;
		rleColor.green	=	adjusted_Green;
		rleColor.blue	=	adjusted_Blue;
		
		rlePixCount		=	rleBuff[cc + 3] & 0x00ff;

		if ((rleColor.red == 0) && (rleColor.green == 0) && (rleColor.blue == 0) && (rlePixCount == 0))
		{
			//*	we have a new line
			rowCount++;
			pixelX	=	(kSCREEN_X_size / 2) - (pixlesWide / 2);

			//*	as good a time as any to fill in the rest of the row
			if (fillBack)
			{
 		  		dispColor(bgColor);
				for (ii=0; ii<pixelX; ii++)
				{
					dispPixel(ii, pixelY);
				}
				for (ii=(pixelX + pixlesWide); ii<kSCREEN_X_size; ii++)
				{
					dispPixel(ii, pixelY);
				}
			}

			pixelY++;
		}
		else if ((rlePixCount > 0) && (rlePixCount < 256))
		{
	   			
   			dispColor(rleColor);
		
			for (jj=0; jj<rlePixCount; jj++)
			{
				dispPixel(pixelX, pixelY);
				pixelX++;
			}
		}
		cc	+=	4;
	}
	if (fillBack)
	{
   		dispColor(bgColor);
		for (jj=pixelY; jj<=kSCREEN_Y_size; jj++)
		{
			for (ii=0; ii<=kSCREEN_X_size; ii++)
			{
				dispPixel(ii, jj);
			}
		}
	}
}
#endif


#ifdef _STARTUPSCREEN_MATRIX_
//*******************************************************************************
void	MatrixDisplay(int topOffset, int iterations)
{
//int		ii,jj;
int		cc;
int		xx, yy;
char	myChar;
char	myString[4];

	
	fill(0);
	
	cc	=	0;
	while (cc < iterations)
	{
		//*	this gives 1 pixel between columns
		xx	=	random(kSCREEN_X_size / 12) * 13;
		yy	=	topOffset;
		while (yy < kSCREEN_Y_size)
		{
			stroke(0, 50 + random(200), 0);
			myChar	=	random(33, 72);
			drawchar(xx, yy, myChar);
		
		//	myString[0]	=	myChar;
		//	myString[1]	=	0;
		//	drawstring(xx, yy, myString);

			yy	+=	10;
		}
		cc++;
	}
}
#endif

#define	kLinrSpacing	11
#define	kMatixTopOffset	36
extern	unsigned char	gLiquidWareLogo[];

//*******************************************************************************
void	DisplaySplashScreen(void)
{
COLOR	bgColor;
COLOR	fontColor;
int		ii;
int		yTextLoc;

#ifdef _STARTUPSCREEN_VERSION_
	char	startupMsg[128];
#endif

#ifdef _STARTUPSCREEN_VERSION_
	bgColor.red		=	0;
	bgColor.green	=	0;
	bgColor.blue	=	0;

	fontColor.red	=	0;
	fontColor.green	=	255;
	fontColor.blue	=	0;
	
	//*	display the overall library version
	yTextLoc	=	10;
	strcpy(startupMsg, kDisplayHardwareString);
	strcat(startupMsg, " ");
	strcat(startupMsg, kDisplayHardwareVersion);
	dispPutS(startupMsg, 5, yTextLoc, fontColor, bgColor);
	yTextLoc	+=	kLinrSpacing;
	
	
	//*	display the SubProcessing library version
	strcpy(startupMsg, "Arduino Procssing Library ");
	strcat(startupMsg, kSubP_VersionString);
	strcat(startupMsg, " ");

#ifdef _SUBP_OPTION_GAMES_
	strcat(startupMsg, "+G");
#endif
#ifdef _SUBP_OPTION_KEYBOARD_
	strcat(startupMsg, "+K");
#endif

	dispPutS(startupMsg, 5, yTextLoc, fontColor, bgColor);
	yTextLoc	+=	kLinrSpacing;

#endif

#ifdef _DEBUG_RECTS_
 	dispColor(bgColor);
 	ii	=	kSCREEN_Y_size / 2;
 	ii	-=	25;
 	ii	=	kSCREEN_Y_size / 3;
 	while (ii > 30)
 	{
		fill(random(255), random(255), random(255));
		stroke(random(255), random(255), random(255));
		drawrect((kSCREEN_X_size / 2) - ii, (kSCREEN_Y_size / 2) - ii, (ii * 2), (ii * 2));
		
		ii	-=	10;
 	}
	fill(0);
	stroke(255);
#endif

#ifdef _STARTUPSCREEN_LIQUIDWARE_
   	for (ii=0; ii<190; ii+=10)
	{
		//*	this is the FADE color, how much to subtract from the actual colors
		//*	0 means none.
		bgColor.red		=	ii;
		bgColor.green	=	ii;
		bgColor.blue	=	ii;
		DisplayRLE_RGB(gLiquidWareLogo, &bgColor, false, kMatixTopOffset);
#ifdef _STARTUPSCREEN_MATRIX_
		if (ii > 100)
		{
			MatrixDisplay(kMatixTopOffset, 3);
		}
#endif
	}
#endif
#ifdef _STARTUPSCREEN_MATRIX_
	ii	=	0;
	while(!serialAvailable() && (ii < 2000))
	{
		MatrixDisplay(kMatixTopOffset, 2);
		ii++;

		gettouch();
		if ((mouseX > 200) && (mouseY < 100))
		{
			break;
		}
	}
#endif



#ifdef _SUBP_OPTION_7_SEGMENT_dontDisplay
int	xx, yy;
//	dispClearScreen();

	xx	=	10;
	yy	=	50;
	for (ii=5; ii<30; ii += 4)
	{
		Display7SegmentString(xx, yy, "0123456789ABCDEF", ii);
		yy	+=	(ii * 2);
		yy	+=	8;
		if (ii > 230)
		{
			break;
		}
	}
	while (true)
	{
		//*	do nothing
	}
#endif



	bgColor.red		=	0;
	bgColor.green	=	0;
	bgColor.blue	=	0;
 	dispColor(bgColor);
//	drawrect(0, yTextLoc, 320, 240);
	
}



#ifdef _STARTUPSCREEN_LIQUIDWARE_
unsigned char	gLiquidWareLogo[]	=	{
0,75,0,75,	//xSize_hi, xSize_lo ,ySize_hi, ySize_lo
150,207,14,0,		//*	background color
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,37,  183,183,183,1,  150,207,14,37,  		0,0,0,0,
150,207,14,36,  183,183,183,3,  150,207,14,36,  		0,0,0,0,
150,207,14,35,  183,183,183,5,  150,207,14,35,  		0,0,0,0,
150,207,14,35,  183,183,183,5,  150,207,14,35,  		0,0,0,0,
150,207,14,34,  183,183,183,7,  150,207,14,34,  		0,0,0,0,
150,207,14,33,  183,183,183,9,  150,207,14,33,  		0,0,0,0,
150,207,14,33,  183,183,183,10,  150,207,14,32,  		0,0,0,0,
150,207,14,32,  183,183,183,12,  150,207,14,31,  		0,0,0,0,
150,207,14,31,  183,183,183,12,  0,0,0,3,  150,207,14,29,  		0,0,0,0,
150,207,14,31,  183,183,183,12,  0,0,0,4,  150,207,14,28,  		0,0,0,0,
150,207,14,30,  183,183,183,12,  0,0,0,6,  150,207,14,27,  		0,0,0,0,
150,207,14,29,  183,183,183,13,  0,0,0,7,  150,207,14,26,  		0,0,0,0,
150,207,14,29,  183,183,183,12,  0,0,0,8,  150,207,14,26,  		0,0,0,0,
150,207,14,28,  183,183,183,12,  182,181,177,1,  0,0,0,4,  4,7,0,1,  0,5,0,1,  0,0,0,3,  150,207,14,25,  		0,0,0,0,
150,207,14,27,  183,183,183,13,  0,0,0,7,  68,79,19,1,  0,0,0,3,  150,207,14,24,  		0,0,0,0,
150,207,14,27,  183,183,183,13,  0,0,0,1,  4,1,10,1,  0,0,0,2,  0,2,0,1,  0,7,0,1,  94,111,17,1,  150,207,14,1,  34,46,0,1,  0,0,0,3,  150,207,14,23,  		0,0,0,0,
150,207,14,26,  183,183,183,13,  0,0,0,6,  26,39,0,1,  168,195,56,1,  150,207,14,1,  125,144,26,1,  1,13,0,1,  0,0,0,2,  150,207,14,23,  		0,0,0,0,
150,207,14,26,  183,183,183,13,  1,1,0,1,  0,0,0,1,  4,0,15,1,  0,0,0,1,  0,2,0,1,  0,0,0,1,  110,132,23,1,  150,207,14,3,  53,70,0,1,  0,0,0,2,  0,2,0,1,  150,207,14,22,  		0,0,0,0,
150,207,14,25,  183,183,183,13,  0,0,0,1,  1,0,0,1,  1,0,4,1,  0,0,0,3,  43,60,0,1,  150,207,14,5,  0,0,0,3,  150,207,14,22,  		0,0,0,0,
150,207,14,25,  183,183,183,12,  0,11,0,1,  1,0,0,1,  0,0,0,5,  128,154,19,1,  150,207,14,5,  60,79,0,1,  0,0,0,3,  150,207,14,21,  		0,0,0,0,
150,207,14,24,  183,183,183,13,  0,0,0,6,  60,78,2,1,  150,207,14,7,  0,0,0,1,  0,11,0,2,  150,207,14,21,  		0,0,0,0,
150,207,14,24,  183,183,183,12,  0,0,0,3,  1,0,5,1,  0,0,0,2,  3,18,0,1,  134,161,46,1,  150,207,14,7,  74,98,0,1,  0,10,0,1,  0,11,0,1,  0,0,0,1,  150,207,14,20,  		0,0,0,0,
150,207,14,23,  183,183,183,13,  0,0,0,6,  57,82,0,1,  150,207,14,9,  0,0,0,1,  0,5,0,1,  0,0,0,1,  150,207,14,20,  		0,0,0,0,
150,207,14,23,  183,183,183,12,  0,0,0,6,  0,19,0,1,  150,207,14,10,  79,103,9,1,  0,9,0,1,  0,11,0,1,  0,0,0,1,  150,207,14,19,  		0,0,0,0,
150,207,14,22,  183,183,183,13,  0,0,0,6,  56,80,0,1,  150,207,14,3,  158,210,3,1,  150,207,14,7,  0,0,0,3,  150,207,14,19,  		0,0,0,0,
150,207,14,22,  183,183,183,12,  0,0,0,5,  0,7,0,1,  0,0,0,1,  130,162,29,1,  150,207,14,11,  79,104,2,1,  0,8,0,1,  0,0,0,2,  150,207,14,18,  		0,0,0,0,
150,207,14,21,  183,183,183,13,  0,11,0,2,  0,3,8,1,  0,0,5,1,  0,5,1,1,  0,13,0,1,  58,86,0,1,  150,207,14,7,  150,209,0,1,  150,207,14,1,  156,210,0,1,  150,207,14,3,  0,0,0,4,  150,207,14,17,  		0,0,0,0,
150,207,14,21,  183,183,183,12,  0,11,0,2,  0,2,0,1,  0,4,3,1,  0,2,1,1,  0,4,0,1,  0,14,0,1,  120,154,15,1,  150,207,14,2,  152,206,0,1,  150,207,14,7,  154,206,0,1,  150,207,14,3,  0,8,0,1,  0,0,0,2,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,3,  181,181,181,1,  183,183,183,9,  0,11,0,1,  0,0,0,2,  0,5,0,1,  0,4,0,1,  0,8,0,1,  43,65,0,1,  150,207,14,3,  153,209,0,1,  150,207,14,6,  151,207,10,1,  153,209,12,1,  150,207,14,3,  0,0,0,3,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,1,  181,181,181,1,  183,183,183,10,  0,0,0,1,  0,11,0,2,  0,0,0,4,  108,135,38,1,  150,207,14,5,  148,211,8,1,  147,212,4,1,  150,207,14,5,  151,210,6,1,  150,207,14,2,  50,85,0,1,  0,0,0,1,  0,2,0,1,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,12,  0,0,0,6,  15,39,0,1,  150,207,14,2,  153,205,8,1,  149,207,1,1,  148,208,0,1,  150,207,14,12,  0,11,0,1,  3,9,7,1,  0,11,0,1,  150,207,14,16,  		0,0,0,0,
150,207,14,19,  183,183,183,13,  0,0,0,2,  0,11,0,1,  0,0,0,1,  0,4,0,1,  0,0,0,1,  79,111,2,1,  150,207,14,17,  30,52,0,1,  0,6,0,1,  0,11,0,1,  150,207,14,16,  		0,0,0,0,
150,207,14,19,  183,183,183,2,  181,181,181,1,  183,183,183,9,  0,0,0,5,  0,5,0,1,  0,0,0,1,  138,179,41,1,  150,207,14,18,  0,9,0,1,  0,2,0,1,  0,11,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,19,  183,183,183,12,  0,0,0,3,  6,6,4,1,  0,0,0,2,  40,68,0,1,  156,203,47,1,  150,207,14,18,  0,23,0,1,  0,7,0,1,  0,11,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,18,  183,183,183,13,  0,0,0,6,  93,129,31,1,  148,201,33,1,  150,207,14,17,  147,194,52,1,  43,74,0,1,  0,9,0,1,  0,11,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,18,  183,183,183,12,  0,0,0,3,  1,0,5,1,  0,3,0,1,  0,0,0,2,  150,207,14,21,  0,11,0,1,  0,2,0,1,  150,207,14,15,  		0,0,0,0,
150,207,14,17,  183,183,183,13,  0,0,0,6,  31,67,0,1,  151,202,61,1,  150,207,14,9,  138,188,37,1,  150,207,14,10,  0,14,0,1,  0,2,0,1,  0,11,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,0,0,1,  1,0,5,1,  0,0,0,4,  60,101,0,1,  147,205,43,1,  150,207,14,8,  98,142,18,1,  38,81,0,1,  136,189,37,1,  150,207,14,9,  11,40,0,1,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,11,0,1,  0,0,4,1,  5,0,11,1,  0,0,5,1,  0,6,0,1,  0,0,0,1,  150,207,14,10,  38,73,0,1,  0,18,0,1,  90,133,18,1,  139,193,45,1,  150,207,14,8,  45,77,4,1,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,11,0,1,  1,1,3,1,  7,0,0,2,  0,0,0,2,  150,207,14,8,  141,197,26,1,  104,147,16,1,  0,0,0,1,  0,11,0,1,  0,0,0,1,  138,186,64,1,  150,207,14,8,  72,106,29,1,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  2,2,2,2,  1,0,0,1,  5,7,4,1,  0,0,0,2,  150,207,14,9,  25,59,0,1,  0,8,0,1,  0,0,0,2,  85,124,31,1,  150,207,14,9,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,0,0,2,  1,0,0,1,  7,9,4,1,  0,0,0,2,  150,207,14,8,  95,137,25,1,  0,18,0,1,  2,10,0,1,  0,2,2,1,  0,0,0,1,  7,40,0,1,  132,183,42,1,  150,207,14,8,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,0,0,3,  7,0,0,1,  0,0,0,2,  150,207,14,7,  134,195,32,1,  30,65,0,1,  0,14,0,1,  4,8,7,1,  0,0,0,1,  0,12,8,1,  0,14,0,1,  85,134,6,1,  133,196,21,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,0,0,2,  1,1,0,1,  0,4,0,1,  0,0,0,2,  150,207,14,7,  105,163,16,1,  0,28,0,1,  0,0,0,5,  31,71,0,1,  123,182,28,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,0,0,3,  3,5,0,1,  0,7,2,1,  0,0,0,1,  150,207,14,7,  61,117,0,1,  0,0,0,7,  112,166,52,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  7,0,0,2,  0,0,0,1,  7,0,0,1,  0,0,0,2,  150,207,14,7,  21,72,0,1,  0,0,0,7,  84,136,38,1,  150,207,14,7,  0,0,0,2,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,11,0,1,  1,1,1,1,  0,0,0,3,  0,10,0,1,  150,207,14,7,  9,55,0,1,  0,0,0,7,  51,99,0,1,  150,207,14,6,  3,31,0,1,  0,6,0,1,  0,0,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,15,  183,183,183,15,  0,11,0,1,  0,0,0,1,  0,1,0,1,  0,0,0,2,  0,8,0,1,  0,0,0,1,  150,207,14,6,  8,57,0,1,  0,0,0,7,  62,114,6,1,  150,207,14,6,  0,0,0,2,  0,4,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,16,  183,183,183,14,  0,11,0,2,  0,0,0,5,  150,207,14,6,  35,91,0,1,  7,0,0,1,  0,0,0,5,  0,20,0,1,  96,155,35,1,  150,207,14,5,  21,62,0,1,  0,0,0,1,  0,8,0,1,  0,5,0,1,  150,207,14,14,  		0,0,0,0,
150,207,14,17,  183,183,183,14,  0,11,0,1,  0,0,0,6,  150,207,14,5,  88,154,22,1,  22,77,0,1,  0,0,0,5,  22,77,0,1,  106,175,33,1,  150,207,14,5,  0,0,0,2,  0,5,2,1,  0,6,5,1,  150,207,14,14,  		0,0,0,0,
150,207,14,17,  183,183,183,14,  0,11,0,2,  0,0,0,5,  150,207,14,6,  71,135,13,1,  0,51,0,1,  0,29,0,1,  0,21,0,1,  0,25,0,1,  22,85,0,1,  98,172,23,1,  150,207,14,5,  0,0,0,1,  0,6,0,1,  0,0,0,3,  150,207,14,14,  		0,0,0,0,
150,207,14,17,  183,183,183,15,  0,11,0,2,  11,0,4,1,  0,0,0,4,  150,207,14,17,  0,8,0,1,  0,0,0,3,  150,207,14,15,  		0,0,0,0,
150,207,14,17,  183,183,183,15,  0,11,0,2,  4,0,2,1,  0,0,0,2,  7,6,12,1,  0,0,0,2,  150,207,14,14,  0,0,0,2,  0,3,2,1,  0,0,0,3,  150,207,14,15,  		0,0,0,0,
150,207,14,18,  183,183,183,14,  0,11,0,2,  4,4,2,1,  0,0,0,6,  0,17,0,1,  150,207,14,11,  0,0,0,5,  6,0,0,1,  150,207,14,16,  		0,0,0,0,
150,207,14,19,  183,183,183,14,  0,11,0,1,  0,0,0,5,  1,1,0,1,  0,0,0,18,  150,207,14,17,  		0,0,0,0,
150,207,14,19,  183,183,183,15,  0,11,0,3,  0,0,0,21,  150,207,14,17,  		0,0,0,0,
150,207,14,20,  183,183,183,16,  0,11,0,3,  3,0,0,1,  0,0,0,17,  150,207,14,18,  		0,0,0,0,
150,207,14,20,  183,183,183,18,  0,11,0,4,  0,4,0,1,  0,3,0,1,  0,0,0,8,  0,0,5,1,  0,0,0,2,  0,11,0,1,  150,207,14,19,  		0,0,0,0,
150,207,14,21,  183,183,183,21,  0,11,0,1,  0,0,0,12,  150,207,14,20,  		0,0,0,0,
150,207,14,22,  183,183,183,24,  0,0,0,8,  150,207,14,21,  		0,0,0,0,
150,207,14,22,  183,183,183,31,  150,207,14,22,  		0,0,0,0,
150,207,14,23,  183,183,183,29,  150,207,14,23,  		0,0,0,0,
150,207,14,24,  183,183,183,27,  150,207,14,24,  		0,0,0,0,
150,207,14,25,  183,183,183,24,  150,207,14,26,  		0,0,0,0,
150,207,14,26,  183,183,183,22,  150,207,14,27,  		0,0,0,0,
150,207,14,28,  183,183,183,19,  150,207,14,28,  		0,0,0,0,
150,207,14,34,  183,183,183,8,  150,207,14,33,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,
150,207,14,75,  		0,0,0,0,

//total values=716,  
//EOF
};

#endif

