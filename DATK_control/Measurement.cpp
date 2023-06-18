#include<windows.h>
#include<stdio.h>
#include<conio.h>

double MES_dTimeScale;
double MES_dTime_ini;		/* Initial  time */
double MES_dTime_prv;		/* Previous time */
double MES_dTime_cur;		/* Current  time */
double MES_dTime_int;		/* Interval time */

void MES_StartTimeCount( void )
{
	LONGLONG freq;
	LONGLONG time_count;
	
	// 起動時からのＣＰＵクロックのカウント値から現在の時刻を計算する。
	 
	// ＣＰＵのクロック周波数を取得する。
	if( !QueryPerformanceFrequency( (LARGE_INTEGER*)&freq ) ){
		printf("err");
		return;
	}
	MES_dTimeScale = 1.0 / freq;	// 周波数から1クロックの周期を計算する。
	
	QueryPerformanceCounter( (LARGE_INTEGER*)&time_count ); // カウント値を取得する。
	MES_dTime_ini = time_count * MES_dTimeScale;						// 開始時間として記録する。
	MES_dTime_cur = 0;
	MES_dTime_prv = 0;
}

void MES_GetTimeCount( void  )
{
	LONGLONG time_count;

	QueryPerformanceCounter( (LARGE_INTEGER*)&time_count ); // カウント値を取得する。
	
	MES_dTime_cur = time_count * MES_dTimeScale - MES_dTime_ini; //　StartTimeCountからの現在の時間（経過時間）を計算する
	MES_dTime_int = MES_dTime_cur - MES_dTime_prv;	// 	前回からの経過時間を計測する。	
	MES_dTime_prv = MES_dTime_cur;			// 	現在の時間を保存する。
}

void MES_WaitNextTime( double dNextTime )
{
	LONGLONG time_count;

	do{ // 	dNextTime秒になるまで待つ。
		QueryPerformanceCounter( (LARGE_INTEGER*)&time_count );
		
		MES_dTime_cur = time_count * MES_dTimeScale - MES_dTime_ini;
	}while( MES_dTime_cur < dNextTime );
	
	MES_dTime_int = MES_dTime_cur - MES_dTime_prv;
	MES_dTime_prv = MES_dTime_cur;
}


BOOL MES_Save( char* pFileName, char* pCaption, void* pData, int nItem, int nSize )
{
	int i;
	FILE* fp;
	float* pfData;

	if( fopen_s( &fp, pFileName, "w+" ) != 0 ){
		printf( "ファイルが開けません");
		return( FALSE );
	}

	fprintf( fp, "%s\n",pCaption );

	pfData = (float*)pData;

	for(i=0;i<nSize*nItem;i++){
		fprintf( fp,"%f",*pfData++ );
		if( (i+1)%nItem==0 ) fprintf( fp,"\n");
		else fprintf( fp,"," );
	}

	fclose( fp );
	return( TRUE );
};


void MES_PrintData( void* pData, int nSize )
{
	int i;
	unsigned char* buf;

	buf = (unsigned char*)pData;

	i=0;
	while(nSize--){
        printf( "%02x ", *buf++ );
		if(++i==16){
			printf("\r\n");
			i=0;
		}
	}
	printf("\r\n");
}
