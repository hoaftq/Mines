/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#include "MinesFrame.h"
CMinesFrame::CMinesFrame(){
	CString wndclass= 
		AfxRegisterWndClass( CS_BYTEALIGNCLIENT,
							 AfxGetApp()->LoadStandardCursor(IDC_ARROW),
							 0,
							 0
						   );
	Create( wndclass,
			"Mines",
			WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU
		   );
	//enable tat ca cac menu,khong can anh xa thong diep
	//can thiet vi khi xu ly trong OnCommand thi anh xa thong diep duoc tao boi khung ung dung
	m_bAutoMenuEnable =FALSE;
}
BOOL CMinesFrame::PreCreateWindow(CREATESTRUCT & cs){
	if(!CFrameWnd::PreCreateWindow(cs)) return FALSE;
	//cs.style &= ~CS_DBLCLKS;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.hMenu= LoadMenu(NULL,MAKEINTRESOURCE(IDR_MAINFRAME));
	return TRUE;
}
#define		ID_TIMER	1
int CMinesFrame::OnCreate(LPCREATESTRUCT lpcs){
	if(CFrameWnd::OnCreate(lpcs)==-1) return -1;
	SetIcon(LoadIcon(lpcs->hInstance,MAKEINTRESOURCE(IDI_MAINFRAME)),FALSE);	
	LoadAccelTable(MAKEINTRESOURCE(IDR_ACCELERATOR));	
	//SetTimer(ID_TIMER,1000,NULL); khong can da dat trong newgame
	//khoi tao game ngay khi khoi dong: se duoc goi trong LoadParameter
	CMenu * sysmenu= GetSystemMenu(FALSE);
	sysmenu->AppendMenu(MF_SEPARATOR);
	sysmenu->AppendMenu(MF_STRING,ID_HELP_ABOUT,"About Mines");
	
	//luu so item ban dau cua menu File
	m_nMenuFileItemCnt= GetMenu()->GetSubMenu(0)->GetMenuItemCount();
	//ham UpdateRecentMenu khong the dat o day vi cac thong so tu register chua duoc doc khi thuc hien ham nay	
	//chi co the dat trong LoadParameter vi ham nay duoc CMinesApp goi sau khi da tao xong cua so
	//hoac dat trong ham xu ly thong diep menu?? cu the o dau ?
	return 0;
}
void CMinesFrame::OnTimer(UINT nIDEvent){
	CDC *pDC= GetDC();
	m_Mines.OnTimer(pDC,nIDEvent);
	ReleaseDC(pDC);
}
void CMinesFrame::OnMouseMove(UINT nFlags,CPoint point){
	//chi xu ly su kien khi dang o trang thai choi duoc
	if(m_Mines.IsPlaying()){
		CDC * pDC= GetDC();
		m_Mines.OnMouseMove(pDC,nFlags,point);
		ReleaseDC(pDC);
	}
}
void CMinesFrame::OnLButtonDown(UINT nFlags,CPoint point){		
		CDC * pDC= GetDC();		
		SetCapture();
		m_Mines.OnLButtonDown(pDC,nFlags,point);
		ReleaseDC(pDC);
}
void CMinesFrame::OnLButtonUp(UINT nFlags,CPoint point){
	//if(m_Mines.IsPlaying()){
		CDC * pDC= GetDC();
		switch(m_Mines.OnLButtonUp(pDC,nFlags,point)){
		case 1://da click tren newgame
			OnFileNewGame(); break;
		case 2://click va up phai mine
			KillTimer(ID_TIMER); break;
		case 3://da click vao o so hay rong cuoi cung(co nghia la chien thang)
			KillTimer(ID_TIMER);//dung time lai
			Win();//kiem tra xem co can cap nhat diem khong
		}
		ReleaseDC(pDC);
		ReleaseCapture();
	//}
}
void CMinesFrame::OnRButtonDown(UINT nFlags,CPoint point){
	if(m_Mines.IsPlaying()){
		CDC *pDC= GetDC();
		//neu gia tri cho lai la 3 co nghia la da co chien thang		
		if(m_Mines.OnRButtonDown(pDC,nFlags,point)==3){ 
			KillTimer(ID_TIMER);
			Win();//kiem tra xem co can cap nhat diem khong
		}
		ReleaseDC(pDC);
	}
}
BOOL CMinesFrame::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}
void CMinesFrame::OnPaint(){
	CPaintDC dc(this);
	CDC dcMem;
	CBitmap bmp,*oldbmp;
	CRect rect;
	CBrush br(GetSysColor(COLOR_BTNFACE));
	dcMem.CreateCompatibleDC(&dc);
	GetClientRect(&rect);
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	oldbmp= dcMem.SelectObject(&bmp);
	dcMem.FillRect(&rect,&br);//to mau cho nen
	//goi ham ve bang choi o trang thai cac trang thai
	m_Mines.ShowBoard(&dcMem);
	//GetUpdateRect(&rect);
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);	
	//dc.BitBlt(rect.left,rect.top ,rect.Width(),rect.Height(),&dcMem,rect.left,rect.top,SRCCOPY);	
	dcMem.SelectObject(oldbmp);
}
void CMinesFrame::NewGame(UINT w,UINT h,UINT m){
	CRect rect;
	SetTimer(ID_TIMER,1000,NULL);
	m_Mines.NewGame(w,h,m);	
	//goi ham tinh kich thuoc can thiet cua cua so choi	
	m_Mines.GetNeedRect(rect);
	CalcWindowRect(&rect);
	SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOMOVE|SWP_NOZORDER);	
}
void CMinesFrame::OnFileNewGame(){
	//NewGame(30,16,99);
	if(m_iGameMode!= ID_FILE_NEW_CUSTOM) OnFileNew(m_iGameMode);
	else{
		UINT w,h,m;
		m_Mines.GetSize(w,h,m);
		NewGame(w,h,m);
	}
	Invalidate();//ve lai bang choi
}
//xu ly truong hop khi tao 1 custom moi trung voi custom cu
//hoac khi click vao custom menu thi can dua menu custom nay len vi tri dau
//pos la vi tri trong mang m_strRecentMode la mode cu se duoc lam moi
void CMinesFrame::Old2NewRecentMenu(char pos){
	//tu id cua menu click tinh ra vi tri trong m_strRecentMode
	//hoac da biet vi tri cua chuoi bi click 
	//can don cac vi tri tren ve tram vao cho vi tri nay
	char c;//bien dem
	CString str= m_strRecentMode[pos];//luu lai string o vi tri pos
	//don de len vi tri pos
	for(c=pos; c<m_cRecentCnt-1; c++)
		m_strRecentMode[c]= m_strRecentMode[c+1];
	m_strRecentMode[m_cRecentCnt-1]= str;//ghi vao vi tri co index lon nhat,tuc la moi nhat
}
//ham nay duoc goi khi thuc su co thay doi ve menu,nen khong can kiem tra xem menu co thuc su bi thay doi hay khong trong ham nay
//ham nay su dung mang m_strRecentMode de cap nhat cho menu,voi chu y la index cang cao thi cang moi
//ham nay duoc goi khi bat dau ung dung va khi nguoi su dung custom 1 mode moi
void CMinesFrame::UpdateRecentMenu(){
	char c;//bien dem
	CMenu * mnuFile= GetMenu()->GetSubMenu(0);//lay menu File la menu dau tien	
	UINT itemcnt= mnuFile->GetMenuItemCount();//so phan tu hien tai
	UINT itemadded= itemcnt-m_nMenuFileItemCnt;//so item da them vao trong menu file so voi ban dau
	//neu chua co menu nao duoc them vao thi truoc het them separator
	//va tang so item len 1
	//truong hop nay chi xay ra khi goi ham nay luc khoi dong
	if(itemadded==0){
		mnuFile->InsertMenu(m_nMenuFileItemCnt-2,MF_BYPOSITION,MF_SEPARATOR);
		itemcnt++;//so item cua menu tang them 1
	}
	else itemadded--;//ko dem separator vi luc do da them separator roi 
	//truong hop so menu them vao van chua du 4 menu custom
	if(itemadded<MAX_RECENTMODE){
		//neu chua du 4 item ma co them custom moi thi them item moi vao
		//id lon nhat cua menu da them vao,dung ID nay de tinh ID cua menu them tiep vao
		UINT IDAdd= ID_FILE_RECENT1+itemadded;

		//m_cRecentCnt la so muc recent can co trong menu,toi da la 4
		//itemadded la so item da them vao 
		//nhu vay can them vao m_cRecentCnt-itemadded item moi
		//cac menu duoc insert tu ID thap den cao,tu vi tri tren den vi tri duoi		
		for(c=0; c<m_cRecentCnt-itemadded; c++){
			//itemcnt la so item trong menu truoc khi goi ham nay
			//sau khi insert them 1 menu thi chi so cua vi tri can chen phai tang them 1
			//chen vao vi tri thu 2 duoi len
			mnuFile->InsertMenu(itemcnt-2+c,MF_BYPOSITION,IDAdd+c);
		}
	}
	//duyet qua danh sach cac recent mode va ghi vao cac item da tao ra
	//chu y la ghi nguoc voi index de dam bao mode moi nhat duoc ghi dau tien
	for(c=0; c<m_cRecentCnt; c++){
		mnuFile->ModifyMenu(ID_FILE_RECENT1+c,MF_BYCOMMAND,ID_FILE_RECENT1+c,m_strRecentMode[m_cRecentCnt-1-c]);
	}
}
void CMinesFrame::OnFileNew(UINT nID){
	UINT w,h,m;	
	switch(nID){
	case ID_FILE_NEW_BEGINNER:
			NewGame(9,9,10); break;
	case ID_FILE_NEW_INTERMEDIATE:
			NewGame(16,16,40); break;
	case ID_FILE_NEW_EXPERT:						
			NewGame(30,16,99); break;
	case ID_FILE_NEW_CUSTOM:
			//MessageBox("precustom");
			m_Mines.GetSize(w,h,m);
			CDlgCustom dlgcustom(w,h,m,this);//hop thoai co gia tri khoi tao lay o game hien tai			
			if(dlgcustom.DoModal()==IDOK){
				//MessageBox("custom");
				NewGame(dlgcustom.m_nWidth,dlgcustom.m_nHeight,dlgcustom.m_nMines); 
				
				//so sanh va ghi vao recent game
				CString recent;
				char c;//bien dem
				recent.Format("%2d-%2d-%4d",dlgcustom.m_nWidth,dlgcustom.m_nHeight,dlgcustom.m_nMines);
				//MessageBox(recent);
				for(c=0; c<m_cRecentCnt; c++){
					if(recent== m_strRecentMode[c]){
						Old2NewRecentMenu(c);	
						goto end;//sau khi don lai menu thi cap nhat lai menu
					}
				}
				//truong hop chua du 4 recent mode thi ghi tiep vao
				if(m_cRecentCnt<MAX_RECENTMODE){
					m_cRecentCnt++;					
					m_strRecentMode[m_cRecentCnt-1]= recent;
					//cach viet nay dan den index cang cao thi cang moi
				}
				else{
				//neu da du thi do^`n lai cho(nghia la mode cu nhat(o vi tri 0) se bi ghi de					
					for(c=0; c<m_cRecentCnt-1; c++)
						m_strRecentMode[c]= m_strRecentMode[c+1];
					m_strRecentMode[m_cRecentCnt-1]=recent;//ghi mode moi vao
				}
end:			UpdateRecentMenu();//co thay doi recentmenu goi ham cap nhat
			}
	}
	m_iGameMode= nID;
	Invalidate();
}
void CMinesFrame::OnUpdateFileNew(CCmdUI *pUI){
	pUI->SetRadio(pUI->m_nID== m_iGameMode);
}
void CMinesFrame::OnFileOption(){
	CDC *pDC= GetDC();
	//m_Number.DrawBar(pDC,CPoint(10,10),FALSE,FALSE);
	//m_Number.DrawFigure(pDC,CPoint(10,100),10);
	//m_Number.ShowNumber(pDC,CPoint(30,10),-34,3);
	//m_Number.DrawColon(pDC,CPoint(20,10),TRUE);
	//m_Number.ShowMinSec(pDC,CPoint(20,10),12,24);
	m_Mines.DrawCntMines(pDC);
	m_Mines.DrawTime(pDC);
	ReleaseDC(pDC);
}
void CMinesFrame::OnClose(){
	//neu time dang hoat dong moi giai phong no
	if(m_Mines.IsPlaying()) KillTimer(ID_TIMER);
	SaveParameter();
	CFrameWnd::OnClose();
}
//ham save cac tham so cua cua so va tham so cua game
void CMinesFrame::SaveParameter(){
	WINDOWPLACEMENT wndpl;
	wndpl.length= sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wndpl);
	AfxGetApp()->WriteProfileInt("Window","show",wndpl.showCmd);
	AfxGetApp()->WriteProfileInt("Window","left",wndpl.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt("Window","top",wndpl.rcNormalPosition.top);

	AfxGetApp()->WriteProfileInt("Game","mark",m_Mines.Marks());
	AfxGetApp()->WriteProfileInt("Game","mode",m_iGameMode);		
	UINT w,h,m;
	m_Mines.GetSize(w,h,m);
	AfxGetApp()->WriteProfileInt("Game","width",w);
	AfxGetApp()->WriteProfileInt("Game","height",h);
	AfxGetApp()->WriteProfileInt("Game","mines",m);
	
	//duyet qua tat ca cac recent mode va ghi vao "mode 1","mode 2",...
	CHAR szEntry[]= "mode 0";
	for(char c=0; c<m_cRecentCnt; c++){
		AfxGetApp()->WriteProfileString("Recent mode",szEntry,m_strRecentMode[c]);
		szEntry[strlen(szEntry)-1]= c+'1';
	}
}
//ham load cac tham so can thiet va tinh toan kich thuoc cua cua so
//cho gia tri TRUE neu da dat duoc left va top cho cua so
void CMinesFrame::LoadParameter(){	
	UINT cmdshow= AfxGetApp()->GetProfileInt("Window","show",SW_SHOW);
	UINT l,t;
	CRect rect;	
	//doc gia tri cho phep danh dau ? hay khong
	m_Mines.Marks()= AfxGetApp()->GetProfileInt("Game","mark",TRUE);

	//hai ham nay khong dat trong OnCreate duoc vi trong do khong the dung AfxGetApp()->GetProfileString
	//dat o day thi thuc te 2 ham nay duoc goi sau OnCreate->cang tot
	m_iGameMode= AfxGetApp()->GetProfileInt("Game","mode",ID_FILE_NEW_BEGINNER);	
	UINT w,h,m;
	w= AfxGetApp()->GetProfileInt("Game","width",9);
	h= AfxGetApp()->GetProfileInt("Game","height",9);
	m= AfxGetApp()->GetProfileInt("Game","mines",10);
	NewGame(w,h,m);
	//OnFileNew(m_iGameMode);//trong ham nay da co viec dat kich thuoc cua so,nhung co the dat lai trong LoadParameter
	//goi ham ghi lai kich thuoc cua cua so
	
	//do ham newgame duoc goi truoc ham nay, no da dat width va height cho cua so
	//bay gio chi can dat left va top
	if((l= AfxGetApp()->GetProfileInt("Window","left",INT_MIN))==INT_MIN ||
	   (t= AfxGetApp()->GetProfileInt("Window","top",INT_MIN))==INT_MIN)
	{//truong hop doc registry bi loi
	 //tinh toan dat cua so o chinh giua man hinh		
		GetWindowRect(&rect);
		SetWindowPos(NULL,(::GetSystemMetrics(SM_CXSCREEN)-rect.Width())/2,
				     (::GetSystemMetrics(SM_CYSCREEN)-rect.Height())/2,0,0,
					 SWP_NOZORDER|SWP_NOSIZE);		
		ShowWindow(SW_NORMAL);		
		//MsgBox("dat giua man hinh");
	}else{
		SetWindowPos(NULL,l,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
		ShowWindow(cmdshow);
	}
	
	//doc 4 mode gan day nhat
	m_cRecentCnt=0;//rat can thiet	
	CHAR szEntry[]= "mode 0";
	for(char c=0; c<MAX_RECENTMODE; c++){
		if((m_strRecentMode[c]= AfxGetApp()->GetProfileString("Recent mode",szEntry,NULL))!=""){	
			//MessageBox(m_strRecentMode[c]);
			m_cRecentCnt++;
		}
		szEntry[strlen(szEntry)-1]= c+'1';
		//MsgBox(szEntry);
	}
	
	//cap nhat menu khi khoi dong
	//khi ma m_cRecentCnt==0 thi co nghia la chua co menu custom nao
	if(m_cRecentCnt!=0) UpdateRecentMenu();
}

BEGIN_MESSAGE_MAP(CMinesFrame,CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_FILE_NEW,OnFileNewGame)
	ON_COMMAND_RANGE(ID_FILE_NEW_BEGINNER,ID_FILE_NEW_CUSTOM,OnFileNew)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_NEW_BEGINNER,ID_FILE_NEW_CUSTOM,OnUpdateFileNew)
	ON_COMMAND(ID_FILE_OPTION,OnFileOption)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_ABOUT,OnHelpAbout)
	ON_COMMAND(ID_HELP_TOPIC,OnHelpTopic)
	ON_COMMAND(ID_FILE_MARKS,OnFileMarks)
	ON_UPDATE_COMMAND_UI(ID_FILE_MARKS,OnUpdateFileMarks)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_EXIT,OnFileExit)
	ON_COMMAND(ID_FILE_BESTTIME,OnFileBestTimes)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

