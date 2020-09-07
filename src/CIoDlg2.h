#pragma once
#include "VegData.h"


// CIoDlg2 窗体视图
//自定义数据导出窗口

class CIoDlg2 : public CFormView
{
	DECLARE_DYNCREATE(CIoDlg2)

protected:
	CIoDlg2();           // 动态创建所使用的受保护的构造函数
	virtual ~CIoDlg2();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_IO2 };
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
	StatData* staPla=NULL;
	int col = 1;		//列表列数
	int dataCnt = 0;	//数据条数
	int outFlag = 0,	//导出标志: 0为导出蔬菜种类信息, 1为导出蔬菜基本信息
		yearFlag = 0;	//年份标识: 0为具体年份, 1为年份区间
	CString checkTxt[CHKCNT] = { L"分类名称",L"蔬菜名称",L"分类码",L"营养成分",
		L"种植面积",L"收获重量",L"种植年份" };		//复选框选项标题
	int checkWid[CHKCNT] = { 73,73,60,100,73,73,73 };		//标题宽度
	int checkFlag[CHKCNT] = { 0,0,0,0,0,0,0 };		//选项标记, 数字表示其在列表中的次序
	CComboBox cmbKind;
	CListCtrl listOut;
	CString editVname;
	CString editYear1;
	CString editYear2;
	CString editArea1;
	CString editArea2;
	CString editWig1;
	CString editWig2;
	void OnCheckBox(int id);
	void CIoDlg2::insertToListOut();
	void CIoDlg2::checkCheckBox(int idx, int state);

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedKind();
	afx_msg void OnBnClickedVeg();
	afx_msg void OnBnClickedYear1();
	afx_msg void OnBnClickedYear2();
	afx_msg void OnBnClickedKname();
	afx_msg void OnBnClickedVname();
	afx_msg void OnBnClickedKnum();
	afx_msg void OnBnClickedNut();
	afx_msg void OnBnClickedArea();
	afx_msg void OnBnClickedWig();
	afx_msg void OnBnClickedYear();
	afx_msg void OnBnClickedOut();
	afx_msg void OnBnClickedGet();
	afx_msg void OnBnClickedAll();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOpt();
	afx_msg void OnEnChangeYear1();
	afx_msg void OnEnChangeYear2();
	afx_msg void OnEnChangeArea1();
	afx_msg void OnEnChangeArea2();
	afx_msg void OnEnChangeWig1();
	afx_msg void OnEnChangeWig2();
};


