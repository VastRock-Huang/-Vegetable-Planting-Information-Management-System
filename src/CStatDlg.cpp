// CStatDlg.cpp: 实现文件
//数据统计窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CStatDlg.h"

// CStatDlg

IMPLEMENT_DYNCREATE(CStatDlg, CFormView)

CStatDlg::CStatDlg()
	: CFormView(DIG_STAT)
	, editStatYear1(_T(""))
	, editStatYear2(_T(""))
	, editStatYear(_T(""))
	, staYear(_T(""))
	, staVTitle(_T(""))
{

}

CStatDlg::~CStatDlg()
{
}

void CStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_STAT1, listStat1);
	DDX_Text(pDX, EDIT_STATYEAR1, editStatYear1);
	DDV_MaxChars(pDX, editStatYear1, 4);
	DDX_Text(pDX, EDIT_STATYEAR2, editStatYear2);
	DDV_MaxChars(pDX, editStatYear2, 4);
	DDX_Control(pDX, LIST_STAT2, listStat2);
	DDX_Text(pDX, EDIT_STATYEAR, editStatYear);
	DDV_MaxChars(pDX, editStatYear, 4);
	DDX_Text(pDX, STA_STATYEAR, staYear);
	DDX_Text(pDX, STA_VTITLE, staVTitle);
}

BEGIN_MESSAGE_MAP(CStatDlg, CFormView)
	ON_BN_CLICKED(BNT_STAT2, &CStatDlg::OnBnClickedStat2)
	ON_BN_CLICKED(BNT_EPT2, &CStatDlg::OnBnClickedEpt2)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(BNT_STAT1, &CStatDlg::OnBnClickedStat1)
	ON_BN_CLICKED(BNT_EPT1, &CStatDlg::OnBnClickedEpt1)
	ON_BN_CLICKED(BNT_STATTREND, &CStatDlg::OnBnClickedStattrend)
END_MESSAGE_MAP()


// CStatDlg 诊断

#ifdef _DEBUG
void CStatDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CStatDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStatDlg 消息处理程序


//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口数据
void CStatDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化统计列表
	CString head1[] = { L"分类名称",L"种植面积",L"收获总重量" },
		head2[] = {L"蔬菜名称",L"分类名称",L"种植面积",L"收获总重量" };
	int wid1[] = { 73,73,90 }, wid2[] = { 72,72,72,90 };
	initList(&listStat1, head1, wid1, 3);
	initList(&listStat2, head2, wid2, 4);

	
	extern int kindCnt;
	extern int vegCnt;
	extern wchar_t maxYear[5], minYear[5];
	kCnt = kindCnt;		//初始化 按种类统计信息条数
	kStatPla = new StatData[kCnt];	//初始化内存
	vStatPla = new StatData[vegCnt];	//初始化按种类统计信息
	yearCnt = _wtoi(maxYear) - _wtoi(minYear) + 1;	//初始化年份数为数据中最大年到最小年的年数
	if (yearCnt <= 0)		//若无数据, 年份差不为正, 则返回
		return;
	if(kCnt>0)
		graphFlag = 0;	//初始化绘图标识
	vStYearPla = new StatData[yearCnt];		//初始化按年份统计信息
	OnBnClickedEpt1();	//初始化种类统计信息及其柱状图
	OnBnClickedEpt2();	
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按起止年份/蔬菜统计种植信息
void CStatDlg::OnBnClickedStat2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int f1 = !editStatYear1.IsEmpty();
	int f2 = !editStatYear2.IsEmpty();
	if (f1 == 0 && f2 == 0)		//若输入框为空则返回
	{
		MessageBox(L"请输入起止年份！", L"警告");
		return;
	}
	
	//判断输入年份是否合法
	if (f1 && editStatYear1.GetLength() < 4 || f2 && editStatYear2.GetLength() < 4 
		|| (f1 && f2 && editStatYear1 >editStatYear2))
	{
		MessageBox(L"输入年份有误！", L"警告");
		editStatYear1.Empty();
		editStatYear2.Empty();
		UpdateData(FALSE);
		return;
	}
	
	listStat2.DeleteAllItems();	//清除列表已有数据
	wchar_t year1[5], year2[5];
	
	extern wchar_t maxYear[5], minYear[5];
	if (editStatYear1.IsEmpty())
		wcscpy(year1, minYear);
	else
		wcscpy(year1, editStatYear1);
	if (editStatYear2.IsEmpty())
		wcscpy(year2, maxYear);
	else
		wcscpy(year2, editStatYear2);
		
	vCnt=statPlantOfVegYear(vStatPla, year1, year2);	//按年份统计数据,返回数据条数
	addStatDataToList(&listStat2, vStatPla, vCnt, 1);	//将数据添加到列表
	
	if (vCnt == 0)	//无符合条件的数据
		listStat2.InsertItem(0, L"无信息!!!");
	else	//有数据
	{
		graphFlag = 1;	//设置绘图标识
		if(wcscmp(year1,year2)==0)	//设置柱状图标题
			staVTitle.Format(L"%s年各种蔬菜收获重量柱状统计图", year1);
		else
			staVTitle.Format(L"%s~%s年各种蔬菜收获重量柱状统计图", year1, year2);
		UpdateData(FALSE);
		Invalidate();	//绘制按蔬菜柱状图
	}
		
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空按蔬菜统计信息的文本框, 绘制包含全部信息的蔬菜柱状图
void CStatDlg::OnBnClickedEpt2()
{
	// TODO: 在此添加控件通知处理程序代码
	
	extern wchar_t maxYear[5], minYear[5];
	editStatYear1 = minYear;		//最大年和最小年赋值给文本框
	editStatYear2 = maxYear;
	if (yearCnt>0)		//若年份差>0, 即正常情况下
	{
		UpdateData(FALSE);
		OnBnClickedStat2();
	}
	//else	//若无数据
	//	listStat2.DeleteAllItems();
	editStatYear1.Empty();	//清空输入框
	editStatYear2.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：绘图响应
void CStatDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CFormView::OnPaint()
	
	if (graphFlag == -1)		//若绘图标识为-1, 则返回
		return;

	drawGraph(dc,kStatPla,0,{ 360,290 }, { 900, 420 });	//绘制按种类柱状图
	if(graphFlag==2)	//绘制蔬菜折线图
		drawGraph(dc, vStYearPla, 2, { 420,640 }, { 850,420 });
	else	//绘制蔬菜柱状图
		drawGraph(dc, vStatPla, 1, { 420,640 }, { 850,420 });
}


