#include<windows.h>
#include<stdio.h>
#include<math.h>
#include"CM01.h"

CM01::CM01()
{
	CheckSumEnable = TRUE;

	bBoardID_PA = 0;
	bBoardID_PB = 0;
	sInputDataSize = 0;
	sOutputDataSize = 0;

	ZeroMemory( &EncBoard.lEncoder, sizeof( CM01_INPUT_ENC ) );
	ZeroMemory( &SensorBoard, sizeof( CM01_INPUT_SENSOR ) );
	ZeroMemory( &PwmBoard, sizeof( CM01_OUTPUT_PWM ) );
	ZeroMemory( &DaBoard, sizeof( CM01_OUTPUT_DA ) );

	PioBoardA.Word.wDir = 0xFFFF;
	PioBoardA.Word.wData = 0x0;
	PioBoardB.Word.wDir = 0xFFFF;
	PioBoardB.Word.wData = 0x0;

	ExtMode = FALSE;

}

CM01::~CM01()
{

}
  
BOOL CM01::Open( ControlModule* pParent, BYTE bParentPort, BYTE bModuleID )
{
	if(!ControlModule::Open( pParent, bParentPort, bModuleID )) return( FALSE );
	return( GetConfig() );
}

BOOL CM01::GetConfig( void  )
{
	BYTE buf[16];

	if( !Transceive( CM01_GET_CONFIG, NULL, 0, buf, 6 ) ) return( FALSE );

	bBoardID_PA = buf[0];
	bBoardID_PB = buf[1];
	memcpy( &sInputDataSize,  &buf[2], 2 ); // Input size
	memcpy( &sOutputDataSize, &buf[4], 2 ); // Output size

	return( TRUE );
}

BOOL CM01::Exchange( void* pOutputData, void* pInputData, BOOL RightNow )
{
	return( Transceive( CM01_EXCHANGE, pOutputData, sOutputDataSize, pInputData, sInputDataSize, RightNow ) );
}

BOOL CM01::Exchange( BOOL RightNow )
{
	BYTE buf_in[256];
	BYTE buf_out[256];
	BYTE* pBuf;

	pBuf = buf_out;
	if( bBoardID_PA == CM01_PIO_BOARD ){
		memcpy( buf_out, (BYTE*)&PioBoardA, CM01_OUTPUT_PIO_SIZE );
		pBuf+= sizeof( CM01_PIO );
	}
	switch( bBoardID_PB ){
	case CM01_PWM_BOARD:
		memcpy( pBuf, (BYTE*)&PwmBoard, CM01_OUTPUT_PWM_SIZE );
		break; // PWM board
	case CM01_DA_BOARD:
		memcpy( pBuf, (BYTE*)&DaBoard, CM01_OUTPUT_DA_SIZE );
		break; // DA board
	case CM01_PIO_BOARD:
		memcpy( pBuf, (BYTE*)&PioBoardB, CM01_OUTPUT_PIO_SIZE );
		break; // PIO board	
	}
	ExtMode = TRUE;
	
	if( !Transceive( CM01_EXCHANGE, buf_out, sOutputDataSize, buf_in, sInputDataSize, RightNow ) ) return( FALSE );

	return( TRUE );
}


BOOL CM01::Receive( BYTE *pRxBuffer, WORD wSize )
{
	BYTE* pBuf;

	if( wSize != wReqRxSize ){
		sprintf_s( Error, "Rx Data size miss match %d %d\r\n", wReqRxSize,wSize );
		
		if( CheckSumEnable ){
			wSize--; // sub checksum;

			if( wSize == 0 ){
				bStatus = pRxBuffer[0];
				return( FALSE );
			}
		}
		bStatus = pRxBuffer[wSize-1];
		return( FALSE );
	}
	
	if( CheckSumEnable ){
		wSize--; // -CheckSum
		if(!CheckSum( pRxBuffer, wSize )){
			bStatus = CM_REPLY_SUM_ERROR;
			return( FALSE );
		}
	}
	wSize--; // sub status;
			
	if( wSize > 0 ){
		if( ExtMode ){
			pBuf = pRxBuffer;
			switch( bBoardID_PA){
			case CM01_ENC_BOARD:
				memcpy( &EncBoard, pBuf, CM01_INPUT_ENC_SIZE );
				pBuf += CM01_INPUT_ENC_SIZE;
				break;
			case CM01_SENSOR_BOARD:
				memcpy( &SensorBoard, pBuf, CM01_INPUT_SENSOR_SIZE );
				pBuf += CM01_INPUT_SENSOR_SIZE;
				break;
			case CM01_PIO_BOARD:
				memcpy( &PioBoardA, pBuf, CM01_INPUT_PIO_SIZE );
				pBuf += CM01_INPUT_PIO_SIZE;
				break;
			}
			if( bBoardID_PB == CM01_PIO_BOARD ){
				memcpy( &PioBoardB, pBuf, CM01_INPUT_PIO_SIZE );
			}
			ExtMode = FALSE;

		}else{
			memcpy( pReserveBuffer, pRxBuffer, wSize );
		}
	}

	bStatus = pRxBuffer[wSize];
	
	if( bStatus!=CM_NORMAL ) return( FALSE );

	return( TRUE );
}

BOOL CM01::ClearData( void )
{
	return( Transceive( CM01_CLEAR_DATA, NULL, 0, NULL, 0 ) );
}

