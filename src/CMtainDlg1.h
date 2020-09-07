#pragma once



// CMtainDlg1 窗体视图
//蔬菜种类信息维护窗口

class CMtainDlg1 : public CFormView
{
	DECLARE_DYNCREATE(CMtainDlg1)

protected:
	CMtainDlg1();           // 动态创建所使用的受保护的构造函数
	virtual ~CMtainDlg1();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_MAINTIAN1 };
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
	CListCtrl listKind;
	CString editAddNum;
	CString editModNum;
	CComboBox cmbNum;
	CString editAddName;
	CString editModName;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedAddc();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedMod();
	afx_msg void OnBnClickedModc();
	afx_msg void OnCbnSelchangeNum();

};


