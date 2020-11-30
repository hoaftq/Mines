/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#include "MinesView.h"
inline void CMines::Init(){
	//khoi dong do rong 1 o va vi tri dat bang choi
	m_nSize= 21;
	m_pointLT = CPoint(0,60);//vi tri dat bang choi,tuy vao vi tri nay de tinh vi tri dat time va hien so mine
	m_rectMines= CRect(0,5,0,50);//0 thu 2 se duoc dat lai
	m_rectTime= m_rectMines;//se duoc khoi dong lai TopLeft() trong newgame
	m_rectNewGame= m_rectMines;
	m_bDownning= FALSE;
	//gia tri khoi dong cho bien nay khong co y nghia vi khi Down thi no co gia tri dau
	//m_pointOldDown= CPoint(-1,-1);
	m_nCloseRemain= m_nWidth* m_nHeight;
	//bien nay se giam trong qua trinh mo o trong LButtonUp
	//hoac trong ham goi nguoc
	
	//so mine ban dau danh dau boi nguoi su dung bang 0
	m_nMineMask= 0;//se duoc sua doi chi trong OnRButtonDown va NewGame de dat lai gia tri dau
	m_cMin= m_cSec= 0;

	m_bQues= FALSE;//cho phep dat dau ?
}
CMines::CMines():CMinesDat(),
m_numMinesTime(CSize(8,2),RGB(0,0,255),RGB(198,198,140),0)
{
	Init();
}
CMines::CMines(UINT w,UINT h,UINT m):CMinesDat(w,h,m),
m_numMinesTime(CSize(8,2),RGB(0,0,255),RGB(198,198,140),0)
{
	Init();
}
CRect CMines::GetRectSqu(UINT i,UINT j){
	//tra ve rect cua 1 o so voi goc tren trai cua cua so
	CRect rect(j*m_nSize,i*m_nSize,(j+1)*m_nSize,(i+1)*m_nSize);
	rect.OffsetRect(m_pointLT);
	return rect;
} 
void CMines::DrawButton(CDC *pDC,CRect  & rect,UINT IDBitmap,EButtonState bs,BOOL drawbkgnd){
	CDC dcMem;
	CBitmap bmp,*oldbmp;
	BITMAP	bm;
	CPen    pen1,pen2,*oldpen;
	CBrush  br;
	int l,t;
	if(drawbkgnd){//bo qua viec ve nen vi khong mau nen trung mau cua cua so
		br.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		pDC->FillRect(&rect,&br);
	}
	if(IDBitmap){
		bmp.LoadMappedBitmap(IDBitmap);
		dcMem.CreateCompatibleDC(pDC);	
		oldbmp= dcMem.SelectObject(&bmp);
		bmp.GetBitmap(&bm);
		//ve bitmap ngay giua button
		l= rect.left+(rect.Width()-bm.bmWidth)/2;
		t= rect.top+(rect.Height()-bm.bmHeight)/2;
		pDC->BitBlt(l,t,bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);
	}
	switch(bs){
	case BS_OVER:	
					break;
	case BS_DOWN:	//ve o dang down
					pen2.CreatePen(PS_SOLID,3,RGB(255,255,255));
					pen1.CreatePen(PS_SOLID,3,RGB(170,170,170));
					rect.DeflateRect(2,1);
					goto label;
	case BS_UP:		//ve o dang up
					pen1.CreatePen(PS_SOLID,3,RGB(255,255,255));
					pen2.CreatePen(PS_SOLID,3,RGB(170,170,170));
					rect.DeflateRect(2,1);
					goto label;
	case BS_4:		//ve o dang phang
					pen1.CreatePen(PS_SOLID,1,RGB(170,170,170));
					pen2.CreatePen(PS_SOLID,1,RGB(170,170,170)); 
label:				//ve duong vien tren
					oldpen= pDC->SelectObject(&pen1);
					pDC->MoveTo(rect.left,rect.bottom);
					pDC->LineTo(rect.left,rect.top);
					pDC->LineTo(rect.right,rect.top);					
					//ve duong vien duoi
					pDC->SelectObject(&pen2);
					pDC->MoveTo(rect.left,rect.bottom);
					pDC->LineTo(rect.right,rect.bottom);
					pDC->LineTo(rect.right,rect.top);
					pDC->SelectObject(oldpen);
	}
	if(IDBitmap) dcMem.SelectObject(oldbmp);
	//dcMem.DeleteDC();//co can khong ?
	//bmp.DeleteObject();
}
//viet lai ham DrawSqus(ve lai tat ca cac o) nham tang toc trong OnPaint
void CMines::DrawSqus2(CDC *pDC){
/*	
	CDC dcMem;	
	CBitmap bmp,*oldbmp;
	BITMAP	bm;
	CPen    pen1,pen2,*oldpen;
	CBrush  br;
	int l,t;
	br.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	//rect.DeflateRect(1,1);
	pDC->FillRect(&rect,&br);
	if(IDBitmap){
		bmp.LoadMappedBitmap(IDBitmap);
		dcMem.CreateCompatibleDC(pDC);	
		oldbmp= dcMem.SelectObject(&bmp);
		bmp.GetBitmap(&bm);
		//ve bitmap ngay giua button
		l= rect.left+(rect.Width()-bm.bmWidth)/2;
		t= rect.top+(rect.Height()-bm.bmHeight)/2;
		pDC->BitBlt(l,t,bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);
	}

*/
//ham nay ve dua vao trang thai trong do nguoi su dung va ket hop voi gia tri
//that cua o
	CRect rectSqu;//luu toa do tren trai cua o 
	int i,j;
	UINT IDBmp;
	CBitmap bmp;
	BITMAP  bm;
	CDC dcMem;//dung de load anh bitmap
	CPen pen1(PS_SOLID,3,RGB(255,255,255));//pen1 & 2 dung de ve button o dang up
	CPen pen2(PS_SOLID,3,RGB(170,170,170));
 	CPen pen3(PS_SOLID,1,RGB(170,170,170));//dung de ve button o dang BS_4
	CPen *oldpen = pDC->GetCurrentPen();//luu lai pen ban dau
	int l,t;
	dcMem.CreateCompatibleDC(pDC);//dung de ve bitmap
	CBitmap *oldbmp = dcMem.GetCurrentBitmap();//luu lai bitmap ban dau
	
	//duyet qua tat ca caco
	for(i=0; i< m_nHeight; i++){
		//rect cua o ben trai nhat ung voi hang i
		rectSqu.TopLeft()= m_pointLT +CPoint(0,i*m_nSize);
		rectSqu.BottomRight()= rectSqu.TopLeft()+CPoint(m_nSize,m_nSize);
		//ung voi hang i duyet cac cot tu 0->m_nWidth-1
		for(j= 0; j< m_nWidth; j++){			
			SMinesSqu & refsqu= GetSqu(i,j);
			switch(refsqu.state){
			case S_MASK:
					IDBmp= IDB_MINEMASK; goto lb1;
			case S_QUES:
					IDBmp= IDB_QUESTION;
lb1:
					bmp.LoadMappedBitmap(IDBmp);						
					dcMem.SelectObject(&bmp);
					bmp.GetBitmap(&bm);
					//ve bitmap ngay giua button
					l= rectSqu.left+(rectSqu.Width()-bm.bmWidth)/2;
					t= rectSqu.top+(rectSqu.Height()-bm.bmHeight)/2;
					pDC->BitBlt(l,t,bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);
			case S_CLOSE:
					pDC->SelectObject(&pen1);
					pDC->MoveTo(rectSqu.left+1,rectSqu.bottom-2);
					pDC->LineTo(rectSqu.left+1,rectSqu.top+1);
					pDC->LineTo(rectSqu.right-2,rectSqu.top+1);					
					pDC->SelectObject(&pen2);
					pDC->MoveTo(rectSqu.left+1,rectSqu.bottom-2);
					pDC->LineTo(rectSqu.right-2,rectSqu.bottom-2);
					pDC->LineTo(rectSqu.right-2,rectSqu.top+1);
					break;
			case S_OPEN:
					;	
			}
		   //tinh toa do cua o (i,j) (chuyen sang o tiep theo trong hang)
			rectSqu.OffsetRect(m_nSize,0);	
		}		
	}
	
/*									
			case BS_OVER:	
						break;
			case BS_DOWN:
						pen2.CreatePen(PS_SOLID,3,RGB(255,255,255));
						pen1.CreatePen(PS_SOLID,3,RGB(170,170,170));
						rect.DeflateRect(2,1);
						goto label;
			case BS_UP:		
						pen1.CreatePen(PS_SOLID,3,RGB(255,255,255));
						pen2.CreatePen(PS_SOLID,3,RGB(170,170,170));
						rect.DeflateRect(2,1);
						goto label;
			case BS_4:		
						pen1.CreatePen(PS_SOLID,1,RGB(170,170,170));
						pen2.CreatePen(PS_SOLID,1,RGB(170,170,170)); 
label:			
						oldpen= pDC->SelectObject(&pen1);
						pDC->MoveTo(rect.left,rect.bottom);
						pDC->LineTo(rect.left,rect.top);
						pDC->LineTo(rect.right,rect.top);					
						pDC->SelectObject(&pen2);
						pDC->MoveTo(rect.left,rect.bottom);
						pDC->LineTo(rect.right,rect.bottom);
						pDC->LineTo(rect.right,rect.top);
						pDC->SelectObject(oldpen);
			}
		}//end for
	if(IDBitmap) dcMem.SelectObject(oldbmp);
	dcMem.DeleteDC();//co can khong ?
	bmp.DeleteObject();
*/
}
void CMines::Draw1Squ(CDC *pDC,UINT i,UINT j,EMinesDraw md){
	UINT IDBitmap;
	EButtonState bs;
	CRect rect= GetRectSqu(i,j);
	switch(md){
	case D_CLOSE:
			bs= BS_UP; IDBitmap= 0; break;
	case D_DOWN:
			bs= BS_DOWN; IDBitmap= 0; break;
	case D_NUMBER:
			bs= BS_4; IDBitmap= GetValue(i,j)-1 +IDB_NUM1; break;
	case D_DIAGONALMINE:
			bs= BS_4; IDBitmap= IDB_MINEFAULT; break;
	case D_CLKMINE:
			bs= BS_4; IDBitmap= IDB_MINECLK; break;
	case D_MASKMINE: 
			bs= BS_UP; IDBitmap= IDB_MINEMASK; break;
	case D_QUESTION:
			bs= BS_UP; IDBitmap= IDB_QUESTION; break;
	case D_EMPTY:
			bs= BS_4; IDBitmap= 0; break;
	case D_MINESTD:
			bs= BS_4; IDBitmap= IDB_MINESTD;
	}
	DrawButton(pDC,rect,IDBitmap,bs);
}
void CMines::DrawSqus(CDC *pDC){
	UINT i,j;
	//duyet qua tung o va tuy thuoc vao trang thai nguoi su dung va gia tri 
	//cua o de ve
	for(i=0; i<m_nHeight; i++)
		for(j=0; j< m_nWidth; j++){
			switch(GetRefState(i,j)){
			case S_CLOSE:
				Draw1Squ(pDC,i,j,D_CLOSE); break;
			case S_OPEN:
				if(GetValue(i,j)) Draw1Squ(pDC,i,j,D_NUMBER);
				else Draw1Squ(pDC,i,j,D_EMPTY);
				break;
			case S_MASK:
				Draw1Squ(pDC,i,j,D_MASKMINE); break;
			case S_QUES:
				Draw1Squ(pDC,i,j,D_QUESTION);
			}
		}
}
void CMines::DrawSqusEnd(CDC *pDC){
	//truoc khi goi ham nay thi trang thai cua o (i,j) phai dat la S_OPEN
	UINT l,k;
	for(l=0; l<m_nHeight; l++)
		for(k=0; k<m_nWidth; k++){
			switch(GetRefState(l,k)){			
			case S_MASK:
				//neu danh dau dung thi ve o trang thai danh dau
				if(GetValue(l,k)== -1) Draw1Squ(pDC,l,k,D_MASKMINE);
				//neu khong thi ve o trang thai danh dau sai
				else Draw1Squ(pDC,l,k,D_DIAGONALMINE);
				break;
			case S_OPEN:
				//neu la o mine click vao thi ve mine voi nen do
				if(l==m_pointMineClk.x && k== m_pointMineClk.y) Draw1Squ(pDC,l,k,D_CLKMINE);
				//neu khong thi ve so
				else if(GetValue(l,k)!= 0) Draw1Squ(pDC,l,k,D_NUMBER);
				//hoac ve o rong
				else Draw1Squ(pDC,l,k,D_EMPTY);
				break;
			case S_CLOSE:				
				//neu la mine thi hien ra
				if(GetValue(l,k)==-1) Draw1Squ(pDC,l,k,D_MINESTD);
				//neu la so hoac o rong thi thoi
				else Draw1Squ(pDC,l,k,D_CLOSE);
				break;
			case S_QUES:
				if(GetValue(l,k)==-1) Draw1Squ(pDC,l,k,D_MINESTD);
				else Draw1Squ(pDC,l,k,D_QUESTION);
			}
		}
}
//ham nay se duoc goi trong OnPaint cua cua so hien thi,
//chi dung duy nhat trong OnPaint de ve lai toan bo
void CMines::ShowBoard(CDC *pDC){
	DrawCntMines(pDC);
	DrawTime(pDC);
	//chua gap mine tuc la dang choi hoac da thang
	if(m_bPlaying){
		//neu con o chua mo
		if(m_nCloseRemain) DrawBntNewGame(pDC);
		//da mo het, tuc la co chien thang
		else DrawBntNewGame(pDC,BS_UP,IDB_WIN);
	}
	//neu m_bPlaying= FALSE co nghia la da gap mine
	else DrawBntNewGame(pDC,BS_UP,IDB_DIE);	
	if(m_bPlaying) DrawSqus(pDC); else DrawSqusEnd(pDC);
}
//trang thai choi chi co the dat boi chinh doi tuong,khong duoc dat tu ben ngoai
BOOL CMines::IsPlaying() const{
	return m_bPlaying;
}
//point o day co toa do theo cua so,pos chua hang va cot tuong ung
BOOL CMines::GetPosOfPoint(CPoint point,CPoint & pos){
	/*
	UINT i,j;
	if(point.x< m_pointLT.x || point.y<m_pointLT.y) return FALSE;
	for(i=0; i< m_nWidth; i++){
		//so sanh canh ben phai cua o thu i voi diem xet
		if(point.x< m_pointLT.x+(i+1)*m_nSize) break;
	}
	for(j=0; j< m_nHeight; j++){
		if(point.y< m_pointLT.y+(j+1)*m_nSize) break;
	}
	if(i==m_nWidth || j== m_nHeight) return FALSE;
	//do quy uoc la pos.x la hang pos.y la cot
	pos = CPoint(j,i); return TRUE;
	*/
	//chu y 2 vien tren va trai thi thuoc vao o do,con vien duoi va phai
	//thi thuoc ve 2 o khac
	int i,j;
	i= (point.y-m_pointLT.y)/m_nSize;
	j= (point.x-m_pointLT.x)/m_nSize;
	if(i<0 ||i>= m_nHeight ||j<0 ||j>= m_nWidth) return FALSE;
	pos= CPoint(i,j); return TRUE;
}

