#pragma once
#include "VegData.h"


// CIoDlg 窗体视图
//数据导入导出窗口

class CIoDlg1 : public CFormView
{
	DECLARE_DYNCREATE(CIoDlg1)

protected:
	CIoDlg1();           // 动态创建所使用的受保护的构造函数
	virtual ~CIoDlg1();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_IO1 };
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

public:
	afx_msg void OnBnClickedPath();
	afx_msg void OnBnClickedIn();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedInc();
	afx_msg void OnBnClickedInhelp();
	afx_msg void OnBnClickedOutpath1();
	afx_msg void OnBnClickedOut1();
	afx_msg void OnBnClickedGet1();
	afx_msg void OnBnClickedOept1();
	afx_msg void OnBnClickedGet2();
	afx_msg void OnBnClickedOutpath2();
	afx_msg void OnBnClickedOut2();
	afx_msg void OnBnClickedOept2();
	afx_msg void OnDestroy();

private:
	CString editInPath;		//导入文件路径
	CComboBox cmbInMode;	//导入模式下拉框
	CString editOutPath1;	//导出文件路径1
	CString editOutYear;	//导出年份
	CListCtrl listOut1;		//导出数据列表1
	CListCtrl listOut2;		//导出数据列表2
	CString editOutVname;	//导出蔬菜名称
	CString editOutPath2;	//导出文件路径2
	StatData* statPla = NULL;	//蔬菜数据指针
};


