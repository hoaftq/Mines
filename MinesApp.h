/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#ifndef MINESAPP_H
#define MINESAPP_H
#include "AfxStd.h"
#include "Number.h"
class CMinesApp: public CWinApp{
public:
	BOOL InitInstance();
};

class CDlgCustom: public CDialog{
public:
	UINT m_nMines;
	UINT m_nWidth;
	UINT m_nHeight;
	UINT m_nWidthMax;
	UINT m_nHeightMax;
	CDlgCustom(UINT w,UINT h,UINT m,CWnd * pParentWnd);
	BOOL OnInitDialog();	
	void DoDataExchange(CDataExchange *pDX);
	afx_msg void OnEditMinesFocus();
	DECLARE_MESSAGE_MAP()	
};


class CDlgAbout: public CDialog{
	CNumber m_Num;
public:
	CDlgAbout(CWnd *pParentWnd); 
	BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

class CDlgBestTime: public CDialog{
protected:
	//TRUE co nghia la chi xem,thong so nay se duoc init tuy theo ham
	//khoi tao de nhan biet dang o che do xem diem hay ghi diem
	BOOL m_bRead;
	//cac thong so can khi ghi diem cao moi
	int m_iGameMode;
	UINT m_nWidth,m_nHeight,m_nMines;
	UCHAR m_cMin,m_cSec;
	//load file diem cao
	void LoadFile();
	//2 nut bam
	void OnOK();
	void OnApply();
	BOOL OnInitDialog();	
public:
	CDlgBestTime(CWnd * pParentWnd);
	CDlgBestTime(CWnd * pParentWnd,int gamemode,UINT w,UINT h,UINT m,UCHAR min,UCHAR sec);
	DECLARE_MESSAGE_MAP()
};
#endif