void CMinesFrame::OnHelpAbout(){
	CDlgAbout dlgabout(this);
	dlgabout.DoModal();
}
void CMinesFrame::OnHelpTopic(){
	//Win();
	//UpdateRecentMenu();
	HINSTANCE hIns;
	hIns= ShellExecute(AfxGetMainWnd()->m_hWnd,"open","Help\\mineshelp.htm",NULL,NULL,SW_NORMAL);
	if((int)hIns<= 32) MessageBox("Error open 'mineshelp.htm'! ","Mines",MB_OK|MB_ICONERROR);
}

void CMinesFrame::OnFileMarks(){
	m_Mines.Marks()= !m_Mines.Marks();
}
void CMinesFrame::OnUpdateFileMarks(CCmdUI *pUI){
	pUI->SetCheck(m_Mines.Marks());
}
void CMinesFrame::OnSysCommand(UINT nID,LPARAM lParam){
	 if(nID== ID_HELP_ABOUT) OnHelpAbout();
	 CFrameWnd::OnSysCommand(nID,lParam);
}
void CMinesFrame::OnFileExit(){
	SendMessage(WM_CLOSE);
}
void CMinesFrame::OnFileBestTimes(){
	//hien thi hop thoai xem diem(vi la ham khoi tao 1 doi so)
	CDlgBestTime dlgbesttime(this);		
	dlgbesttime.DoModal();		
}

