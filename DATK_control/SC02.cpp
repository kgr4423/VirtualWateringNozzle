#include"windows.h"
#include<stdio.h>
#include<conio.h>
#include<string.h>

#include "SC02.h"

SC02::SC02()
{
	ftHandle = NULL;
	bComDeviceCount = 0;
	iTxBufCount = 0;
	iRxBufCount = 0;

// resize
	if( TxBuffer )	delete TxBuffer;
	if( RxBuffer )	delete RxBuffer;

	TxBuffer = new BYTE[SC_BUF_SIZE];
	RxBuffer = new BYTE[SC_BUF_SIZE];

	ZeroMemory( TxBuffer, SC_BUF_SIZE );
	ZeroMemory( RxBuffer, SC_BUF_SIZE );
	
	dwTimeout_ms = 2000;

}

SC02::~SC02()
{
	if( ftHandle ) Close();
}

BOOL SC02::Open( BYTE bModuleID )
{
	FT_STATUS ftStatus;
	DWORD dwNumDevice;
	int i;

	//dbg_printf("Open Serial Hub ID %d: ", bTargetID );

	ftStatus = FT_ListDevices( &dwNumDevice, NULL, FT_LIST_NUMBER_ONLY );
	//dbg_printf( "FT device num %d\r\n", dwNumDevice );
	
	if( dwNumDevice == 0 ){
		sprintf_s(  Error, "No FT device found\r\n");
		return( FALSE );
	}
	
	for( i=0; i<(int)dwNumDevice; i++){
		ftStatus = FT_Open(i, &ftHandle);
		if( ftStatus == FT_OK ){
			if( bModuleID == CM_MASTER_ID ){
				SetTimeout( dwTimeout_ms );
				ftStatus = FT_SetLatencyTimer( ftHandle, 0 );
				return( TRUE );
			}
			SetTimeout( dwTimeout_ms );
			if( GetID() ){
				if( bModuleID == bID ){
					ftStatus = FT_SetLatencyTimer( ftHandle, 0 );
					return( TRUE );
				}
			}
		}
		FT_Close( ftHandle );

	}

	sprintf_s( Error, "Target ID device not found\r\n" );

	return( FALSE );
}


BOOL SC02::Close( void  )
{
	FT_Close( ftHandle );
	ftHandle = NULL;

	return( TRUE );
}


BOOL SC02::Transceive( BYTE bCommand, void* pTxBuffer, WORD wTxSize, void* pRxBuffer, WORD wRxSize, BOOL RightNow )
{
	WORD wSize;
	DWORD  dwNumberOfBytesRW;

	if( ftHandle ==NULL ) return( FALSE );

	wSize = 1+wTxSize; // Add Command

	//printf("Total Tx Size %d\r\n", wSize );

	TxBuffer[0] = bCommand;

	if( wTxSize != 0 ) memcpy( &TxBuffer[1], pTxBuffer, wTxSize );

	ftStatus = FT_Write( ftHandle, TxBuffer, wSize, &dwNumberOfBytesRW );
	if( ftStatus != FT_OK ){
		sprintf_s( Error, "Fail in FT_Write %d\r\n", ftStatus );
		return( FALSE );
	}

	wSize = wRxSize + 1; // Add status
	//printf("Total Rx size %d\r\n",wSize );

	ftStatus = FT_Read(  ftHandle,  RxBuffer, wSize, &dwNumberOfBytesRW );
	if( ftStatus != FT_OK ){
		sprintf_s( Error, "Fail in FT_Read %d\r\n", ftStatus );
		return( FALSE );
	}

	bStatus = RxBuffer[dwNumberOfBytesRW-1];

	if(wSize!=dwNumberOfBytesRW){
		if( bStatus != CM_NORMAL ){
			sprintf_s( Error, "Read size missmatch: required %d  actual %d  Status %d\r\n",wSize, dwNumberOfBytesRW, bStatus );
			return( FALSE );
		}
		wRxSize = (WORD)dwNumberOfBytesRW;
	}
	memcpy(  pRxBuffer, RxBuffer, wRxSize );

	return(TRUE);
}