void CMines::NewGame(UINT w,UINT h,UINT m){
	//qua trinh sinh du lieu: Dat kich thuoc va so mine->Reset so lieu dam
	//bao tinh dung dan cho phat sinh du lieu->Dien ngau nhien mine vao
	//cac o->Dien day so vao cac o con lai	
	m_bPlaying= TRUE;
	SetSize(w,h,m);
	m_nCloseRemain= m_nWidth* m_nHeight;
	m_nMineMask= 0;//reset so mine duoc danh dau
	m_cMin= m_cSec= 0;//reset thoi gian
	//tinh toan vi tri dat cac button
	//phan nay chi can goi khi tao game voi kich thuoc moi
	CRect rect;
	int width;
	#define SPACE 2
	GetNeedRect(rect);//lay rect can thiet cua ca cua so
	width= (rect.Width()-2*SPACE)/3;//SPACE la khoang cach giua 2 button
	m_rectMines.right= m_rectMines.left+width;
	m_rectTime.left= m_rectMines.right+SPACE;
	m_rectTime.right= m_rectTime.left+width;
	m_rectNewGame.left= m_rectTime.right;
	m_rectNewGame.right= m_rectNewGame.left+width;
	
	//tuy vao kich thuoc(be rong w) ma dat lai kich co cho led
	if(w<14)
		m_numMinesTime.SetSizeBar(CSize(3+w-6,2));
	/*
	if(w==6) 
		m_numMinesTime.SetSizeBar(CSize(3,2));
	else if(w==7)
		m_numMinesTime.SetSizeBar(CSize(4,2));
	else if(w<=10)
		m_numMinesTime.SetSizeBar(CSize(8,2));//ham khoi tao cung dat gia tri mac dinh la gia tri nay??
	*/
	else if(w< 20)
		m_numMinesTime.SetSizeBar(CSize(12,3));
	else if(w< 25)
		m_numMinesTime.SetSizeBar(CSize(13,3));
	else m_numMinesTime.SetSizeBar(CSize(14,3));

	//khoi tao du lieu va bang choi
	Reset();
	RandMines();
	FillNums();

	//dem= 0;
}

