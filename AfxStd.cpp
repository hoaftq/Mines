/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#include "AfxStd.h"
/*
void MsgBox(CString str){
	MessageBox(NULL,str,"debug",MB_OK);
}
void MsgBoxN(int n){
	CString str;
	str.Format("%d",n);
	MsgBox(str);
}
*/
//hai ham nay dung de viet 1 highscore o trong CMinesFrame va CDlgBestTime
BOOL ReadFile(LPCTSTR filename,SHighScore *hs, char pos){
	try{
		CFile file(filename,CFile::modeRead|CFile::shareDenyNone);
		file.Seek(pos*sizeof(SHighScore),CFile::begin);
		if(file.Read(hs,sizeof(SHighScore))== sizeof(SHighScore)) return TRUE;
	}catch(CFileException *err){
		return FALSE;
	}
}
BOOL WriteFile(LPCTSTR filename,SHighScore *hs,char pos){
	CFile file;
	if(!file.Open(filename,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate|CFile::shareDenyNone))
		return FALSE;
	file.Seek(pos*sizeof(SHighScore),CFile::begin);
	file.Write(hs,sizeof(SHighScore));
	return TRUE;
}