/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#ifndef MINESDAT_H
#define MINESDAT_H
#include "AfxStd.h"
//trang thai cua o,phu thuoc nguoi choi
enum EMinesState{
	S_CLOSE= 0,
	S_OPEN,
	S_MASK,
	S_QUES
};
//trang thai gia tri cua cac o, do cach dien mines
//co the nhan biet qua thanh phan value cua SMinesSqu
//0 la o rong,FFFFH(-1) la mine,con lai la so tu 1->8
enum EMinesValue{
	V_EMPTY= 10,
	V_NUMBER,
	V_MINE
};

struct SMinesSqu{
	EMinesState state;//trang thai do nguoi choi
	int		value;//gia tri cua o = -1 neu la mine, 0 neu la trong va gia tri so neu la o so
	BOOL	tested;//dung de tim mien o rong
};				 
//ham lay lai trang thai cua o tu gia tri value cua o
EMinesValue GetStateValue(int value);

class CMines;
typedef void (CALLBACK * FINDSQUEMPTYS)(CDC * pDC,CMines * pMines,UINT i,UINT j,BOOL isempty);

class CMinesDat {
protected:
	UINT m_nWidth,m_nHeight,m_nMines;
	CArray<SMinesSqu,SMinesSqu &> squ;
	//dien ngau nhien min
	void RandMines();
	//dien so sau khi da dien min
	void FillNums();	
	SMinesSqu & GetSqu(UINT i,UINT j);
private:
	//tim so mine random trong moi hang sao cho tong so min la m_nMines,so mine cua moi hang luu trong arrcntms
	void RandRowCntMines(CUIntArray & arrcntms);
	//dien random mine vao cac cot cua hang row sao cho so min trong hang la cntmines
	void FillMinesRow(UINT cntmines,UINT row);
public:
	CMinesDat(UINT w,UINT h,UINT m);
	CMinesDat();
	~CMinesDat();
	//dat va lay kich thuoc bang choi va so min
	void SetSize(UINT w,UINT h,UINT m);
	void GetSize(UINT & w,UINT & h,UINT & m) const;
	//lay va dat trang thai cua 1 o
	EMinesState & GetRefState(UINT i,UINT j);
	void SetState(UINT i,UINT j,EMinesState state);
	//lay gia tri so cua mot o cho lai gia tri 0 neu khong phai la o so,-1 neu la mine
	//chu y -1 chinh la FFFFH
	UINT GetValue(UINT i,UINT j) const;
	//ham nay phai duoc goi truoc khi su dung lai doi tuong
	void Reset();
	//tim cac o trong xung quanh vi tri (i,j) lay ca tren vi tri bien (la so)
	void FindSquEmptys(UINT i,UINT j,FINDSQUEMPTYS FindSquEmptysProc,CDC * pDC,CMines * pMines);
	//void ResetTested();//khoi tao lai gia tri tested cua cac o
	
	//friend void CALLBACK CallbackSquEmptys(CDC *pDC,CMines *pMines,UINT i,UINT j,BOOL isempty);
public:
	void Print();
};
#endif