void CMinesFrame::WriteDefaultFile(){
	SHighScore hs;
	CFile file(g_szFileName,CFile::modeWrite|CFile::modeCreate);
	memset(hs.name,'-',sizeof(hs.name)-1);
	hs.name[sizeof(hs.name)-1]= 0;
	strcpy(hs.time,"99:99");
	//memset(hs.time,'-',sizeof(hs.time)-1);
	//hs.time[sizeof(hs.time)-1]= 0;		
	//memset(hs.mode,'-',sizeof(hs.mode)-1); khong can vi khong su dung cac tham so nay khi doc
	//hs.mode[sizeof(hs.mode)-1]= 0;
	//chi su dung moi hs.mode trong truong hop custom
	for(int i=0; i< 4; i++){
		if(i==3) strcpy(hs.mode,"Custom");
		file.Write(&hs,sizeof(hs));
	}
}
//goi ham nay khi co chien thang,no doc thoi gian tot nhat trong file
//va thuc hien so sanh voi thoi gian hien tai neu nho hon thi no
//hien ra hop thoai yeu cau nhap ten.
//neu file chua ton tai thi no ghi file mac dinh va hien ra hop
//thoai yeu cau nhap ten
//chu y khi ghi vao (o dialog) phai dung dinh dang
void CMinesFrame::Win(){
	SHighScore hs;
	UCHAR min,sec;
	UINT w,h,m;
	m_Mines.GetTime(min,sec);//lay thoi gian hien tai
	m_Mines.GetSize(w,h,m);//lay thong so game choi hien tai
	//doc diem cao o mode game hien tai
	if(ReadFile(g_szFileName,&hs,m_iGameMode-ID_FILE_NEW_BEGINNER)){
		//MsgBox(hs.time);
		CHAR time[3];		
		UCHAR oldmin,oldsec;     //lay thoi gian cu(tu file)
		strncpy(time,hs.time,2);//lay phan min		
		oldmin= atoi(time);
		//MsgBoxN(oldmin);
		strncpy(time,hs.time+3,2);//lay phan second
		oldsec= atoi(time);
		//MsgBoxN(oldsec); 
		//truong hop custom thi neu so mine cang lon thi uu tien ghi vao danh sach diem cao
		if(m_iGameMode==ID_FILE_NEW_CUSTOM){
			CHAR strmine[4];
			char c;
			UINT oldmines;
			//do hs.mode doi voi truong hop custom co dang ww-hh-mmmm
			//nen tim ra dau gach cuoi de copy mmmm vao strmine
			for(c=strlen(hs.mode)-1; c>=0; c--) 
				if(hs.mode[c]=='-') break;
			strcpy(strmine,hs.mode+c+1);
			oldmines= atol(strmine);
			//truong hop so mine o game moi lon hon thi cho phep cap nhat diem cao
			if(oldmines< m)goto label;
			if(oldmines>m) return;//neu thap hon thi ket thuc
			//con neu bang thi se lam tiep cau lenh duoi de kiem tra thoi gian
		}
		//cac mode game khac se chi kiem tra thoi gian
		//truong hop thoi gian moi tot hon thi hien thi hop thoai cap nhat diem
		if((min<oldmin)||(min==oldmin && sec< oldsec)) goto label;
	}else{//truong hop doc file bi loi
		WriteDefaultFile();//viet file mac dinh
		//khi dialog hien len no can doc thong tin trong file		
label:		
		CDlgBestTime dlgbesttime(this,m_iGameMode,w,h,m,min,sec);
		dlgbesttime.DoModal();
	}
}