void CMines::GetNeedRect(CRect & rect){
	rect.SetRect(CPoint(0,0),m_pointLT+CPoint(m_nWidth*m_nSize,(m_nHeight+1)*m_nSize));	
}

//chu y voi cac ham nay phai bat chuot o cua so goi ham
void CMines::OnMouseMove(CDC *pDC,UINT nFlags,CPoint point){
	CPoint pos;//se chua tao do theo dong cot bang choi cua chuot
	//xu ly trong truong hop dang nhan chuot trai va di tren bang choi		
	if(GetPosOfPoint(point,pos)){
		if(m_bDownning){
			if(pos!= m_pointOldDown){//truong hop vi tri dang dung khac vi tri cu thi moi xu ly
				//ve o cu ve trang thai binh thuong,tuy vao o do co dat ques hay khong
				if(GetRefState(m_pointOldDown.x,m_pointOldDown.y)== S_QUES)
					Draw1Squ(pDC,m_pointOldDown.x,m_pointOldDown.y,D_QUESTION);
				else
					Draw1Squ(pDC,m_pointOldDown.x,m_pointOldDown.y,D_CLOSE);
				//chi duoc xu ly khi o down dang do'ng hoac co dau ?
				if(GetRefState(pos.x,pos.y)== S_CLOSE||GetRefState(pos.x,pos.y)== S_QUES){			
					//ve down tren o moi
					Draw1Squ(pDC,pos.x,pos.y,D_DOWN);				
					//vi tri nhan chuot cu duoc cap nhat
					m_pointOldDown= pos;//chu y 2 bien nay luu dong cot dang dung chu khong phai toa do tinh theo diem
				}						
			}//end if(pos!= m_pointOldDown)
		}//end if(m_bDownning)
	}else{
	//truong hop di chuot ra ngoai bang choi
	//neu lai di vao thi o down cu (mac du thuc te khong co nua) 
	//nhung no van su dung m_pointOldDown
		//neu van dang nhan chuot
		//if(m_bDownning && m_pointOldDown.x>=0){
		//	Draw1Squ(pDC,m_pointOldDown.x,m_pointOldDown.y,D_CLOSE);
		//	m_pointOldDown= CPoint(-1,-1);
		//}
		;
	}
}
/*
Chu y:
	bien m_pointOldDown se duoc dung nhu sau:
	neu m_bDowning la TRUE thi do la vi tri down truoc do
	nguoc lai thi m_pointOldDown.x duoc su dung nhu sau
		= -1  neu ?
		= -2  neu down tren button newgame
		= -3  neu down tren cho da danh dau hoac tren o da mo
	cac gia tri nay se duoc su kien Up dung de nhan biet da Down o
	dau de xu ly thich hop
*/
void CMines::OnLButtonDown(CDC *pDC,UINT nFlags,CPoint point){	
	//xu ly truong hop click vao button newgame,neu xay ra thi xu ly va
	//thoat luon
	if(m_rectNewGame.PtInRect(point)){
		DrawBntNewGame(pDC,BS_DOWN);
		//dung dau hieu nay de nhan biet co down tren bnt newgame trong LButtonUp
		m_pointOldDown.x= -2; //down tren button newgame
		return;
	}

	//truong hop van choi da ket thuc thi ngoai viec xu ly viec an newgame ra
	//khong xu ly o noi nao khac
	if(!IsPlaying()) return;

	//vi tri down chinh la vi tri m_pointOldDown dau tien
	//vi tri nay se duoc su dung khi m_bDownning co gia tri TRUE	
	if(GetPosOfPoint(point,m_pointOldDown)){
		EMinesState state= GetRefState(m_pointOldDown.x,m_pointOldDown.y);
		//chi duoc xu ly khi o down dang do'ng hoac co dau ?
		if(state== S_CLOSE||state== S_QUES){
			m_bDownning= TRUE;
			//ve down tren o
			Draw1Squ(pDC,m_pointOldDown.x,m_pointOldDown.y,D_DOWN);
		}else{
			m_pointOldDown.x= -3;//down tren o da mo hoac o danh dau
			DrawBntNewGame(pDC,BS_UP,IDB_FORBID);
		}
	}
}
void CALLBACK CallbackSquEmptys(CDC* pDC,CMines * pMines,UINT i,UINT j,BOOL isempty){
	//ham nay duoc goi tuc la co 1 o duoc mo
	//CString str;
	//str.Format("(%d,%d)",i,j);
	//MsgBox(str); 
	//pMines->dem++;
	//truong hop nguoi su dung danh dau mine vao o bi mat di thi can giam so mine danh dau boi nguoi su dung xuong
	if(pMines->GetRefState(i,j)== S_MASK){
		//MessageBox(NULL,"a","a",MB_OK);
		pMines->m_nMineMask--;	
	}
	pMines->m_nCloseRemain--;//giam so o chua mo
	pMines->GetRefState(i,j)= S_OPEN;
	if(isempty) pMines->Draw1Squ(pDC,i,j,D_EMPTY);
    else pMines->Draw1Squ(pDC,i,j,D_NUMBER);
	//TRACE("(%d , %d)",i,j);
}
//ham tra ve 1 neu da click tren newgame va up o vi tri bat ky
//           2 neu clik phai mine
//			 3 neu chien thang
//           0 trong cac truong hop con lai
UINT CMines::OnLButtonUp(CDC * pDC,UINT nFlagd,CPoint point){	
	//m_pointOldDown= CPoint(-1,-1);
	CPoint pos;
	UINT value;
	//xu ly trong truong hop up ma co clk tren bnt newgame
	if(m_pointOldDown.x==-2){
	  DrawBntNewGame(pDC);
	  m_pointOldDown.x=-1;
	  return 1; //da click tren newgame
	} 
	else if(m_pointOldDown.x==-3){//da down tren o da mo hoac o da danh dau
		DrawBntNewGame(pDC);//xu ly o dau co nghia la khong can quan tam den vi tri Up
	}
	if(!IsPlaying()) return 0;
	//chi xu ly up khi co down?	
	if(GetPosOfPoint(point,pos) && m_bDownning){
	//CString str;
	//str.Format("%d %d",pos.x,pos.y);
	//MessageBox(str);
		switch(GetRefState(pos.x,pos.y)){
		case S_QUES:
		case S_CLOSE:
			value= GetValue(pos.x,pos.y);			
			//truong hop click vao o rong
			if(value==0){FindSquEmptys(pos.x,pos.y,(FINDSQUEMPTYS)CallbackSquEmptys,pDC,this); 			
						//do so mine bi danh dau nham bi mat di(so m_nMineMask duoc cap nhat trong ham goi nguoc)
						//nen co the can ve lai so mine
						DrawCntMines(pDC);
						//MsgBoxN(dem);				
			}
			else if(value==-1){
			//truong hop click phai mine,doi trang thai m_bPlaying
				m_nCloseRemain--;//dieu nay la khong can thiet vi luc nay m_bPlaying da la FALSE
				GetRefState(pos.x,pos.y)= S_OPEN;//can dat the nay truoc khi goi ham DrawSqusEnd
				m_pointMineClk= pos;//can gan bien m_pointMineClk truoc khi goi DrawSqusEnd
				DrawSqusEnd(pDC);
				DrawBntNewGame(pDC,BS_UP,IDB_DIE);//ve dau hieu chet				
				m_bPlaying= FALSE;
				m_bDownning= FALSE;//cau lenh quan trong
				return 2;//(click va)up phai mine
			}else{
			//truong hop click vao o co so
				m_nCloseRemain--; //mo them 1 o
				GetRefState(pos.x,pos.y)= S_OPEN;
				Draw1Squ(pDC,pos.x,pos.y,D_NUMBER);
				//coi nhu da kiem tra de khi tim cac o xung quanh o rong thi khong tinh them nhung o da mo nay
				GetSqu(pos.x,pos.y).tested= TRUE;
			}
			break;
		case S_OPEN:
					break;
		}
	}
	m_bDownning= FALSE;
	//truong hop da co chien thang
	if(!m_nCloseRemain){
		DrawBntNewGame(pDC,BS_UP,IDB_WIN);
		//MessageBox(NULL,"Chien thang","Mines",MB_OK);
		return 3;
	}
	return 0;
}
//ham tra ve gia tri 3 neu da co chien thang
//con lai tra ve 0
UINT CMines::OnRButtonDown(CDC *pDC,UINT nFlags,CPoint point){
	CPoint pos;
	if(GetPosOfPoint(point,pos)){
		EMinesState & usestl= GetRefState(pos.x,pos.y);
		switch(usestl){
		case S_CLOSE:
			usestl= S_MASK;
			Draw1Squ(pDC,pos.x,pos.y,D_MASKMINE);
			m_nMineMask++;//tang so mine danh dau len
			//neu o danh dau dung la mine thi giam so o can mo con lai di 1
			if(GetValue(pos.x,pos.y)==-1) m_nCloseRemain--;
			break;
		case S_MASK:
			//truong hop cho phep dat dau?
			if(m_bQues){
				usestl= S_QUES;
				Draw1Squ(pDC,pos.x,pos.y,D_QUESTION);
			}else{//truong hop khong cho dat dau ?
				usestl= S_CLOSE;
				Draw1Squ(pDC,pos.x,pos.y,D_CLOSE);
			}
			//neu o bo danh dau la mine that thi tang so o con dong len 1
			if(GetValue(pos.x,pos.y)==-1) m_nCloseRemain++;
			m_nMineMask--;//chuyen thanh dau?
			break;
		case S_QUES:
			usestl = S_CLOSE;
			Draw1Squ(pDC,pos.x,pos.y,D_CLOSE);
		}
		DrawCntMines(pDC);//ve lai so mines
		//truong hop da co chien thang
		if(!m_nCloseRemain){
			DrawBntNewGame(pDC,BS_UP,IDB_WIN);
			//MessageBox(NULL,"Chien thang","Mines",MB_OK);
			return 3;
		//phai xet xuat hien thang khong tai 2 cho
		}
	}
	return 0;
}
void CMines::DrawCntMines(CDC *pDC){
	CPoint pos;//vi tri dat so
	CSize size= m_numMinesTime.GetSizeNum(3,0);
	CRect rect(m_rectMines);
	DrawButton(pDC,rect,0,BS_UP,FALSE);
	//dat vi tri sao cho so man giua
	pos.x= m_rectMines.left+(m_rectMines.Width()-size.cx)/2;
	pos.y= m_rectMines.top+(m_rectMines.Height()-size.cy)/2;
	m_numMinesTime.ShowNumber(pDC,pos,m_nMines-m_nMineMask,3);//dung 3 o de dat so
}
void CMines::DrawTime(CDC *pDC){
	CPoint pos;
	CSize size= m_numMinesTime.GetSizeNum(4,1);
	CRect rect(m_rectTime);//ham drawbuton thay doi rect dua vao nen can luu lai
	DrawButton(pDC,rect,0,BS_UP,FALSE);
	pos.x= m_rectTime.left+(m_rectTime.Width()-size.cx)/2;
	pos.y= m_rectTime.top+(m_rectTime.Height()-size.cy)/2;
	m_numMinesTime.ShowMinSec(pDC,pos,m_cMin,m_cSec);
}
//khi su dung ham nay chi co 2 trang thai duoc su dung la BS_DOWN,BS_UP
void CMines::DrawBntNewGame(CDC *pDC,EButtonState bs,UINT IDBitmap){
	CRect rect(m_rectNewGame);
	DrawButton(pDC,rect,IDBitmap,bs,FALSE);
}
void CMines::OnTimer(CDC* pDC,UINT nIDEvent){
	m_cSec++;
	if(m_cSec==60){m_cSec=0; m_cMin++;}
	DrawTime(pDC);
}

BOOL & CMines::Marks(){
	return m_bQues;
}

UINT CMines::GetSizeSquare(){
	return m_nSize;
}

CPoint CMines::GetPosBoard(){
	return m_pointLT;
}

void CMines::GetTime(UCHAR & min,UCHAR & sec){
	min= m_cMin; sec= m_cSec;
}