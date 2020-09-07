// CMtainDlg3.cpp: 实现文件
//蔬菜种植信息窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CMtainDlg3.h"
#include "VegData.h"

// CMtainDlg3

IMPLEMENT_DYNCREATE(CMtainDlg3, CFormView)

CMtainDlg3::CMtainDlg3()
	: CFormView(DIG_MAINTAIN3)
	, editPnum(_T(""))
	, editAddVname(_T(""))
	, editModVname(_T(""))
	, editAddArea(_T(""))
	, editAddWig(_T(""))
	, editAddYear(_T(""))
	, editModArea(_T(""))
	, editModWig(_T(""))
	, editModYear(_T(""))
{

}

CMtainDlg3::~CMtainDlg3()
{
}

void CMtainDlg3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_PLANT, listPlant);
	DDX_Text(pDX, EDIT_PNUM, editPnum);
	DDX_Control(pDX, CMB_VNUM, cmbAddVnum);
	DDX_Control(pDX, CMB_VNUM2, cmbModVnum);
	DDX_Text(pDX, EDIT_VNAME, editAddVname);
	DDX_Text(pDX, EDIT_VNAME2, editModVname);
	DDX_Control(pDX, CMB_PNUM, cmbPnum);
	DDX_Text(pDX, EDIT_ADDAREA, editAddArea);
	DDX_Text(pDX, EDIT_ADDWIG, editAddWig);
	DDX_Text(pDX, EDIT_ADDYEAR, editAddYear);
	DDV_MaxChars(pDX, editAddYear, 4);
	DDX_Text(pDX, EDIT_MODAREA, editModArea);
	DDX_Text(pDX, EDIR_MODWIG, editModWig);
	DDX_Text(pDX, EDIT_MODYEAR, editModYear);
	DDV_MaxChars(pDX, editModYear, 4);
}

BEGIN_MESSAGE_MAP(CMtainDlg3, CFormView)
	ON_CBN_SELCHANGE(CMB_VNUM, &CMtainDlg3::OnCbnSelchangeVnum)
	ON_CBN_SELCHANGE(CMB_VNUM2, &CMtainDlg3::OnCbnSelchangeVnum2)
	ON_BN_CLICKED(BNT_ADD, &CMtainDlg3::OnBnClickedAdd)
	ON_BN_CLICKED(BNT_ADDC, &CMtainDlg3::OnBnClickedAddc)
	ON_BN_CLICKED(BNT_DEL, &CMtainDlg3::OnBnClickedDel)
	ON_CBN_SELCHANGE(CMB_PNUM, &CMtainDlg3::OnCbnSelchangePnum)
	ON_BN_CLICKED(BNT_MOD, &CMtainDlg3::OnBnClickedMod)
	ON_BN_CLICKED(BNT_MODC, &CMtainDlg3::OnBnClickedModc)
END_MESSAGE_MAP()


// CMtainDlg3 诊断

