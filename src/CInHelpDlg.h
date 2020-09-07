#pragma once


// CInHelpDlg 对话框
//数据导入帮助窗口

class CInHelpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInHelpDlg)

public:
	CInHelpDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInHelpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIG_INHELP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
