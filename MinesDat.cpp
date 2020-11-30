/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#include "MinesDat.h"
//ham nay tra ve trang thai(ve gia tri) cua o dua vao gia tri cua o
EMinesValue GetStateValue(int value){
	switch(value){
	case -1:
		return V_MINE;
	case 0:
		return V_EMPTY;		
	default:
		if(value>=1) return V_NUMBER;
	}
}

CMinesDat::CMinesDat(){
	srand(time(NULL));//chi khoi dong bo tao so ngau nhien 1 lan,khi khoi dong
	m_nWidth= m_nHeight= m_nMines= 0;
}
CMinesDat::CMinesDat(UINT w,UINT h,UINT m){
	srand(time(NULL));
	SetSize(w,h,m);
}
CMinesDat::~CMinesDat(){
	m_nWidth= m_nHeight= m_nMines= 0;
	squ.RemoveAll();
	squ.FreeExtra();
}
void CMinesDat::SetSize(UINT w,UINT h,UINT m){
	if(m_nWidth!= w ||m_nHeight!= h){
		m_nWidth= w;
		m_nHeight= h;		
		squ.SetSize(h*w);
	}
	m_nMines= m;
	//dua gia tri cua tat ca cac o trong bang choi ve 0
	Reset();
}
void CMinesDat::GetSize(UINT & w,UINT & h,UINT & m)const {
	w= m_nWidth;
	h= m_nHeight;
	m= m_nMines;
}
//lay tham chieu den trang thai do nguoi su dung
//chu y i la chi so cua hang,j la chi so cot
EMinesState & CMinesDat::GetRefState(UINT i,UINT j){
	return squ[i*m_nWidth+j].state;
}
//dat trang thai nguoi su dung
void CMinesDat::SetState(UINT i,UINT j,EMinesState state){
	GetRefState(i,j)= state;
}
//lay gia tro cua o (i,j), co the dung gia tri nay hoac ham GetStateValue de chuyen ve state
UINT CMinesDat::GetValue(UINT i,UINT j) const{
	return squ[i*m_nWidth+j].value;
}
//lay tham chieu cua du lieu, la thanh phan protected
SMinesSqu & CMinesDat::GetSqu(UINT i,UINT j){
	return squ[i*m_nWidth+j];
}
void CMinesDat::RandMines(){
	UINT i;
	CUIntArray arrcntms;
	RandRowCntMines(arrcntms);//arrcntms chua so mines cua cac hang
	//duyet qua tat ca cac hang va dien mines cho moi hang
	for(i= 0; i<m_nHeight; i++){
		FillMinesRow(arrcntms[i],i);
	}
} 
void CMinesDat::FillNums(){
	UINT i,j;
	//duyet qua tat ca cac o
	for(i=0; i< m_nHeight; i++)
		for(j=0; j< m_nWidth; j++){
			//neu o dang xet khong phai la mine thi hoac la o rong hoac phai dien so			
			if(GetStateValue(GetSqu(i,j).value)!= V_MINE){				
				UINT cntmines= 0;
				//kiem tra cac vi tri xung quanh o (i,j)
				if(j>=1 && GetSqu(i,j-1).value== -1) cntmines++;
				if(j<m_nWidth-1 && GetSqu(i,j+1).value ==-1) cntmines++;
				if(i>=1 && GetSqu(i-1,j).value ==-1) cntmines++;
				if(i<m_nHeight-1 && GetSqu(i+1,j).value ==-1) cntmines++;
				if(i>=1 && j>=1 && GetSqu(i-1,j-1).value ==-1) cntmines++;
				if(i>=1 && j< m_nWidth-1 && GetSqu(i-1,j+1).value ==-1) cntmines++;
				if(i< m_nHeight-1 && j>=1 && GetSqu(i+1,j-1).value ==-1) cntmines++;
				if(i< m_nHeight-1 && j< m_nWidth-1 && GetSqu(i+1,j+1).value ==-1) cntmines++;
				GetSqu(i,j).value= cntmines;
				//!neu cntmines==0 thi co nghia la o rong
			}						
		}
}

