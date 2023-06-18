#pragma once

extern double MES_dTime_ini;		/* Initial  time [s]*/
extern double MES_dTime_prv;		/* Previous time [s]*/
extern double MES_dTime_cur;		/* Current  time [s]*/
extern double MES_dTime_int;		/* Interval time [s]*/

void MES_StartTimeCount( void );
void MES_GetTimeCount( void  );
void MES_WaitNextTime( double dNextTime );
BOOL MES_Save( char* pFileName, char* pCaption, void* pData, int nItem, int nSize );
void MES_PrintData( void* pData, int nSize );