#ifdef _DEBUG
void CMtainDlg3::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMtainDlg3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMtainDlg3 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口数据
void CMtainDlg3::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化列表
	CString plantHead[] = { L"编号",L"蔬菜编号",L"种植面积",L"收获重量",L"种植年份" };
	int plantWid[]= { 73, 73, 73, 73, 73 };
	initList(&listPlant, plantHead, plantWid, 5);	//初始化表头
	showAllPlant(&listPlant);	//显示所有蔬菜种植信息

	//初始化种植信息编号
	
	extern int plantIdx;
	editPnum.Format(L"%d", plantIdx + 1);
	UpdateData(FALSE);

	//更新下拉框
	updatPlantCmb(&cmbPnum);
	updateVegCmb(&cmbAddVnum);
	updateVegCmb(&cmbModVnum);
	//根据种植信息更新数据
	OnCbnSelchangePnum();
	OnCbnSelchangeVnum();
	OnCbnSelchangeVnum2();
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选择种植信息对应的蔬菜名称(用于添加种植信息)
void CMtainDlg3::OnCbnSelchangeVnum()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Veg tmpv;
	if (OnCbnChangeVeg(&cmbAddVnum, &tmpv) == CB_ERR)	//若蔬菜基本信息为0则返回
		return;
	editAddVname = tmpv.v_name;		//获取蔬菜名称
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选择种植信息对应的蔬菜名称(用于修改种植信息)
void CMtainDlg3::OnCbnSelchangeVnum2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Veg tmpv;
	if (OnCbnChangeVeg(&cmbModVnum, &tmpv) == CB_ERR)	//若蔬菜基本信息为0则返回
		return;
	editModVname = tmpv.v_name;		//获取蔬菜名称
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加蔬菜种植信息
void CMtainDlg3::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//若文本框为空则返回
	if (editAddArea.IsEmpty() || editAddWig.IsEmpty() || editAddYear.IsEmpty())
	{
		MessageBox(L"输入信息不完整！", L"警告");
		return;
	}

	Plant ipt;
	//收获重量合法性检测
	if (!(ipt.p_wegiht =(float) _wtof(editAddWig)))
	{
		MessageBox(L"输入收获重量有误！", L"警告");
		editAddArea.Empty();
		editAddWig.Empty();
		editAddYear.Empty();
		UpdateData(FALSE);
		return;
	}
	//种植年份合法性检测
	if (editAddYear.GetLength() < 4)
	{
		MessageBox(L"输入种植年份有误！", L"警告");
		editAddArea.Empty();
		editAddWig.Empty();
		editAddYear.Empty();
		UpdateData(FALSE);
		return;
	}
	CString str;
	cmbAddVnum.GetLBText(cmbAddVnum.GetCurSel(), str);	//获取添加信息对应的蔬菜编号
	//将添加数据存上蔬菜种植信息结构体
	ipt.p_vnum = _wtoi(str);
	ipt.p_area = _wtoi(editAddArea);
	wcscpy(ipt.p_year, editAddYear);

	
	addToPlant(&ipt);	//添加蔬菜种植信息
	MessageBox(L"录入成功！", L"完成");
	editAddArea.Empty();	//清空输入框
	editAddWig.Empty();
	editAddYear.Empty();
	UpdateData(FALSE);		//更新数据
	cmbAddVnum.SetCurSel(0);
	OnCbnSelchangeVnum();
	showAllPlant(&listPlant);
	updatPlantCmb(&cmbPnum);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空添加的蔬菜种植信息
void CMtainDlg3::OnBnClickedAddc()
{
	// TODO: 在此添加控件通知处理程序代码
	editAddArea.Empty();
	editAddWig.Empty();
	editAddYear.Empty();
	UpdateData(FALSE);
	cmbAddVnum.SetCurSel(0);
	OnCbnSelchangeVnum();
}


//////////////////////////////////////////////////////////////////////////
//函数功能：删除蔬菜种植信息
void CMtainDlg3::OnBnClickedDel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	extern int plantCnt;
	int i;
	for(i=0;i< plantCnt;++i)		//确定删除信息的索引
		if(listPlant.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
			break;
	if (i == plantCnt)		//若未选中信息
	{
		MessageBox(L"请选中待删除的蔬菜种植信息！", L"警告");
		return;
	}

	//提示框 询问确认删除
	CString str = L"确定删除该条信息？\r\n编号：" + listPlant.GetItemText(i, 0)
		+ L"\r\n蔬菜编号："+listPlant.GetItemText(i,1)
		+L"\t种植面积："+listPlant.GetItemText(i,2)
		+L"\r\n收获重量："+listPlant.GetItemText(i,3)
		+L"\t种植年份："+listPlant.GetItemText(i,4);
	if (IDCANCEL == MessageBox(str, L"警告", MB_OKCANCEL))		//若按下取消按钮则返回
		return;
	
	str = listPlant.GetItemText(i, 0);	//获取待删除种植信息编号
	delInPlant(_wtoi(str));		//删除选中的蔬菜种植信息
	listPlant.DeleteItem(i);	//在列表中删除
	updatPlantCmb(&cmbPnum);		//更新下拉框
	OnCbnSelchangePnum();		//更新种植信息数据
}


