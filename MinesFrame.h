/*	Game Mines 
	Write by Trac Quang Hoa  DKTD1,DHBK Ha Noi
	Date 21-4-2008
*/
#ifndef MINESFRAME_H
#define MINESFRAME_H
#include "AfxStd.h"
#include "MinesView.h"
#include "Number.h"
#include "MinesApp.h"//dinh nghia cac dialog trong nay
const CHAR MAX_RECENTMODE = 4;
class CMinesFrame: public CFrameWnd{
	CMines m_Mines;
	//co tri so bang voi ID cua menu tuong ung
	int	   m_iGameMode;
	BOOL   m_bMarks;//cho gia tri true neu co cho phep danh dau ?
	//ham viet file highscore mac dinh
	void WriteDefaultFile();
	 
	//4 recent mode game mo gan day nhat o dang custom
	//duoc su dung trong SaveParameter va LoadParameter
	//va sua doi trong khi nguoi choi tao van choi moi dang custom	
	CString m_strRecentMode[MAX_RECENTMODE];//index cang cao la cang moi
	char    m_cRecentCnt;//dung dem so recent dang co hien tai(vi` co the la 0,1,2 hoac 3)
	UINT	m_nMenuFileItemCnt;//so item cua menu File ban dau truoc khi them cac item moi vao,duoc khoi dong trong OnCreate
	//cap nhat lai menu recent,duoc goi khi co custom moi
	void UpdateRecentMenu();
	//
	void Old2NewRecentMenu(char pos);
	//co chien thang can cap nhat xem diem co lon hon diem hien tai khong
	void Win();	
	
	//luu gia tri nho lon cua cua so,dung de tinh toan ba?ng choi lon nhat
	//su dung man hinh khong chinh xax
	//do khong biet cach nao lay truc tiep nen phai dung bien luu lai vi chi co the lay trong OnGetMinMaxInfo
	CPoint m_pointMaxTrackSize;
public:
	CPoint GetMaxTrackSize();//ham nay su duoc dialog custom su dung
	CMinesFrame();
	void   NewGame(UINT w,UINT h,UINT m);
	void SaveParameter();
	void LoadParameter();
	virtual BOOL PreCreateWindow(CREATESTRUCT & );
	afx_msg int OnCreate(LPCREATESTRUCT );
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags,CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	afx_msg void OnPaint();	
	afx_msg void OnFileNewGame();
	afx_msg void OnFileNew(UINT nID);
	afx_msg void OnUpdateFileNew(CCmdUI *pUI);
	afx_msg void OnFileOption();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnHelpAbout();
	afx_msg void OnHelpTopic();
	afx_msg void OnFileMarks();
	afx_msg void OnUpdateFileMarks(CCmdUI *pUI);
	afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
	afx_msg void OnFileExit();
	afx_msg void OnFileBestTimes();
	BOOL OnCommand(WPARAM wParam,LPARAM lParam);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	DECLARE_MESSAGE_MAP()
	friend CDlgCustom;
};
#endif