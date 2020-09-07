// CSecDlg.cpp: 实现文件
//数据查询窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CSecDlg.h"
#include "VegData.h"


// CSecDlg

IMPLEMENT_DYNCREATE(CSecDlg, CFormView)

CSecDlg::CSecDlg()
	: CFormView(DIG_SEARCH)
	, editSecKnum(_T(""))
	, editSecKname(_T(""))
	, editSecVname(_T(""))
	, editSecVknum(_T(""))
	, editSecVnut(_T(""))
	, editSecPVname(_T(""))
	, editSecPyear(_T(""))
	, editSecPVname2(_T(""))
{

}

CSecDlg::~CSecDlg()
{
}

void CSecDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_SECVEG, listSecVeg);
	DDX_Control(pDX, LIST_SECPLANT, listSecPlant);
	DDX_Text(pDX, EDIT_SECKNUM, editSecKnum);
	DDV_MaxChars(pDX, editSecKnum, 1);
	DDX_Text(pDX, EDIT_KNAME, editSecKname);
	DDX_Text(pDX, EDIT_SECVNAME, editSecVname);
	DDV_MaxChars(pDX, editSecVname, 9);
	DDX_Text(pDX, EDIT_SECVKIND, editSecVknum);
	DDV_MaxChars(pDX, editSecVknum, 1);
	DDX_Text(pDX, EDIT_SECVNUT, editSecVnut);
	DDV_MaxChars(pDX, editSecVnut, 8);
	DDX_Text(pDX, EDIT_SECPVNUM, editSecPVname);
	DDV_MaxChars(pDX, editSecPVname, 9);
	DDX_Text(pDX, EDIT_SECPYEAR, editSecPyear);
	DDV_MaxChars(pDX, editSecPyear, 4);
	DDX_Text(pDX, EDIT_PVNAME, editSecPVname2);
	DDV_MaxChars(pDX, editSecPVname2, 9);
}

BEGIN_MESSAGE_MAP(CSecDlg, CFormView)
	ON_BN_CLICKED(BNT_SECK, &CSecDlg::OnBnClickedSeck)
	ON_BN_CLICKED(BUT_EPTK, &CSecDlg::OnBnClickedEptk)
	ON_BN_CLICKED(BNT_SECV1, &CSecDlg::OnBnClickedSecv1)
	ON_BN_CLICKED(BNT_EPTV1, &CSecDlg::OnBnClickedEptv1)
	ON_BN_CLICKED(BNT_SECV2, &CSecDlg::OnBnClickedSecv2)
	ON_BN_CLICKED(BNT_EPTV2, &CSecDlg::OnBnClickedEptv2)
	ON_BN_CLICKED(BNT_SECP1, &CSecDlg::OnBnClickedSecp1)
	ON_BN_CLICKED(BNT_EPTP1, &CSecDlg::OnBnClickedEptp1)
	ON_BN_CLICKED(BNT_SECP2, &CSecDlg::OnBnClickedSecp2)
	ON_BN_CLICKED(BNT_EPTP2, &CSecDlg::OnBnClickedEptp2)
END_MESSAGE_MAP()


// CSecDlg 诊断

#ifdef _DEBUG
void CSecDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSecDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSecDlg 消息处理程序


//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口数据
void CSecDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	
	//初始化查询结果列表
	CString vegHead[] = { L"蔬菜编号",L"蔬菜名称",L"分类码",L"营养成分" },
		plantHead[] = { L"编号",L"蔬菜编号",L"种植面积",L"收获重量",L"种植年份" };
	int vegWid[] = { 75,75,60,400 }, plantWid[] = { 73,73,73,75,75 };
	initList(&listSecVeg, vegHead, vegWid, 4);	//初始化表头
	initList(&listSecPlant, plantHead, plantWid, 5);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：蔬菜种类信息查询