void CMinesDat::RandRowCntMines(CUIntArray & arrcntms){	
	UINT i,total=0;
	UINT j;
	arrcntms.SetSize(m_nHeight);
	//moi hang duoc gan cho 1 so se su dung nhu so ty le voi so mines
	//tong cua cac so nay la total
	for(i=0; i< m_nHeight; i++){
		arrcntms.SetAt(i,rand() %(m_nWidth-1)+1);//so trong nay khong quan trong  
		total+=arrcntms.GetAt(i);
	}
	//TRACE1("So mines= %d\n",m_nMines);
	//TRACE1("total= %d\n",total);
	//tinh so mines cua moi hang sao cho tong so mines la m_nMines
	//va so mines cua moi hang ty le voi so trong attcntms o tren
	//so mines tuong ung luu lai vao arrcntms
	for(i=0; i< m_nHeight; i++){
		UINT temcnt= (UINT)(0.5+(float)arrcntms.GetAt(i)*m_nMines/total);
		//truong hop so mine trong hang lai nhieu hon so o trong hang
		if(temcnt> m_nWidth) temcnt= m_nWidth;
		arrcntms.SetAt(i,temcnt);
		//TRACE2("So thu %d la %d\n",i,arrcntms[i]);
	}
	//tinh lai so mines de them hoac bot cho phu hop
	total= 0;
	for(i=0; i< m_nHeight; i++){
		total+= arrcntms.GetAt(i);
	}
	/*
	MsgBox("a");
	MsgBoxN(m_nHeight);	
	CString str,s;
	for(i= 0; i<m_nHeight; i++){
		s.Format("%d ",arrcntms.GetAt(i));
		str+= s;
	}
	MessageBox(NULL,str,"so min moi hang",MB_OK);
	MsgBoxN(total);
	*/

	if(total< m_nMines){
	//MsgBox("here");
	//truong hop so mines van con be hon so mines phai co
		for(i=1; i<= m_nMines-total; i++){
			j= rand()% m_nHeight;//j chua chi so cua 1 hang random			
			//neu so mines o hang j lon hon ca so o trong hang thi chuyen sang hang tiep theo
//			if(j==16) MsgBox("error");
			while(arrcntms.GetAt(j)>= m_nWidth){//-1				
				j++;
				if(j>=m_nHeight) j=0;//sai o day do dat m_nWidth ma mai khong tim ra								
				//j--; if(j<0) j= m_nWidth-1;
				//MessageBox(NULL,"!","title",MB_OK);
				//truong hop nay duong nhu khong bao gio xay ra
			}
			//tang so mines cua hang duoc chon len 1						
			//arrcntms.SetAt(j,arrcntms.GetAt(j)+1);						
			arrcntms[j]++; 
			//TRACE("i= %d arrcntms[i]= %d",j,arrcntms[j]);
		}
	}else if(total> m_nMines){
		//MsgBox("here2");
	//truong hop so mines nhieu hon so mines phai co
		for(i=1; i<= total-m_nMines; i++){
			j= rand()% m_nHeight;
			//neu so mines o hang j la 0 thi xet tiep cac hang tiep theo
			while(arrcntms.GetAt(j)== 0){
				j++;
				if(j>= m_nWidth) j= 0;
			}
			//sau vong lap j chua chi so co arrcntms[j] != 0
			arrcntms.SetAt(j,arrcntms.GetAt(j)-1);
		}
	}
	/*
	//CString str,s;
	str= "";
	for(i= 0; i<m_nHeight; i++){
		s.Format("%d ",arrcntms.GetAt(i));
		str+= s;
	}
	MessageBox(NULL,str,"2 so min moi hang",MB_OK);
	*/
	//MsgBox("here3");
}
void CMinesDat::FillMinesRow(UINT cntmines,UINT row){
	UINT i,j,pos;
	CString str,s;//dung de sua loi
	//duyet qua cac phan tu trong 1 hang
	for(i=0; i< cntmines; i++){
		pos= rand()% (m_nWidth-i);//vi tri se dat mines trong cac vi tri chua co mines con lai
		//tim ra vi tri thuc cua o se dat mine
		UINT cnt = -1;//dem xem da qua bao nhieu o chua dien mine
		for(j= 0; j< m_nWidth; j++){
			//neu o duyet qua khong phai la mine thi tang so dem offset len 1
			if(GetStateValue(GetSqu(row,j).value)!=	V_MINE) cnt++;		
			if(cnt== pos){
			//da den vi tri thuc can tim
				//s.Format("%d ",j);
				//str+=s;
				GetSqu(row,j).value= -1;//dat o (row,j) la mines
				break;
			}			
		}
	}
	//MessageBox(NULL,str,"info",MB_OK);
}
//ham dat tat ca cac bien ve trang thai binh thuong,can thiet cho van
//choi moi	
void CMinesDat::Reset(){
	UINT i,j;
	for(i=0; i< m_nHeight; i++)
		for(j=0; j<m_nWidth; j++){
			GetSqu(i,j).state= S_CLOSE;
			GetSqu(i,j).value = 0;
			GetSqu(i,j).tested= FALSE;
		}
}
/*
void  CMinesDat::Print(){
	UINT i,j;
	CString str,s;
	for(i=0; i< m_nHeight; i++){
		for(j=0; j<m_nWidth; j++){
			if(GetSqu(i,j).value==-1)
				s= "x";
			else
				s.Format("%d",GetSqu(i,j).value);
			str+= s;
		}
		str+="\n";
	}
	MessageBox(NULL,str,"table",MB_OK);
}
*/
/*Do ham nay co sat voi du lieu nen dat o base class nay
  Khi goi ham nay can cung cap 2 doi so kieu CDC* va CMines *
  cac doi so nay se duoc cung cap cho ham goi nguoc de ham goi
  nguoc co the truy nhap den doi tuong CMines co day du cac ham
  ve va co Dc de ve
*/