BOOL SC02::Transfer( HANDLE hChildModule, void* pTxBuffer, WORD wTxSize, WORD wRxSize, BOOL RightNow )
{
	int size, i;

// Header registration 
	if(bComDeviceCount ==256 ) return( FALSE );
	pChildModule[ bComDeviceCount ] = (ControlModule*)hChildModule;

// Check transmit data size
	if( iTxBufCount==0 ){
		// sizeof(bCommand and bComDeviceCount) + sizeof(bPort and bID) + size0f(wTxSize) + sizeof(wRxSize) + wTxSize 
    	size = 2 + 2 + 4 + wTxSize;
		iTxBufCount = 1; // skip 1bytes data. device count
		iRxBufCount = 0;
	}else{
		size = iTxBufCount + 2 + 4 + wTxSize;
	}
	//printf("TR Tx Size %d\r\n", size );
	if( size > SC_BUF_SIZE ){
		sprintf_s( Error, "TX size exceeded by ID %d module", pChildModule[ bComDeviceCount ]->bID );
		return( FALSE );
	}

// Check receive data size iRxBufCount + sizeof(bPort and bID) + sizeof(wRxSize) + wRxSize + sizeof(bStatus)
	size =	iRxBufCount +  2 + 2 + wRxSize + 1;
	//printf("TR Rx Size %d\r\n", size );
	if( size > SC_BUF_SIZE ){
		sprintf_s( Error, "RX size size exceeded by ID %d module", pChildModule[ bComDeviceCount ]->bID );
		return( FALSE );
	}

////////////////////////////////////////////////////////////////////////////////////
	// Copy transmit header
	TransferTxBuffer[ iTxBufCount++ ] = pChildModule[ bComDeviceCount ]->bPort;
	TransferTxBuffer[ iTxBufCount++ ] = pChildModule[ bComDeviceCount ]->bID;
	memcpy( &TransferTxBuffer[ iTxBufCount ], &wTxSize, 2 );
	iTxBufCount += 2;
	memcpy( &TransferTxBuffer[ iTxBufCount ], &wRxSize, 2 );
	iTxBufCount += 2;

	// Copy transmit data and add TX and RX data size
	memcpy( &TransferTxBuffer[ iTxBufCount ], pTxBuffer, wTxSize );

	iTxBufCount += wTxSize;
	iRxBufCount += 4 + wRxSize; // add header
	bComDeviceCount++;

////////////////////////////////////////////////////////////////////////////////////

// Transceive 

	if( RightNow ){
		TransferTxBuffer[ 0 ] = bComDeviceCount;

		if( !Transceive( CM_TRANSFER, TransferTxBuffer, iTxBufCount, TransferRxBuffer, iRxBufCount ) ) return( FALSE );

	// Copy received data to registrated pointers
		BYTE* pBuffer;
		BYTE id, port;
		WORD rx_size;

		pBuffer = TransferRxBuffer;
		
		for(i=0; i<bComDeviceCount; i++ ){
			port = *pBuffer++;
			id = *pBuffer++;
			memcpy( &rx_size, pBuffer, 2 );
			pBuffer +=2;

			pChildModule[i]->Receive( pBuffer, rx_size );
			pBuffer += rx_size;
		}
	// End process
		bComDeviceCount = 0;
		iTxBufCount = 0;
		iRxBufCount = 0;
	}

	return(TRUE);
}

BOOL SC02::SetTimeout( DWORD dwTime_ms  )
{
	ftStatus = FT_SetTimeouts( ftHandle, dwTime_ms, dwTime_ms );
	if (ftStatus != FT_OK){
		sprintf_s( Error , "FT_SetTimeouts failed\n");
		return( FALSE ); 
	}

	dwTimeout_ms = dwTime_ms;

	return( TRUE );
}


BOOL SC02::Reset( void )
{
	DWORD dw;

	dw = dwTimeout_ms;

	SetTimeout( 5000 );	// Timeout 5 sec
	ftStatus = FT_ResetPort( ftHandle ); // Reset port
	if (ftStatus != FT_OK){
		sprintf_s( Error , "FT_ResetPort failed\n");
		return( FALSE ); 
	}
	Sleep(300);

	return(	SetTimeout( dw ) );
}

BOOL SC02::SetParam( PCM_PARAM pParam  )
{
	if(!Transceive( CM_SET_PARAM, pParam, sizeof( CM_PARAM ), NULL, 0 ) ) return(FALSE);

	return( SetTimeout( (DWORD)((pParam->fComTimeout+1)*1000.0f) ) );
}



