/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#ifndef AFXSTD_H
#define AFXSTD_H
#include <afxwin.h>
#include <afxtempl.h>
#include "resource.h"
//void MsgBox(CString );
//void MsgBoxN(int );

struct SHighScore{
	CHAR name[15];
	CHAR mode[15];
	CHAR time[6]; 
};
const CHAR g_szFileName[] = "Mines.hsc";
BOOL ReadFile(LPCTSTR filename,SHighScore *hs,char pos);
BOOL WriteFile(LPCTSTR filename,SHighScore * hs,char pos);
#endif