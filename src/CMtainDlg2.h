#pragma once



// CMtainDlg2 窗体视图
//蔬菜基本信息维护窗口

class CMtainDlg2 : public CFormView
{
	DECLARE_DYNCREATE(CMtainDlg2)

protected:
	CMtainDlg2();           // 动态创建所使用的受保护的构造函数
	virtual ~CMtainDlg2();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_MAINTAIN2 };
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
	CString editAddName;
	CString editModName;
	CString editAddNut;
	CComboBox cmbAddKind;
	CComboBox cmbModKind;
	CListCtrl listVeg;
	CComboBox cmbVnum;
	CString editModNut;
	CString editVnum;

public:
	afx_msg void OnBnClickedAddc();
	afx_msg void OnBnClickedAdd();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedMod();
	afx_msg void OnCbnSelchangeVnum();
	afx_msg void OnBnClickedModc();

};