//////////////////////////////////////////////////////////////////////////
//函数功能：绘制统计图
//入口参数：dc为MFC绘图对象, statPla为绘图数据, graphFlag为绘图标识, org为原点坐标, ax为图形大小
//返回值：无
void  CStatDlg::drawGraph(CPaintDC& dc,StatData* statPla,
	int graphFlag, POINT org,SIZE ax)
{
	int dataCnt;	//数据条数
	if (graphFlag == 0)		//种类柱状图时数据条数为蔬菜种类数
		dataCnt = kCnt;
	else if (graphFlag == 1)	//蔬菜柱状图时为按蔬菜统计信息条数
		dataCnt = vCnt;
	else	//蔬菜条形图时为年份数
		dataCnt = yearCnt;

	LOGFONT log;
	CFont font;
	log.lfHeight = 32;
	log.lfEscapement = 0;
	log.lfItalic = 0;
	log.lfUnderline = 0;
	log.lfStrikeOut = 0;

	//修改字体
	font.CreateFontIndirect(&log);
	dc.SelectObject(&font);     

	//画笔
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen);

	//逻辑坐标
	SetViewportOrgEx(dc, org.x, org.y, NULL);
	dc.SetMapMode(MM_LOMETRIC);		//0.1毫米

	//纵坐标轴
	int axV = ax.cy;
	dc.MoveTo(0, 0);
	dc.LineTo(0, axV);		//纵坐标高度
	dc.LineTo(-10, axV - 20);		//画坐标箭头
	dc.MoveTo(0, axV);
	dc.LineTo(10, axV - 20);

	//纵坐标刻度
	CString str;
	int vDataStep = ((int)(statPla[0].p_wegiht) / 100 + 1) * 10;	//纵坐标数据间隔大小
	int vAxVStep = (axV / 100) * 10;	//纵坐标像素间隔
	int vStepRatio = vAxVStep / vDataStep;		//纵坐标像素间隔与数据间隔之比
	for (int i = 0; i < 11; ++i)	//绘制纵坐标刻度
	{
		str.Format(L"%d", vDataStep * i);
		dc.TextOut(-50, i * vAxVStep+10, str);
	}
	dc.TextOut(-50, axV + 20, L"kg");

	//横坐标轴
	int axH = ax.cx;
	dc.MoveTo(0, 0);
	dc.LineTo(axH, 0);		//横坐标宽度
	if(graphFlag != 2)		//蔬菜柱状图时
	{
		dc.LineTo(axH - 20, 10);
		dc.MoveTo(axH, 0);
		dc.LineTo(axH - 20, -10);
	}
	int hAxHStep = axH / (2 * dataCnt + 1);		//横轴数据间隔

	//画刷颜色设置
	int brushColor[4][3] = { {3, 141, 216},{253, 51, 67},
	{139, 197, 51}, {250, 164, 51} };
	CBrush statBrush[4];
	for (int i = 0; i < 4; ++i)
	{
		statBrush[i].CreateSolidBrush(RGB(brushColor[i][0], brushColor[i][1], brushColor[i][2]));
	}

	if (graphFlag == 2)		//绘制蔬菜折线图
	{
		//绘制种植面积坐标轴
		dc.MoveTo(axH, 0);
		dc.LineTo(axH, axV);		//纵坐标高度
		dc.LineTo(axH-10, axV - 20);	//画坐标箭头
		dc.MoveTo(axH, axV);
		dc.LineTo(axH+10, axV - 20);

		for (int i = 0; i < 11; ++i)	//绘制刻度
		{
			str.Format(L"%d", i);
			dc.TextOut(axH + 20, i * vAxVStep+10, str);
		}
		dc.TextOut(axH+10, axV + 20, L"分");

		//绘制收获重量
		CPen pen1(PS_SOLID, 2, RGB(253, 51, 67));	//设置画笔
		dc.SelectObject(&pen1);
		for (int i = 0; i < dataCnt; ++i)	
		{
			int tmpH =(int) statPla[i].p_wegiht * vStepRatio;
			dc.SelectObject(statBrush[1]);	//选择画刷
			dc.Ellipse(hAxHStep * (2 * i + 1) - 5, tmpH - 5, hAxHStep * (2 * i + 1) + 5, tmpH + 5);	//画点
			str.Format(L"%.2f", statPla[i].p_wegiht);	//标注数字
			dc.TextOutW(hAxHStep * (2 * i + 1), tmpH + 60, str);
			dc.TextOut(hAxHStep * (2 * i + 1), -10, statPla[i].p_year);	//横坐标标题
		}
		if (dataCnt > 1)	//数据大于1时绘制折线
		{
			dc.MoveTo(hAxHStep, (int)statPla[0].p_wegiht * vStepRatio);
			for (int i = 1; i < dataCnt; ++i)
			{
				int tmpH =(int) statPla[i].p_wegiht * vStepRatio;
				dc.LineTo(hAxHStep * (2 * i + 1), tmpH);
			}
		}

		//绘制种植面积
		CPen pen2(PS_SOLID, 2, RGB(3, 141, 216));
		dc.SelectObject(&pen2);
		for (int i = 0; i < dataCnt; ++i)
		{
			int tmpH = statPla[i].p_area * vAxVStep;
			dc.SelectObject(statBrush[0]);
			dc.Ellipse(hAxHStep * (2 * i + 1) - 5, tmpH - 5, hAxHStep * (2 * i + 1) + 5, tmpH + 5);
			str.Format(L"%d", statPla[i].p_area);
			dc.TextOutW(hAxHStep * (2 * i + 1), tmpH + 25, str);
		}
		if (dataCnt > 1)
		{
			dc.MoveTo(hAxHStep, statPla[0].p_area * vAxVStep);
			for (int i = 1; i < dataCnt; ++i)
			{
				int tmpH = statPla[i].p_area * vAxVStep;
				dc.LineTo(hAxHStep * (2 * i + 1), tmpH);
			}
		}
		return;
	}

	//绘制柱状图
	CRect* statRect = new CRect[dataCnt];
	for (int i = 0; i < dataCnt; ++i)	//绘制矩形
	{
		statRect[i].right = hAxHStep * 2 * (i + 1);
		statRect[i].left = statRect[i].right - hAxHStep;
		statRect[i].bottom = 0;
		statRect[i].top = (LONG)statPla[i].p_wegiht * vStepRatio;

		dc.FillRect(statRect[i], &statBrush[i % 4]);	//填充颜色
		if (graphFlag == 0)		//设置标注
			str = statPla[i].pKind->k_name;
		else
			str = statPla[i].pVeg->v_name;
		dc.TextOut(hAxHStep * (2 * i + 1), -10, str);
		str.Format(L"%.2f", statPla[i].p_wegiht);
		dc.TextOutW(hAxHStep * (2 * i + 1), statRect[i].top + 40, str);
	}
	delete[] statRect;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：注销窗口前处理数据