void CSecDlg::OnBnClickedSeck()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//查询信息为空则返回
	if (editSecKnum.IsEmpty())
	{
		MessageBox(L"请输入查询信息！", L"警告");
		return;
	}

	Kind ipt, * opt;
	
	ipt.k_num = editSecKnum[0];		//将待查信息封装到结构体中
	if (opt = checkInKind(ipt.k_num,ipt.k_name, 0))	//若该种类查到则输出到文本框
	{
		editSecKname.Format(L"%s",opt->k_name);
	}
	else
	{
		editSecKname = L"未找到!!!";
	}
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空待查询的蔬菜种类信息
void CSecDlg::OnBnClickedEptk()
{
	// TODO: 在此添加控件通知处理程序代码
	editSecKnum.Empty();
	editSecKname.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜部分名称蔬菜基本信息查询
void CSecDlg::OnBnClickedSecv1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (editSecVname.IsEmpty())
	{
		MessageBox(L"请输入查询信息！", L"警告");
		return;
	}
	
	wchar_t subname[10];
	wcscpy(subname, editSecVname);
	listSecVeg.DeleteAllItems();
	secVegByName(subname, &listSecVeg);	//按蔬菜部分名称查询并添加到列表中
	listSecVeg.SortItems(CListCtrlCmp, (DWORD_PTR)&listSecVeg);		//列表排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空待查询的蔬菜基本信息
void CSecDlg::OnBnClickedEptv1()
{
	// TODO: 在此添加控件通知处理程序代码
	listSecVeg.DeleteAllItems();
	editSecVname.Empty();
	editSecVknum.Empty();
	editSecVnut.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按分类码/营养成分查询蔬菜基本信息
void CSecDlg::OnBnClickedSecv2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//文本框不能均为空
	if (editSecVknum.IsEmpty() && editSecVnut.IsEmpty())
	{
		MessageBox(L"请输入查询信息！", L"警告");
		return;
	}
	wchar_t kNum = editSecVknum[0], nut[10];
	wcscpy(nut, editSecVnut);
	
	listSecVeg.DeleteAllItems();	//清空查询结果列表
	secVegByKindNut(kNum, nut, &listSecVeg);		//查询并添加到列表中
	listSecVeg.SortItems(CListCtrlCmp, (DWORD_PTR)&listSecVeg);		//列表排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空待查询的蔬菜基本信息
void CSecDlg::OnBnClickedEptv2()
{
	// TODO: 在此添加控件通知处理程序代码
	//listSecVeg.DeleteAllItems();
	//editSecVname.Empty();
	//editSecVknum.Empty();
	//editSecVnut.Empty();
	//UpdateData(FALSE);
	OnBnClickedEptv1();
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜名称/种植年份查询蔬菜种植信息
void CSecDlg::OnBnClickedSecp1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (editSecPVname.IsEmpty() && editSecPyear.IsEmpty())
	{
		MessageBox(L"请输入查询信息！", L"警告");
		return;
	}
	wchar_t subname[10], year[5];
	wcscpy(subname, editSecPVname);
	wcscpy(year, editSecPyear);
	listSecPlant.DeleteAllItems();	//清空查询结果列表
	
	secPlantByVnameYear(subname, year, &listSecPlant);	//查询并添加到列表中
	listSecPlant.SortItems(CListCtrlCmp, (DWORD_PTR)&listSecPlant);		//列表排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空待查询的蔬菜种植信息
void CSecDlg::OnBnClickedEptp1()
{
	// TODO: 在此添加控件通知处理程序代码
	listSecPlant.DeleteAllItems();
	editSecPVname.Empty();
	editSecPyear.Empty();
	editSecPVname2.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜名称查询蔬菜种植信息
void CSecDlg::OnBnClickedSecp2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (editSecPVname2.IsEmpty())
	{
		MessageBox(L"请输入查询信息！", L"警告");
		return;
	}
	wchar_t vName[10];
	wcscpy(vName, editSecPVname2);
	listSecPlant.DeleteAllItems();	//清空结果列表
	
	secPlantByVname(vName, &listSecPlant);	//查询并添加到列表
	listSecPlant.SortItems(PlantCmpByYear, (DWORD_PTR)&listSecPlant);	//列表按照年份排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空待查询的蔬菜种植信息
void CSecDlg::OnBnClickedEptp2()
{
	// TODO: 在此添加控件通知处理程序代码
	//listSecPlant.DeleteAllItems();
	//editSecPVname.Empty();
	//editSecPyear.Empty();
	//editSecPVname2.Empty();
	//UpdateData(FALSE);
	OnBnClickedEptp1();
}
