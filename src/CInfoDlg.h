#pragma once



// CInfoDlg 窗体视图
//数据内容窗口

class CInfoDlg : public CFormView
{
	DECLARE_DYNCREATE(CInfoDlg)

protected:
	CInfoDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CInfoDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_INFO };
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
	CListCtrl list_kind;
	CListCtrl list_veg;
	CListCtrl list_plant;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLvnItemchangedKind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedVeg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedShow();
};