void CMinesDat::FindSquEmptys(UINT i,UINT j,FINDSQUEMPTYS FindSquEmptysProc,CDC * pDC,CMines *pMines){
	//goi ham nay khi da biet i,j la 1 o rong	
	//dem++;
	GetSqu(i,j).tested= TRUE;
	FindSquEmptysProc(pDC,pMines,i,j,TRUE);//goi ham xu ly o rong
	//xet tiep o ben canh la (i,j-1) (voi dieu kien la j>=1)
	//neu la o rong thi de qui o nay
	//neu khong tuc la 1 o so thi goi ham nguoc de xu ly
	if(j>=1  && !GetSqu(i,j-1).tested){
		if(GetSqu(i,j-1).value== 0) FindSquEmptys(i,j-1,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i,j-1).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i,j-1,FALSE);//FALSE nghia la o so			
		}
			//!khong the xay ra truong hop o day la mines
	}
	if(j< m_nWidth-1 && !GetSqu(i,j+1).tested){
		if(GetSqu(i,j+1).value==0) FindSquEmptys(i,j+1,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i,j+1).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i,j+1,FALSE);//xu ly o chua so			
		}
	}
	if(i>=1 && !GetSqu(i-1,j).tested){
		if(GetSqu(i-1,j).value==0) FindSquEmptys(i-1,j,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i-1,j).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i-1,j,FALSE);			
		}
	}
	if(i< m_nHeight-1 && !GetSqu(i+1,j).tested){
		if(GetSqu(i+1,j).value==0) FindSquEmptys(i+1,j,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i+1,j).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i+1,j,FALSE);			
		}
	}
	if(i>=1 && j>= 1 && !GetSqu(i-1,j-1).tested){
		if(GetSqu(i-1,j-1).value==0) FindSquEmptys(i-1,j-1,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i-1,j-1).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i-1,j-1,FALSE);		
		}
	}
	if(i>=1 && j< m_nWidth-1 && !GetSqu(i-1,j+1).tested){
		if(GetSqu(i-1,j+1).value==0) FindSquEmptys(i-1,j+1,FindSquEmptysProc,pDC,pMines);
		else{
		      GetSqu(i-1,j+1).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i-1,j+1,FALSE);
		}
	}
	if(i< m_nHeight-1 && j>=1 && !GetSqu(i+1,j-1).tested){
		if(GetSqu(i+1,j-1).value==0) FindSquEmptys(i+1,j-1,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i+1,j-1).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i+1,j-1,FALSE);
		}
	}
	if(i<m_nHeight-1 && j< m_nWidth-1 && !GetSqu(i+1,j+1).tested){
		if(GetSqu(i+1,j+1).value==0) FindSquEmptys(i+1,j+1,FindSquEmptysProc,pDC,pMines);
		else{
			  GetSqu(i+1,j+1).tested = TRUE;
			  FindSquEmptysProc(pDC,pMines,i+1,j+1,FALSE);
		}
	}
}
/*
void CMinesDat::ResetTested(){
	UINT i,j;
	for(i=0; i< m_nHeight; i++)
		for(j=0; j<m_nWidth; j++){
			GetSqu(i,j).tested= FALSE;
		}	
}
*/