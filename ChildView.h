#ifndef CHILDVIEW_H
#define CHILDVIEW_H
#include "AfxStd.h"
#include "MinesView.h"
class CChildView: public CWnd{
	CMines m_Mines;
public:
	CChildView();
};
#endif