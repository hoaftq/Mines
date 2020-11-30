/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#ifndef NUMBER_H
#define NUMBER_H
#include "AfxStd.h"
class CNumber{
protected:
	COLORREF m_clrLBar;//mau cua thanh sang
	COLORREF m_clrDBar; //mau cua thanh toi
	COLORREF m_clrBkgnd;//mau nen cua led
	CSize	 m_sizeLed;//do lon cua led
	CSize    m_sizeBar;//do lon cua bar nam ngang,2 so lon nay phu thuoc nhau
	int		 m_iSpaceLed;//khoang cach giua 2 led trong 1 so
	//ve 1 thanh nam ngang hoac doc pos chi vi tri tren trai cua thanh
	void DrawBar(CDC* pDC,CPoint pos,BOOL ishoriz,BOOL islight);	
	void DrawColon(CDC* pDC,CPoint pos,BOOL islight);
	//ve 1 chu so tu 1->9,neu 10 la dau tru
	virtual void DrawFigure(CDC * pDC,CPoint pos,UCHAR figure);
public:
	CNumber(CSize sizebar,COLORREF lc,COLORREF dc,COLORREF bkc);
	CNumber(CSize sizeled);
	//ham dat lai kich thuoc cua thanh led
	void SetSizeBar(CSize sizebar);
	//ve so number,cntfigure la so chu so can ve
	void ShowNumber(CDC * pDC,CPoint pos,int number,int cntfigure);
	void ShowMinSec(CDC *pDC,CPoint pos,int min,int sec);
	//ham tra ve do rong vung dien tich can thiet de ve cntfigure chu so va cntcolon dau :
	CSize GetSizeNum(int cntfigure,int cntcolon);
	//ham ve thoi gian
	void ShowDate(CDC *pDC,CPoint pos,int day,int month,int year);
};
#endif