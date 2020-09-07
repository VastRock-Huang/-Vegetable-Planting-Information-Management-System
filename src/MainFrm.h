
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

//////////////////////////////////////////////////////////////////////////
//自定义消息
//WM_USER 是用户自定义消息的一个起始值
//WM_USER+100是为了区分系统消息和用户消息，避免冲突
#define NM_INFO (WM_USER+100)		//数据内容消息
#define NM_MAINTAIN1 (WM_USER+101)		//蔬菜种类信息维护消息
#define NM_MAINTAIN2 (WM_USER+102)		//蔬菜基本信息维护消息
#define NM_MAINTAIN3 (WM_USER+103)		//蔬菜种植信息维护消息
#define NM_SEARCH (WM_USER+104)		//数据查询消息
#define NM_STAT (WM_USER+105)		//数据统计消息
#define NM_IO1 (WM_USER+106)		//基本数据导入导出消息
#define NM_IO2 (WM_USER+107)		//自定义导出消息

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

private:
	CSplitterWnd m_spliter;		//切分窗口类对象
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);		//根据左侧选择的标题进行窗口切换
	afx_msg LRESULT OnCustomChange(WPARAM wParam, LPARAM lParam);		//添加自定义消息处理函数

public:
	afx_msg void OnClose();
	afx_msg void OnInfo();
	afx_msg void OnMaintain1();
	afx_msg void OnMaintain2();
	afx_msg void OnMaintain3();
	afx_msg void OnSec();
	afx_msg void OnStat();
	afx_msg void OnIo1();
	afx_msg void OnIo2();
};


