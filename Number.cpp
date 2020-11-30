/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#include "Number.h"
CNumber::CNumber(CSize sizebar,COLORREF lc,COLORREF dc,COLORREF bkc){
	SetSizeBar(sizebar);
	m_clrLBar= lc;
	m_clrDBar= dc;
	m_clrBkgnd= bkc;
	m_iSpaceLed= 3;
}
CNumber::CNumber(CSize sizeled){
	m_sizeLed= sizeled;
	m_sizeBar.cx= 9*m_sizeLed.cx-4*m_sizeLed.cy;
	m_sizeBar.cy= (m_sizeLed.cy- 2*m_sizeBar.cx)/3;	
	//can than khi su dung ham nay vi phai tinh ra duoc 
	//m_sizeBar.x .y phai duong va co ty le phu hop moi co
	//hinh chinh xac	
	m_clrLBar= RGB(255,0,0);
	m_clrDBar= RGB(100,0,0);
	m_clrBkgnd= RGB(20,20,255);
	m_iSpaceLed= 3;
}

void CNumber::DrawBar(CDC* pDC,CPoint pos,BOOL ishoriz,BOOL islight){	
	CBrush br;
	CRect rect;
	//chon but ve co mau khac nhau khi thanh la sang hay toi
	if(islight) br.CreateSolidBrush(m_clrLBar);//pen.CreatePen(PS_SOLID,m_sizeBar.cx,);
	else br.CreateSolidBrush(m_clrDBar);
	//truong hop thanh nam ngang
	if(ishoriz)	rect.SetRect(pos,pos+m_sizeBar);
	else rect.SetRect(pos.x,pos.y,pos.x+m_sizeBar.cy,pos.y+m_sizeBar.cx);
	pDC->FillRect(&rect,&br);
}
//!khi goi ham nay thi figure chi co the la 0->10,10 ung voi dau -
void CNumber::DrawFigure(CDC *pDC,CPoint pos,UCHAR figure){
	CPoint m_pointPosLed[7] =
					{						
						CPoint(2*m_sizeBar.cy/3, 0),//thanh a	
						CPoint(m_sizeBar.cx+m_sizeBar.cy/3,m_sizeBar.cy),//thanh b
						CPoint(m_sizeBar.cx+m_sizeBar.cy/3,2*m_sizeBar.cy+m_sizeBar.cx),//thanh c
						CPoint(2*m_sizeBar.cy/3,2*(m_sizeBar.cx+m_sizeBar.cy)),//thanh d
						CPoint(0, 2*m_sizeBar.cy+m_sizeBar.cx),//thanh e
						CPoint(0, m_sizeBar.cy),//thanh f
						CPoint(2*m_sizeBar.cy/3, m_sizeBar.cx+m_sizeBar.cy)//thanh g											
					};
	static UCHAR tabled[11]= {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x3F};
	CBrush brD(m_clrLBar);//doi nguoc cho phu hop voi giao dien
	CBrush brL(m_clrDBar);	
	CBrush brBkgnd(m_clrBkgnd);
	CRect  rect;
	//ve nen cho led,khong can co the ve cho tat ca cac led trong 1 so luon
	/*
	rect.TopLeft()= pos;
	rect.BottomRight()= rect.TopLeft()+m_sizeLed;
	pDC->FillRect(&rect,&brBkgnd);
	*/
	//ve theo thu tu cac thanh a->g
	figure= tabled[figure];
	for(int i=0; i<7; i++){
		//truong hop thanh nam ngang thanh co dang nhu sau
		//		--------
		//     ----------
		//      --------
		CBrush *pbr= (figure &0x01)?&brL:&brD;//chon but ve tuy vao thanh co duoc sang hay khong
		if(i%3==0){
			//lay rect cua thanh nam ngang(gom 3 lop)
			rect.TopLeft()= pos+m_pointPosLed[i];
			//CString str;
			//str.Format("%d %d %d",m_pointPosLed[i].x,m_pointPosLed[i].y,2*m_sizeBar.cy/3);
			//MessageBox(NULL,str,"i",MB_OK);
			rect.BottomRight()= rect.TopLeft()+m_sizeBar;
			//lay rect  cua thanh ngang dau tien
			rect.DeflateRect(m_sizeBar.cy/3,0,m_sizeBar.cy/3,2*m_sizeBar.cy/3);
			pDC->FillRect(&rect,pbr);
			//lay rect cua thanh ngang duoi cung
			rect.OffsetRect(0,2*m_sizeBar.cy/3);
			pDC->FillRect(&rect,pbr);
			//lay rect cua thanh ngang o giua(tang do dai them ca trai va phai)			
			rect.OffsetRect(0,-m_sizeBar.cy/3);			
			rect.InflateRect(m_sizeBar.cy/3,0,m_sizeBar.cy/3,0);
			pDC->FillRect(&rect,pbr);
		}
		//truong hop thanh nam dung(lat thanh nam ngang len)
		else{
			//if(i==0){
			//lay rect cua thanh nam doc
			rect.TopLeft()= pos+m_pointPosLed[i];//CSize(m_pointPosLed[i].y,m_pointPosLed[i].x);
			rect.BottomRight()= rect.TopLeft()+CSize(m_sizeBar.cy,m_sizeBar.cx);
			//lay 1/3 ben trai nhat
			rect.DeflateRect( 0,m_sizeBar.cy/3,2*m_sizeBar.cy/3,m_sizeBar.cy/3);
			pDC->FillRect(&rect,pbr);
			//lay 1/3 ben phai nhat
			rect.OffsetRect(2*m_sizeBar.cy/3,0);
			pDC->FillRect(&rect,pbr);
			//lay 1/3 o giua
			rect.OffsetRect(-m_sizeBar.cy/3,0);
			rect.InflateRect(0,m_sizeBar.cy/3,0,m_sizeBar.cy/3);
			pDC->FillRect(&rect,pbr);
			//if(i==1) break;
			//}
		}
		figure>>=1;//dich phai 1 bit
		//lay 7 bit cua so figure tu thap len cao ung voi cac thanh  a->g
	}
} 
void CNumber::ShowNumber(CDC *pDC,CPoint pos,int number,int cntfigure){
	BOOL neg= FALSE;
	CPoint pos1led;
	if(number<0){ neg= TRUE; number=-number;}
	//viet tu led cuoi cung
	pos1led.y= pos.y;
	for(int i=cntfigure-1; i>=0 ; i--){
		pos1led.x=pos.x+i*(m_sizeLed.cx+m_iSpaceLed);
		DrawFigure(pDC,pos1led,number%10);
		number= number/10;
	}
	//truong hop so la am thi viet de dau tru len vi tri dau tien trong cntfigure chu so
	if(neg){		
		DrawFigure(pDC,pos,10);
	}
}
void CNumber::DrawColon(CDC *pDC,CPoint pos,BOOL islight){
	CRect rect;
	CBrush br(islight?m_clrLBar:m_clrDBar);
	//vi tri cua dau cham phia tren
	rect.TopLeft()= pos+CPoint(0,(m_sizeBar.cy+m_sizeBar.cx)/2);
	rect.BottomRight()= rect.TopLeft()+CSize(m_sizeBar.cy,m_sizeBar.cy);
	pDC->FillRect(&rect,&br);
	//vi tri cua dau cham phia duoi
	rect.OffsetRect(0,m_sizeBar.cx+m_sizeBar.cy);
	pDC->FillRect(&rect,&br);
}
void CNumber::ShowMinSec(CDC *pDC,CPoint pos,int min,int sec){
	ShowNumber(pDC,pos,min,2);
	pos.x=pos.x+2*(m_sizeLed.cx+m_iSpaceLed);
	DrawColon(pDC,pos,TRUE);
	pos.x+=m_sizeBar.cy+m_iSpaceLed;
	ShowNumber(pDC,pos,sec,2);
}
CSize CNumber::GetSizeNum(int cntfigure,int cntcolon){
	return CSize((cntfigure+cntcolon-1)*m_iSpaceLed+cntfigure*m_sizeLed.cx+cntcolon*m_sizeBar.cy,
				 m_sizeLed.cy);
}
void CNumber::SetSizeBar(CSize sizebar){
	m_sizeBar= sizebar;
	//sizeBar tinh theo thanh nam ngang ==
	m_sizeLed.cx= m_sizeBar.cx+4*m_sizeBar.cy/3;
	m_sizeLed.cy= 2*m_sizeBar.cx+3*m_sizeBar.cy;
}
void CNumber::ShowDate(CDC* pDC,CPoint pos,int day,int month,int year){
	ShowNumber(pDC,pos,day,2);
	pos.Offset(2*m_sizeLed.cx+2*m_iSpaceLed,0);
	DrawFigure(pDC,pos,10);
	pos.Offset(m_sizeLed.cx+m_iSpaceLed,0);
	ShowNumber(pDC,pos,month,2);
	pos.Offset(2*m_sizeLed.cx+2*m_iSpaceLed,0);
	DrawFigure(pDC,pos,10);
	pos.Offset(m_sizeLed.cx+m_iSpaceLed,0);
	ShowNumber(pDC,pos,year,4);

}