BOOL CMinesFrame::OnCommand(WPARAM wParam,LPARAM lParam){
	UINT IDM= LOWORD(wParam);
	if(ID_FILE_RECENT1<=IDM && IDM<= ID_FILE_RECENT1+MAX_RECENTMODE-1){
		CMenu *mnuFile= GetMenu()->GetSubMenu(0);
		UINT w,h,m;
		CString str;
		mnuFile->GetMenuString(IDM,str,MF_BYCOMMAND);
		//string cua menu item co dinh dang nhu sau ww-hh-mmm
		//xem trong OnFileNew phan custom
		w= atol(str.Left(2));
		h= atol(str.Mid(3,2));
		m= atol(str.Right(3));
		NewGame(w,h,m);
		m_iGameMode= ID_FILE_NEW_CUSTOM;//che do choi bay gio la CUSTOM 
		//de khi newgame ...
		Invalidate();//ve lai bang choi
		//dua menu vua click len lam menu tren cung
		//tim ra pos cua string tren menu item vua click trong mang m_strRecentMode
		//MsgBoxN(IDM-ID_FILE_RECENT1);
		//do index cao cao la cang moi tuc la nam tren nen
		//ung voi IDM thi index trong m_strRecentMode la m_cRecentCnt-1-(IDM-ID_FILE_RECENT1)
		Old2NewRecentMenu(m_cRecentCnt-1-(IDM-ID_FILE_RECENT1));
		//chu y ham UpdateRecentMenu su dung mang m_strCecentMode va m_cRecentCnt de viet menu
		//va bo sung menu neu can vi vay ham Old2NewRecentMenu muon thay doi menu thi chi can
		//thay doi mang va chi so do
		UpdateRecentMenu();
	}		
	return CFrameWnd::OnCommand(wParam,lParam);
}
//lay size nho nhat cua cua so
void CMinesFrame::OnGetMinMaxInfo(MINMAXINFO *lpMMI){
	m_pointMaxTrackSize= lpMMI->ptMaxTrackSize;
}
CPoint CMinesFrame::GetMaxTrackSize(){
	return m_pointMaxTrackSize;
}
				