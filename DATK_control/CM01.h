#pragma once

#include<windows.h>
#include"ControlModule.h"

// コマンド
#define CM01_GET_CONFIG		50
#define CM01_CLEAR_DATA		51
#define CM01_EXCHANGE		150

// 入力ボードID
#define CM01_ENC_BOARD 		0
#define CM01_SENSOR_BOARD	1

// 出力ボードID
#define CM01_PWM_BOARD 		0
#define CM01_DA_BOARD		1

#define CM01_PIO_BOARD		2

typedef struct _CM01_INPUT_ENC
{
	long lEncoder;
	WORD wSensor[5];
} CM01_INPUT_ENC;

#define CM01_INPUT_ENC_SIZE	14

typedef struct _CM01_INPUT_SENSOR
{
	WORD wSensor[8];
} CM01_INPUT_SENSOR;

#define CM01_INPUT_SENSOR_SIZE 16

typedef struct _CM01_OUTPUT_PWM
{
	short sPwm1;
	short sPwm2;

} CM01_OUTPUT_PWM;

#define CM01_OUTPUT_PWM_SIZE 4


typedef struct _CM01_OUTPUT_DA
{
	short sDA;
} CM01_OUTPUT_DA;

#define CM01_OUTPUT_DA_SIZE 2


typedef union _CM01_PIO
{
	struct{
		WORD wData;
		WORD wDir;
	} Word;
	struct{
		unsigned P0: 1;
		unsigned P1: 1;
		unsigned P2: 1;
		unsigned P3: 1;
		unsigned P4: 1;
		unsigned P5: 1;
		unsigned P6: 1;
		unsigned P7: 1;

		unsigned P8:  1;
		unsigned P9:  1;
		unsigned P10: 1;
		unsigned P11: 1;
		unsigned P12: 1;
		unsigned P13: 1;
		unsigned P14: 1;
		unsigned P15: 1;

		unsigned DIR0:  1;
		unsigned DIR1:  1;
		unsigned DIR2:  1;
		unsigned DIR3:  1;
		unsigned DIR4:  1;
		unsigned DIR5:  1;
		unsigned DIR6:  1;
		unsigned DIR7:  1;

		unsigned DIR8:  1;
		unsigned DIR9:  1;
		unsigned DIR10: 1;
		unsigned DIR11: 1;
		unsigned DIR12: 1;
		unsigned DIR13: 1;
		unsigned DIR14: 1;
		unsigned DIR15: 1;
	} Bit;

} CM01_PIO;

#define CM01_INPUT_PIO_SIZE		2
#define CM01_OUTPUT_PIO_SIZE	4


class CM01 : public ControlModule
{
public:
	CM01();
	~CM01();

	BOOL Open( ControlModule* pParent, BYTE bParentPort, BYTE bModuleID );
	BOOL GetConfig( void );
	BOOL Exchange( void* pOutputData, void* pInputData, BOOL RightNow=TRUE );
	BOOL Exchange( BOOL RightNow=TRUE );
	BOOL Receive( BYTE *pRxBuffer, WORD wSize );	
	BOOL ClearData( void );
	
	BYTE bBoardID_PA;
	BYTE bBoardID_PB;
	short sInputDataSize;
	short sOutputDataSize;

	short sInputDataSize_PA;
	short sOutputDataSize_PA;
	short sInputDataSize_PB;
	short sOutputDataSize_PB;

	CM01_INPUT_ENC EncBoard;
	CM01_INPUT_SENSOR SensorBoard;
	CM01_OUTPUT_PWM PwmBoard;
	CM01_OUTPUT_DA DaBoard;
	CM01_PIO PioBoardA;
	CM01_PIO PioBoardB;

	BOOL ExtMode;

};