void CStatDlg::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	delete[] vStatPla;		//释放统计信息
	delete[] kStatPla;
	delete[] vStYearPla;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按年份/种类统计种植信息
void CStatDlg::OnBnClickedStat1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (editStatYear.IsEmpty())
	{
		MessageBox(L"请输入种植年份！", L"警告");
		return;
	}
	if (editStatYear.GetLength() < 4)
	{
		MessageBox(L"输入年份有误！", L"警告");
		editStatYear.Empty();
		UpdateData(FALSE);
		return;
	}
	wchar_t year[5];
	wcscpy(year, editStatYear);
	;
	extern int kindCnt;
	statPlantOfKind(kStatPla, year);		//按年份统计种植信息
	if(kindCnt>1)	//数据条数大于1按收获重量排序
		qucikSortByWeight(kStatPla, 0, kindCnt - 1);
	addStatDataToList(&listStat1, kStatPla, kindCnt, 0);	//将统计数据添加到列表
	staYear = editStatYear + L"年";	//设置种类统计图年份标题
	UpdateData(FALSE);
	Invalidate();		//绘制
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空按种类统计信息的文本框, 绘制包含全部信息的种类柱状图
void CStatDlg::OnBnClickedEpt1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	extern int kindCnt;
	statPlantOfKind(kStatPla, L"");		//按种类统计种植信息并存到kStatPla中
	if (kindCnt > 1)		//数据条数大于1, 则对数据按收获重量排序
		qucikSortByWeight(kStatPla, 0, kindCnt - 1);
	addStatDataToList(&listStat1, kStatPla, kindCnt, 0);		//将统计数据添加到列表
	staYear.Empty();	//清空
	editStatYear.Empty();
	UpdateData(FALSE);
	Invalidate();	//绘制柱状图
}


