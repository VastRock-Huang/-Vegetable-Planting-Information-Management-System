
// VegInfoSystemView.cpp: CVegInfoSystemView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "VegInfoSystem.h"
#endif

#include "VegInfoSystemDoc.h"
#include "VegInfoSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVegInfoSystemView

IMPLEMENT_DYNCREATE(CVegInfoSystemView, CView)

BEGIN_MESSAGE_MAP(CVegInfoSystemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CVegInfoSystemView 构造/析构

CVegInfoSystemView::CVegInfoSystemView() noexcept
{
	// TODO: 在此处添加构造代码

}

CVegInfoSystemView::~CVegInfoSystemView()
{
}

BOOL CVegInfoSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CVegInfoSystemView 绘图

void CVegInfoSystemView::OnDraw(CDC* /*pDC*/)
{
	CVegInfoSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CVegInfoSystemView 打印

BOOL CVegInfoSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CVegInfoSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CVegInfoSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CVegInfoSystemView 诊断

#ifdef _DEBUG
void CVegInfoSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CVegInfoSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVegInfoSystemDoc* CVegInfoSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVegInfoSystemDoc)));
	return (CVegInfoSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CVegInfoSystemView 消息处理程序
