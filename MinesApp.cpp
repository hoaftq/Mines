/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#include "MinesApp.h"
#include "MinesFrame.h"
CMinesApp	g_MinesApp;
BOOL CMinesApp::InitInstance(){
	m_pMainWnd = new CMinesFrame;
	SetRegistryKey("THBT");
	//ham nay chi co the goi o day ?? khong goi duoc o cac ham tao hay OnCreate cua m_pMainWnd	
	((CMinesFrame*)m_pMainWnd)->LoadParameter();
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

CDlgCustom::CDlgCustom(UINT w,UINT h,UINT m,CWnd * pParentWnd):CDialog(IDD_CUSTOM,pParentWnd){
	m_nWidth= w;
	m_nHeight= h;
	m_nMines= m;
	CRect rect(0,0,0,0);	
	UINT size= ((CMinesFrame*)pParentWnd)->m_Mines.GetSizeSquare();	
	CPoint lt= ((CMinesFrame*)pParentWnd)->m_Mines.GetPosBoard();
	pParentWnd->CalcWindowRect(&rect);	
	m_nWidthMax= (::GetSystemMetrics(SM_CXSCREEN)-rect.Width()-lt.x)/size+1; //+1 va -2 de hieu chinh cho chinh xac thanh cong cu
	m_nHeightMax= (::GetSystemMetrics(SM_CYSCREEN)-rect.Height()-lt.y)/size-2;	
	//MsgBoxN(m_nWidthMax);
	//MsgBoxN(m_nHeightMax);
}	
BOOL CDlgCustom::OnInitDialog(){
	DWORD stlcls= ::GetClassLong( m_hWnd,GCL_STYLE);
//	if(!::SetClassLong(m_hWnd,GCL_STYLE,CS_SAVEBITS)) MsgBox("error");
	CHAR str[10];
	wsprintf(str,"(6 -> %d)",m_nWidthMax);	
	SetDlgItemText(IDC_WIDTHMAX,str);
	//MsgBox(str);
	wsprintf(str,"(1 -> %d)",m_nHeightMax);
	GetDlgItem(IDC_HEIGHTMAX)->SetWindowText(str);
	
	//gioi han 2 text nhap with va height
	((CEdit*)GetDlgItem(IDC_WIDTH))->LimitText(2);
	((CEdit*)GetDlgItem(IDC_HEIGHT))->LimitText(2);
	return CDialog::OnInitDialog();
}
void CDlgCustom::DoDataExchange(CDataExchange * pDX){
	DDX_Text(pDX,IDC_WIDTH,m_nWidth);
	DDV_MinMaxUInt(pDX,m_nWidth,6,m_nWidthMax);//6 do kich co nho nhat cua cua so
	DDX_Text(pDX,IDC_HEIGHT,m_nHeight);
	DDV_MinMaxUInt(pDX,m_nHeight,1,m_nHeightMax);
	DDX_Text(pDX,IDC_MINES,m_nMines);
	DDV_MinMaxUInt(pDX,m_nMines,1,m_nWidth*m_nHeight-1);
}
BEGIN_MESSAGE_MAP(CDlgCustom,CDialog)
	ON_EN_SETFOCUS(IDC_MINES,OnEditMinesFocus)
END_MESSAGE_MAP()
void CDlgCustom::OnEditMinesFocus(){
	CHAR str[10];
	UINT temw,temh;
	BOOL result;
	temw= GetDlgItemInt(IDC_WIDTH,&result,FALSE);
	if(result)	temh= GetDlgItemInt(IDC_HEIGHT,&result,FALSE);
	if(result){
		wsprintf(str,"(1 -> %d)",temw*temh-1);
		SetDlgItemText(IDC_MINEMAX,str);
	}
}

CDlgAbout::CDlgAbout(CWnd * pParentWnd):CDialog(IDD_ABOUT,pParentWnd),
m_Num(CSize(8,2),RGB(0,0,0),RGB(192,192,192),0){
	
}
BOOL CDlgAbout::OnInitDialog(){
	return CDialog::OnInitDialog();
}
void CDlgAbout::OnPaint(){
	CPaintDC  dc(this);
	CRect rect;
	GetDlgItem(IDC_DATE)->GetWindowRect(&rect);
	ScreenToClient(&rect);	
	//m_Num.ShowNumber(&dc,CPoint(50,140),134,4);
	//khoi tao thu cong!!!!
	//m_Num.ShowDate(&dc,CPoint(45,130),21,4,2008);
	m_Num.ShowDate(&dc,rect.TopLeft(),21,4,2008);
	GetDlgItem(IDC_DATE)->ShowWindow(SW_HIDE);
}
BEGIN_MESSAGE_MAP(CDlgAbout,CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()




CDlgBestTime::CDlgBestTime(CWnd *pParentWnd):CDialog(IDD_BESTTIMES,pParentWnd){
	m_bRead= TRUE;//o che do chi xem
}
CDlgBestTime::CDlgBestTime(CWnd * pParentWnd,int gamemode,UINT w,UINT h,UINT m,UCHAR min,UCHAR sec):CDialog(IDD_BESTTIMES,pParentWnd){	
	m_bRead= FALSE;//o che do cho phep ghi
	m_iGameMode= gamemode;
	m_nWidth= w;
	m_nHeight= h;
	m_nMines= m;
	m_cMin= min;
	m_cSec= sec;	
}

BOOL CDlgBestTime::OnInitDialog(){
	LoadFile();//doc file va hien thi cac thong so cu~
	
	//truong hop cho phep ghi highscore moi
	if(!m_bRead){
		//cho phep cac ctrl can thiet cho viec nhap
		GetDlgItem(IDC_NEW_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_NEW_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_NEW_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_NEW_NAME))->SetLimitText(15);
		CHAR str[15];//15 la vi ten toi da la 15 ky tu
		//memset(str,' ',15);//xoa vung dem
		wsprintf(str,"%2d:%2d",m_cMin,m_cSec);	
		SetDlgItemText(IDC_NEW_TIME,str);//lay thoi gian hien tai va ghi vao static thoi gian
		//memset(str,' ',15);//xoa vung dem(co le la khong can)
		//lay chuoi game mode
		switch(m_iGameMode){
		case ID_FILE_NEW_BEGINNER:
			strcpy(str,"Beginner"); break;
		case ID_FILE_NEW_INTERMEDIATE:
			strcpy(str,"Intermediate"); break;
		case ID_FILE_NEW_EXPERT:
			strcpy(str,"Expert"); break;
		case ID_FILE_NEW_CUSTOM:
			wsprintf(str,"%d-%d-%d",m_nWidth,m_nHeight,m_nMines);		
		}		
		SetDlgItemText(IDC_NEW_MODE,str);
	}	
	return CDialog::OnInitDialog();
}
//ham doc cac highscore va hien thi
void CDlgBestTime::LoadFile(){
	int i;
	SHighScore  buffer;
	try{
		//cho phep file duoc truy cap boi nhieu parties
		CFile file(g_szFileName,CFile::modeRead|CFile::shareDenyNone);				
		for(i=0; i< 4; i++){			
			if(file.Read(&buffer,sizeof(SHighScore))==sizeof(SHighScore)){
				SetDlgItemText(i+IDC_NAME_BEGINNER,buffer.name);
				SetDlgItemText(i+IDC_TIME_BEGINNER,buffer.time);			
			}
		}
		//truong hop custom thi o cot mode ghi ro kich thuoc bang choi va so mine
		//con cac truong hop khac la cac static co dinh 
		SetDlgItemText(IDC_MODE_CUSTOM,buffer.mode);
	}catch(CFileException *err) {		
		//loi co the xay ra khi khong tim thay file
		//luu do hop thoai hien thi nhu o trang thai thiet ke
		//err->ReportError();		
		AfxGetMainWnd()->MessageBox("Mines.hsc was not found",NULL,MB_OK|MB_ICONEXCLAMATION);
		err->Delete();
	}
}

void CDlgBestTime::OnOK(){
	//neu dang o che do ghi moi
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled()) OnApply();
	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgBestTime,CDialog)
	ON_BN_CLICKED(IDC_APPLY,OnApply)
END_MESSAGE_MAP()

void CDlgBestTime::OnApply(){
	//Khi nguoi su dung cap nhat gia tri diem cao moi
	SHighScore hs;
	int pos = m_iGameMode-ID_FILE_NEW_BEGINNER;
	GetDlgItemText(IDC_NEW_NAME,hs.name,sizeof(hs.name));
	SetDlgItemText(IDC_NAME_BEGINNER+pos,hs.name);//cap nhat vao static tuong ung
	//MsgBox(hs.name);
	GetDlgItemText(IDC_NEW_TIME,hs.time,sizeof(hs.time));
	SetDlgItemText(IDC_TIME_BEGINNER+pos,hs.time);//cap nhat vao static tuong ung
	//MsgBox(hs.time);
	GetDlgItemText(IDC_NEW_MODE,hs.mode,sizeof(hs.mode));
	if(m_iGameMode==ID_FILE_NEW_CUSTOM) SetDlgItemText(IDC_MODE_CUSTOM,hs.mode);//cap nhat vao static tuong ung
	//MsgBox(hs.mode);	
	//cap nhat vao file
	WriteFile(g_szFileName,&hs,m_iGameMode-ID_FILE_NEW_BEGINNER);
}