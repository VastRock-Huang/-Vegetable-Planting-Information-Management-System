
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "VegInfoSystem.h"
#include "MainFrm.h"

#include "VegData.h"
#include "CDisplayView.h"		//右侧展示窗口头文件
#include "CSelectView.h"		//左侧选择窗口头文件
#include "CInfoDlg.h"			//数据内容窗口
#include "CMtainDlg1.h"			//蔬菜种类信息维护窗口
#include "CMtainDlg2.h"			//蔬菜基本信息维护窗口
#include "CMtainDlg3.h"			//蔬菜种植信息维护窗口
#include "CSecDlg.h"
#include "CStatDlg.h"
#include "CIoDlg1.h"
#include "CIoDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

	//////////////////////////////////////////////////////////////////////////
	//将自定义消息与函数关联
	//ON_MESSAGE相应自定义消息
	//产生NM_XXX消息后自动调用OnCustomChange函数
	ON_MESSAGE(NM_INFO, OnCustomChange)
	ON_MESSAGE(NM_MAINTAIN1, OnCustomChange)
	ON_MESSAGE(NM_MAINTAIN2, OnCustomChange)
	ON_MESSAGE(NM_MAINTAIN3, OnCustomChange)
	ON_MESSAGE(NM_SEARCH, OnCustomChange)
	ON_MESSAGE(NM_STAT, OnCustomChange)
	ON_MESSAGE(NM_IO1, OnCustomChange)
	ON_MESSAGE(NM_IO2,OnCustomChange)
	ON_WM_CLOSE()

	ON_COMMAND(MENU_INFO, &CMainFrame::OnInfo)
	ON_COMMAND(MENU_MAINTAIN1, &CMainFrame::OnMaintain1)
	ON_COMMAND(MENU_MAINTAIN2, &CMainFrame::OnMaintain2)
	ON_COMMAND(MENU_MAINTAIN3, &CMainFrame::OnMaintain3)
	ON_COMMAND(MENU_SEC, &CMainFrame::OnSec)
	ON_COMMAND(MENU_STAT, &CMainFrame::OnStat)
	ON_COMMAND(MENU_IO1, &CMainFrame::OnIo1)
	ON_COMMAND(MENU_IO2, &CMainFrame::OnIo2)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//////////////////////////////////////////////////////////////////////////
	//设置应用图标
	SetClassLongPtr(m_hWnd, GCLP_HICON, (LONG)AfxGetApp()->LoadIconW(IDR_MAINFRAME));
	//设置标题-右侧
	SetTitle(L"Version 3.0");
	//设置窗口大小
	MoveWindow(0, 0, 1200, 800);
	//设置窗口居中显示窗口
	CenterWindow();
	initData();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style = cs.style & (~WS_THICKFRAME);		//固定窗口大小
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


//////////////////////////////////////////////////////////////////////////
//函数功能：程序主窗体划分
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	//将窗口拆成一行两列
	m_spliter.CreateStatic(this, 1, 2);
	//设置左侧和右侧具体显示内容
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(200, 800), pContext);
	m_spliter.CreateView(0, 1, RUNTIME_CLASS(CDisplayView), CSize(1000, 800), pContext);

	return TRUE;	//自行拆分
}


//////////////////////////////////////////////////////////////////////////
//函数功能：根据左侧选择的标题进行窗口切换
LRESULT CMainFrame::OnCustomChange(WPARAM wParam, LPARAM lParam)
{
	CCreateContext Context;
	switch (wParam)
	{
	case NM_INFO:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CInfoDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CInfoDlg), CSize(1000, 800), &Context);
		CInfoDlg* pNewView = (CInfoDlg*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_MAINTAIN1:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CMtainDlg1);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CMtainDlg1), CSize(1000, 800), &Context);
		CMtainDlg1* pNewView = (CMtainDlg1*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_MAINTAIN2:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CMtainDlg2);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CMtainDlg2), CSize(1000, 800), &Context);
		CMtainDlg2* pNewView = (CMtainDlg2*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_MAINTAIN3:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CMtainDlg3);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CMtainDlg3), CSize(1000, 800), &Context);
		CMtainDlg3* pNewView = (CMtainDlg3*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_SEARCH:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CSecDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CSecDlg), CSize(1000, 800), &Context);
		CSecDlg* pNewView = (CSecDlg*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_STAT:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CStatDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CStatDlg), CSize(1000, 800), &Context);
		CStatDlg* pNewView = (CStatDlg*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_IO1:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CIoDlg1);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CIoDlg1), CSize(1000, 800), &Context);
		CIoDlg1* pNewView = (CIoDlg1*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	case NM_IO2:
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CIoDlg2);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CIoDlg2), CSize(1000, 800), &Context);
		CIoDlg2* pNewView = (CIoDlg2*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
	break;
	}

	return LRESULT();
}


//关闭窗口
void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	

	if (!saveData())		//保存数据
		return;
	freeData();

	CFrameWnd::OnClose();
}

//////////////////////////////////////////////////////////////////////////
//标题栏选项选择函数

void CMainFrame::OnInfo()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_INFO, (WPARAM)NM_INFO, (LPARAM)0);
}


void CMainFrame::OnMaintain1()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_MAINTAIN1, (WPARAM)NM_MAINTAIN1, (LPARAM)0);
}



void CMainFrame::OnMaintain2()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_MAINTAIN2, (WPARAM)NM_MAINTAIN2, (LPARAM)0);	
}


void CMainFrame::OnMaintain3()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_MAINTAIN3, (WPARAM)NM_MAINTAIN3, (LPARAM)0);
}


void CMainFrame::OnSec()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_SEARCH, (WPARAM)NM_SEARCH, (LPARAM)0);
}


void CMainFrame::OnStat()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_STAT, (WPARAM)NM_STAT, (LPARAM)0);
}


void CMainFrame::OnIo1()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_IO1, (WPARAM)NM_IO1, (LPARAM)0);
}


void CMainFrame::OnIo2()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_IO2, (WPARAM)NM_IO2, (LPARAM)0);
}
