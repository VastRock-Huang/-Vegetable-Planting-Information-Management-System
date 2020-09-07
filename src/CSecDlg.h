#pragma once



// CSecDlg 窗体视图
//数据查询窗口

class CSecDlg : public CFormView
{
	DECLARE_DYNCREATE(CSecDlg)

protected:
	CSecDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CSecDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_SEARCH };
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
	CListCtrl listSecVeg;
	CListCtrl listSecPlant;
	CString editSecKnum;
	CString editSecKname;
	CString editSecVname;
	CString editSecVknum;
	CString editSecVnut;
	CString editSecPVname;
	CString editSecPyear;
	CString editSecPVname2;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedSeck();
	afx_msg void OnBnClickedEptk();
	afx_msg void OnBnClickedSecv1();
	afx_msg void OnBnClickedEptv1();
	afx_msg void OnBnClickedSecv2();
	afx_msg void OnBnClickedEptv2();
	afx_msg void OnBnClickedSecp1();
	afx_msg void OnBnClickedEptp1();
	afx_msg void OnBnClickedSecp2();
	afx_msg void OnBnClickedEptp2();

};


