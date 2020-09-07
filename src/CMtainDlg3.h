#pragma once



// CMtainDlg3 窗体视图
//蔬菜种植信息窗口

class CMtainDlg3 : public CFormView
{
	DECLARE_DYNCREATE(CMtainDlg3)

protected:
	CMtainDlg3();           // 动态创建所使用的受保护的构造函数
	virtual ~CMtainDlg3();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_MAINTAIN3 };
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
	CListCtrl listPlant;
	CString editPnum;
	CComboBox cmbAddVnum;
	CComboBox cmbModVnum;
	CString editAddVname;
	CString editModVname;
	CComboBox cmbPnum;
	CString editAddArea;
	CString editAddWig;
	CString editAddYear;
	CString editModArea;
	CString editModYear;
	CString editModWig;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnCbnSelchangeVnum();
	afx_msg void OnCbnSelchangeVnum2();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedAddc();
	afx_msg void OnBnClickedDel();
	afx_msg void OnCbnSelchangePnum();
	afx_msg void OnBnClickedMod();
	afx_msg void OnBnClickedModc();

};