//////////////////////////////////////////////////////////////////////////
//函数功能：趋势按钮响应 绘制折线图
void CStatDlg::OnBnClickedStattrend()
{
	// TODO: 在此添加控件通知处理程序代码
	//此处在已有统计数据情况下绘制, 千万不能更新数据
	int i, len = listStat2.GetItemCount();		//i记录删除种类的索引i>=0
	for (i = 0; i < len; i++)		//获取列表当前选中项
	{
		if (listStat2.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			break;
	}
	if (i == len)		//若未选中
	{
		MessageBox(L"请选中一条蔬菜统计信息！", L"警告");
		return;
	}
	if (i == 0 && listStat2.GetItemText(i, 0) == L"无信息!!!")	//统计结果无数据则返回
	{
		MessageBox(L"无蔬菜统计信息！", L"警告");
		return;
	}
	
	extern wchar_t maxYear[5], minYear[5];
	wchar_t year1[5], year2[5];
	if (editStatYear1.IsEmpty())
		wcscpy(year1, minYear);
	else
		wcscpy(year1, editStatYear1);
	if (editStatYear2.IsEmpty())
		wcscpy(year2, maxYear);
	else
		wcscpy(year2, editStatYear2);

	yearCnt = _wtoi(year2) - _wtoi(year1) + 1;	//设置年份数
	
	statVegByYear(vStYearPla, vStatPla[i].pVeg->v_name, year1, year2);	//按起止年份统计每年的种植信息

	graphFlag = 2;	//设置绘图标识为折线图
	if(wcscmp(year1,year2)==0)	//设置统计图标题
		staVTitle.Format(L"%s年%s收获重量及种植面积折线图", year1, vStatPla[i].pVeg->v_name);
	else
		staVTitle.Format(L"%s~%s年%s收获重量及种植面积折线图", year1, year2, vStatPla[i].pVeg->v_name);
	UpdateData(FALSE);
	Invalidate();	//绘制折线图
}