//////////////////////////////////////////////////////////////////////////
//函数功能：根据选择的种植信息更新数据
void CMtainDlg3::OnCbnSelchangePnum()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int idx = cmbPnum.GetCurSel();		//获取当前选择的种植信息的索引
	if (idx == CB_ERR)		//若种植信息为0则返回
		return;
	CString pNum = listPlant.GetItemText(idx, 1);	//获取当前选择的种植信息编号
	CString str;
	//更新种植信息对应的蔬菜编号的下拉框
	for (int i = 0; i < cmbModVnum.GetCount(); ++i)
	{
		cmbModVnum.GetLBText(i, str);
		if (str == pNum)
		{
			cmbModVnum.SetCurSel(i);
			OnCbnSelchangeVnum2();
			break;
		}
	}
	//更新蔬菜种植信息到文本框
	editModArea = listPlant.GetItemText(idx, 2);
	editModWig = listPlant.GetItemText(idx, 3);
	editModYear = listPlant.GetItemText(idx, 4);
	UpdateData(FALSE);	
}


//////////////////////////////////////////////////////////////////////////
//函数功能：修改蔬菜种植信息
void CMtainDlg3::OnBnClickedMod()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//文本框为空则返回
	if (editModArea.IsEmpty() || editModWig.IsEmpty() || editModYear.IsEmpty())
	{
		MessageBox(L"无修改信息！", L"警告");
		OnCbnSelchangePnum();
		return;
	}

	int idx = cmbPnum.GetCurSel();		//记录修改的种植信息索引
	CString pNum=listPlant.GetItemText(idx,0),		//记录修改项原信息
		oldVnum = listPlant.GetItemText(idx, 1),
		oldArea = listPlant.GetItemText(idx, 2),
		oldWig = listPlant.GetItemText(idx, 3),
		oldYear = listPlant.GetItemText(idx, 4);

	CString modVnum;
	cmbModVnum.GetLBText(cmbModVnum.GetCurSel(), modVnum);	//获取修改后种植信息对应的蔬菜编号

	int f1 = (oldVnum == modVnum) ? 1 : 0;		//蔬菜编号标记
	int f2 = (oldArea == editModArea) ? 1 : 0;	//种植面积标记
	int f3 = (oldWig == editModWig) ? 1 : 0;	//收获重量标记
	int f4 = (oldYear == editModYear) ? 1 : 0;	//种植年份标记

	if (f1 && f2 && f3 && f4)	//未修改信息则返回
	{
		MessageBox(L"修改信息与原来相同！", L"警告");
		OnCbnSelchangePnum();
		return;
	}
	if (editModYear.GetLength() < 4)
	{
		MessageBox(L"输入种植年份有误！", L"警告");
		OnCbnSelchangePnum();
		return;
	}

	Plant ipt;
	if (!(ipt.p_wegiht = (float)_wtof(editModWig)))
	{
		MessageBox(L"修改收获重量有误！", L"警告");
		OnCbnSelchangePnum();
		return;
	}

	//将修改后信息存入结构体
	ipt.p_num = _wtoi(pNum);
	ipt.p_vnum = _wtoi(modVnum);
	ipt.p_area = _wtoi(editModArea);
	wcscpy(ipt.p_year, editModYear);

	
	modInPlant(&ipt, _wtoi(oldVnum));	//修改蔬菜种植信息
	MessageBox(L"修改成功！", L"完成");
	//更新种植信息
	showAllPlant(&listPlant);
	OnCbnSelchangePnum();
}


//////////////////////////////////////////////////////////////////////////
//函数功能：重置修改的蔬菜种植信息
void CMtainDlg3::OnBnClickedModc()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCbnSelchangePnum();
}
