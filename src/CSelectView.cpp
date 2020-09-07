// CSelectView.cpp: 实现文件
//左侧树状选择栏

#include "pch.h"
#include "VegInfoSystem.h"
#include "CSelectView.h"
#include "MainFrm.h"

// CSelectView

IMPLEMENT_DYNCREATE(CSelectView, CTreeView)

CSelectView::CSelectView()
{

}

CSelectView::~CSelectView()
{
}

BEGIN_MESSAGE_MAP(CSelectView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSelectView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CSelectView 诊断

#ifdef _DEBUG
void CSelectView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CSelectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSelectView 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：初始化树状选择栏
void CSelectView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	//初始化树视图
	//获取树控件
	m_treeCtrl = &GetTreeCtrl();
	//初始化图片集合
	HICON icons[10];
	int cnt=0;
	icons[0] = AfxGetApp()->LoadIconW(IDI_LIST);
	icons[1] = AfxGetApp()->LoadIconW(IDI_LISTon);
	icons[2] = AfxGetApp()->LoadIconW(IDI_MAINTAIN);
	icons[3] = AfxGetApp()->LoadIconW(IDI_SEARCH);
	icons[4] = AfxGetApp()->LoadIconW(IDI_SEARCHon);
	icons[5] = AfxGetApp()->LoadIconW(IDI_STAT);
	icons[6] = AfxGetApp()->LoadIconW(IDI_STATon);
	icons[7] = AfxGetApp()->LoadIconW(IDI_IO);
	icons[8] = AfxGetApp()->LoadIconW(IDI_CHECK);
	icons[9] = AfxGetApp()->LoadIconW(IDI_CHECKon);

	m_imgList.Create(30, 30, ILC_COLOR32, 12, 12);
	for (int i = 0; i < 10; i++)
	{
		m_imgList.Add(icons[i]);		//添加具体照片
	}
	m_treeCtrl->SetImageList(&m_imgList, TVSIL_NORMAL);

	//添加结点
	int i = 0;
	m_treeCtrl->InsertItem(m_txtList[i++], 0, 1, NULL);

	treeMaintain = m_treeCtrl->InsertItem(m_txtList[i++], 2, 2, NULL);
	m_treeCtrl->InsertItem(m_txtList[i++], 8, 9, treeMaintain);
	m_treeCtrl->InsertItem(m_txtList[i++], 8, 9, treeMaintain);
	m_treeCtrl->InsertItem(m_txtList[i++], 8, 9, treeMaintain);
	
	treeSearch = m_treeCtrl->InsertItem(m_txtList[i++], 3, 4, NULL);
	
	treeStat = m_treeCtrl->InsertItem(m_txtList[i++], 5, 6, NULL);
	
	treeIO = m_treeCtrl->InsertItem(m_txtList[i++], 7, 7, NULL);
	m_treeCtrl->InsertItem(m_txtList[i++], 8, 9, treeIO);
	m_treeCtrl->InsertItem(m_txtList[i++], 8, 9, treeIO);

	//展开结点
	m_treeCtrl->Expand(treeMaintain, TVE_EXPAND);
	m_treeCtrl->Expand(treeIO, TVE_EXPAND);
}

//////////////////////////////////////////////////////////////////////////
//函数功能：对左侧选择栏进行选择
void CSelectView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//获取当前选中项
	HTREEITEM item = m_treeCtrl->GetSelectedItem();
	//通过选中的项 获取这个项中的内容
	CString str = m_treeCtrl->GetItemText(item);

	//判断具体选择的项
	if (str == m_txtList[0])
	{
		//CWnd::PostMessage 将有个信息放入窗口的消息队列
		//AdxGetMainWnd()：框架窗口对象的指针
		//AfxGetMainWnd()->GetSafeHwnd()：获取返回窗口的句柄，CWnd::GetSafeHwnd
		//NM_A：发送自定义消息
		//(WPARAM)NM_A：指定了附加的消息信息
		//(LPARAM)0：指定了附加的消息信息，此参数这里未定义
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_INFO, (WPARAM)NM_INFO, (LPARAM)0);
	}
	else if (str==m_txtList[1])
	{
		m_treeCtrl->SelectItem(m_treeCtrl->GetChildItem(treeMaintain));
	}
	else if(str==m_txtList[2])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_MAINTAIN1, (WPARAM)NM_MAINTAIN1, (LPARAM)0);
	}
	else if (str == m_txtList[3])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_MAINTAIN2, (WPARAM)NM_MAINTAIN2, (LPARAM)0);
	}
	else if (str == m_txtList[4])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_MAINTAIN3, (WPARAM)NM_MAINTAIN3, (LPARAM)0);
	}
	else if (str == m_txtList[5])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_SEARCH, (WPARAM)NM_SEARCH, (LPARAM)0);
	}
	else if (str==m_txtList[6])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_STAT, (WPARAM)NM_STAT, (LPARAM)0);
	}
	else if (str == m_txtList[7])
	{
		m_treeCtrl->SelectItem(m_treeCtrl->GetChildItem(treeIO));
	}
	else if (str == m_txtList[8])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_IO1, (WPARAM)NM_IO1, (LPARAM)0);
	}
	else if (str == m_txtList[9])
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_IO2, (WPARAM)NM_IO2, (LPARAM)0);
	}

}


