#pragma once
#include "VegData.h"


// CStatDlg 窗体视图
//数据统计窗口

class CStatDlg : public CFormView
{
	DECLARE_DYNCREATE(CStatDlg)

protected:
	CStatDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CStatDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_STAT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl listStat1;
	CString editStatYear1;
	CString editStatYear2;
	CListCtrl listStat2;
	CString editStatYear;
	CString staYear;	//种类柱状图的年份标题
	CString staVTitle;		//种类柱状图标题
	StatData* kStatPla = NULL;		//按种类统计数据
	StatData* vStatPla = NULL;		//按蔬菜统计数据
	StatData* vStYearPla = NULL;	//按年份统计数据
	int kCnt = 0;	//按种类统计数据条数
	int vCnt = 0;	//按蔬菜统计数据条数
	int yearCnt = 0;	//年份数
	int graphFlag = -1;		//绘图标识: -1为无数据时不绘图, 1为柱状图, 2为折线图
	void drawGraph(CPaintDC& dc, StatData* statPla, int dataCnt, POINT org, SIZE ax);	//绘制统计图

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedStat2();
	afx_msg void OnBnClickedEpt2();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStat1();
	afx_msg void OnBnClickedEpt1();
	afx_msg void OnBnClickedStattrend();
	
};


