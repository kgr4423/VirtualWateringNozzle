#include"Server.h"
#include<windows.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<math.h>
#include"SC02.h"
#include"CM01.h"
#include"Measurement.h"

#define MODULE_NUM	1	// 連結するCM01の台数

// 指定されたCM基盤pModuleに送るデータnValを設定する
void SetOutput( CM01 *pModule, int nVal )
{
	// ポートAがPIOの場合のデータ設定
	if( pModule->bBoardID_PA == CM01_PIO_BOARD ){
		pModule->PioBoardA.Word.wDir = 0xFFFF;
	}
	// ポートBのデータ設定
	switch(  pModule->bBoardID_PB ){
	case CM01_PWM_BOARD:
		pModule->PwmBoard.sPwm1 = nVal;
		pModule->PwmBoard.sPwm2 = nVal;
		break; 
	case CM01_DA_BOARD:
		pModule->DaBoard.sDA = nVal;
		break;
	case CM01_PIO_BOARD:
		pModule->PioBoardB.Word.wDir = 0xFFFF;
		break;	
	}
}

// 指定されたCM基盤pModuleに入力されたデータをコンソールに表示
void PrintData( CM01 *pModule )
{
	printf(" ID=%d ", 0x00FF&pModule->bID );

	switch( pModule->bBoardID_PA){
	case CM01_ENC_BOARD:
		printf("enc %d ad %d ", pModule->EncBoard.lEncoder, pModule->EncBoard.wSensor[0] );
		break;
	case CM01_SENSOR_BOARD:
		printf("sens %d  %d  %d  %d  %d  %d  %d  %d ",
			pModule->SensorBoard.wSensor[0], pModule->SensorBoard.wSensor[1],
			pModule->SensorBoard.wSensor[2], pModule->SensorBoard.wSensor[3],
			pModule->SensorBoard.wSensor[4], pModule->SensorBoard.wSensor[5],
			pModule->SensorBoard.wSensor[6], pModule->SensorBoard.wSensor[7]
		);
		break;
	case CM01_PIO_BOARD:
		printf("pioA %04x ", pModule->PioBoardA.Word.wData );
		break;
	}
	// ポートBのデータ設定
	switch(  pModule->bBoardID_PB ){
	case CM01_PWM_BOARD:
		printf("pwm %d ", pModule->PwmBoard.sPwm1 );
		break; 
	case CM01_DA_BOARD:
		printf("da %d ", pModule->DaBoard.sDA );
		break;
	case CM01_PIO_BOARD:
		printf("pioB %04x ", pModule->PioBoardB.Word.wData );
		break;	
	}
}

int main( int argc, char* argv[] )
{
	Server server;
	SC02 sc;
	CM01 cm[MODULE_NUM];

	int nVal = 0;
	int i;
	BOOL loop=TRUE;

	// Client(Unity)と接続する
	server.Connect();

	// SC基盤を開く
	printf("Open SC02 ");
	if( !sc.Open( CM_MASTER_ID ) ){
		printf( "Error:%s\r\n", sc.Error );
		return( 0 );
	}
	printf("OK\n");
	// CM基板を開く
	for(i=0;i<MODULE_NUM;i++){
		printf("Open CM01 ID=%d ", i );
		if( !cm[i].Open( &sc, 0, (BYTE)i+1 )){
			printf( "Error:%s", cm[i].Error );
			printf("Close SC02\n");
			sc.Close();
			return( 0 );
		}
		printf("OK\n");
	}

	// 時間計測開始
	MES_StartTimeCount();

	// Client(Unity) ⇔ Server(このプログラム) ⇔ DATK のデータ送受信ループ
	while(loop){
		// コンソールに時間表示
		printf("%02d:%02d:%02d dt %.3fms ", (int)(MES_dTime_cur/60.0f),(int)MES_dTime_cur%60,(int)(100.0f*MES_dTime_cur)%100, MES_dTime_int * 1000.0f);
		// Escapeキーで終了
		if(_kbhit()){
			switch(_getch()){
			case 27:
				
				loop = FALSE;
				nVal = 0;
				break; 
			}
		}
		// Unityからデータを受信
		server.Receive();
		nVal = (int)server.receive_num;
		// DATKへの出力データを設定
		for(i=0;i<MODULE_NUM;i++) SetOutput( &cm[i], nVal );
		// 通信時間計測
		MES_GetTimeCount(); 
		// DATKとのデータ送受信
		for(i=0;i<MODULE_NUM-1;i++){
			if(!cm[i].Exchange(FALSE) ){
				printf("Error:%s", cm[i].Error );
				printf("Close SC02\n");
				sc.Close();
				return(1);
			}
		}
		if(!cm[i].Exchange(TRUE) ){
			printf("Error:%s", cm[i].Error );
			printf("Close SC02\n");
			sc.Close();
			return(1);
		}
		// 通信時間計測
		MES_GetTimeCount(); 
		// DATKからの受信データを表示
		for(i=0;i<MODULE_NUM;i++) PrintData( &cm[i] );
		printf("\n");
		// Unityへデータを送信
		for(i=0;i<MODULE_NUM;i++) server.Send( &cm[i] );

	    Sleep(100);
	}
	
	// CM基盤を閉じる
	for(i=0;i<MODULE_NUM;i++){
		printf("Close CM01 ID=%d ", i ); 
		if( !cm[i].Close() ){
			printf("Error:%s", cm[i].Error );
		}else{
			printf("OK\r\n");
		}
	}
	// SC基盤を閉じる
	printf("Close SC02 ");
	sc.Close();
	printf("OK\n");

	// Clientとの接続を切る
	server.Inf();

	return(1);
}

