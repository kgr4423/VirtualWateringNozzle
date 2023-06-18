#pragma once

#include"ControlModule.h"
#include"ftd2xx.h"

#define SC_BUF_SIZE 256

class SC02 : public ControlModule
{
public:

	SC02();
	~SC02();
    
	BOOL Open( BYTE bModuleID );
	BOOL Close( void );
	BOOL Reset( void );
	BOOL SetParam( PCM_PARAM pParam  );

	BYTE bComDeviceCount;

private:
	BOOL Transceive( BYTE bCommand, void* pTxBuffer, WORD wTxSize, 
											void* pRxBuffer, WORD wRxSize, BOOL RightNow=TRUE );
    BOOL Transfer( HANDLE hChildModule, void* pTxBuffer, WORD wTxSize, WORD wRxSize, BOOL RightNow );
	BOOL SetTimeout( DWORD dwTime_ms );

	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;

	BYTE TransferTxBuffer[SC_BUF_SIZE];
	BYTE TransferRxBuffer[SC_BUF_SIZE];
	int iTxBufCount; 
	int iRxBufCount; 
	ControlModule* pChildModule[256];

	DWORD  dwTimeout_ms;
	DWORD  dwNumberOfBytesRW; 

};



