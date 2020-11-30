/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#ifndef MINESVIEW_H
#define MINESVIEW_H
#include "AfxStd.h"
#include "MinesDat.h"
#include "Number.h"
//trang thai ve cua cac o,su dung de nhan biet hinh ve tuong ung
enum EMinesDraw{
	D_CLOSE,//tinh trang binh thuong
	D_DOWN,
	D_NUMBER,//neu vao truong hop nay thi xet so phai ve neu la 0 thi ko ve
	D_DIAGONALMINE,//o danh dau sai mine
	D_CLKMINE,	   //o trung phai mine
	D_MASKMINE,	   //o danh dau mine
	D_QUESTION,	   //o danh dau?
	D_EMPTY,
	D_MINESTD	  //hien ra mine khi ket thuc choi
};
//trang thai cua 1 mybutton
enum EButtonState{
	BS_OVER,//trang thai khi co mouse o tren
	BS_DOWN,
	BS_UP,//trang thai binh thuong
	BS_4	//trang thai ma nut tro thanh 1 hien thi khong tuong tac nua
};
class CMines : public CMinesDat{
protected:
	//do rong cua 1 squ,ca 2 chieu bang nhau
	UINT m_nSize;
	//vi tri dat bang choi tinh theo left va top
	CPoint m_pointLT; 
	CRect m_rectMines;//vi tri dat time va so mine
	CRect m_rectTime;
	CRect m_rectNewGame;//button click de new game
	//khoi dong cac bien duoc goi boi cac ham khoi tao
	void Init();
	//draw 1 button o vi tri tuy y, voi bitmap va cac trang thai khac nhau
	void DrawButton(CDC* pDC,CRect & rect,UINT IDBitmap,EButtonState bs,BOOL drawbkgnd= TRUE);
	//lay lai CRect cua squ o (i,j)
	CRect GetRectSqu(UINT i,UINT j);
	//su dung de nhan biet "chuot" dang an,va luu vi tri cu cua chuot 
	BOOL m_bDownning;
	CPoint m_pointOldDown;
	//bien luu trang thai choi: co the choi tiep hay vua 
	//cham phai mine(chi co the choi tiep khie an newgame)
	BOOL m_bPlaying; //duoc gan TRUE trong newgame
	CPoint m_pointMineClk;//vi tri kich phai mine chi duoc su dung khi m_bPlaying= FALSE
	//can luu lai tham so nay de su dung cho ham DrawSqusEnd
	
	//luu so o chua mo con lai,luc so nay bang 0 va m_bPlaying= TRUE thi chien thang
	//phai khoi dong va newgame bien nay bang tong so o
	UINT m_nCloseRemain;
	
	//obj hien thi so mine con lai va time
	CNumber m_numMinesTime;
	UINT    m_nMineMask;//so mine danh dau boi nguoi su dung
	UCHAR	m_cMin,m_cSec;//thoi gian da qua,duoc khoi dong trong Init va NewGame,cap nhat trong OnTimer

	BOOL	m_bQues;//neu la TRUE thi cho phep dat dau ? khi click chuot phai
public:
	CMines();
	CMines(UINT w,UINT h,UINT m);
protected:
	//ham ve tung squ dua vao tham so md
	void Draw1Squ(CDC* pDC,UINT i,UINT j,EMinesDraw md);
	//xet xem diem point nam trong squ thu may trong bang choi	
	//tra lai TRUE neu tim duoc
	BOOL GetPosOfPoint(CPoint point,CPoint & pos);
	//ham ve bang choi khi chua ket thuc van choi(luc chua gap mine hoac chua go het)
	void DrawSqus(CDC* pDC);
	void DrawSqus2(CDC* pDC);
	//ve bang choi khi luot choi ket thuc(luc gap mine),cung nhu ham tren 
	//no dua vao trang thai chon cua nguoi choi va gia tri thuc
	//m_pointMineClk la vi tri click phai mine
	void DrawSqusEnd(CDC *pDC);
public:		
	//ham hien so mines
	void DrawCntMines(CDC *pDC);
	//ham hien trhi thoi gian
	void DrawTime(CDC* pDC);
	//ve button newgame
	void DrawBntNewGame(CDC *pDC,EButtonState bs= BS_UP,UINT IDBitmap= IDB_NORMAL);
	//ham nay ve bang choi tuy vao m_bPlaying ma goi DrawSqus hay DrawSqusEnd	
	void ShowBoard(CDC *pDC);
	//kiem tra trang thai co the choi duoc hay da gap phai mine
	BOOL IsPlaying() const;
	//tao luot choi moi	
	void NewGame(UINT w,UINT h,UINT m);
	//ham tra ve kich thuoc can thiet cua vung choi,dung de tinh kich thuoc cua so
	void GetNeedRect(CRect & rect);
	//gia cac ham nhu khi co su kien chuot,se duoc goi trong cac ham tuong ung
	void OnMouseMove(CDC *pDC,UINT nFlags,CPoint point);	
	void OnLButtonDown(CDC* pDC,UINT nFlags,CPoint point);
	//gia tri cho lai nhu sau 1 neu click tren button newgame
	//						  2 neu click phai mine
	//						  3 neu click vao o rong hoac so cuoi cung(tuc la co chien thang)
	//                        con lai cho gia tri 0
	UINT OnLButtonUp(CDC *pDC,UINT nFlags,CPoint point);
	//gia tri tra ve la  3 neu da co chien thang(danh dau o mine cuoi cung)
	//					 con lai gia tri 0
	UINT OnRButtonDown(CDC* pDC,UINT nFlags,CPoint point);
	void OnTimer(CDC *pDC,UINT nIDEvent);
	//ham dat va lay viec cho phep hay khong viec dat dau ?
	BOOL & Marks();
	//ham cho phep lay do rong cua moi o
	UINT GetSizeSquare();
	//ham lay vi tri bang choi
	CPoint GetPosBoard();
	//lay thoi gian
	void GetTime(UCHAR & min,UCHAR & sec);

	friend void CALLBACK CallbackSquEmptys(CDC *pDC,CMines *pMines,UINT i,UINT j,BOOL isempty);
};
#endif