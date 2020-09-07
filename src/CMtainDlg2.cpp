// CMtainDlg2.cpp: 实现文件
//蔬菜基本信息维护窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CMtainDlg2.h"
#include "VegData.h"

// CMtainDlg2

IMPLEMENT_DYNCREATE(CMtainDlg2, CFormView)

CMtainDlg2::CMtainDlg2()
	: CFormView(DIG_MAINTAIN2)
	, editAddName(_T(""))
	, editModName(_T(""))
	, editAddNut(_T(""))
	, editModNut(_T(""))
	, editVnum(_T(""))
{

}

CMtainDlg2::~CMtainDlg2()
{
}

void CMtainDlg2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_VEG, listVeg);
	DDX_Text(pDX, EDIT_ADDVNAME, editAddName);
	DDV_MaxChars(pDX, editAddName, 9);
	DDX_Text(pDX, EDIT_MODVNAME, editModName);
	DDV_MaxChars(pDX, editModName, 9);
	DDX_Text(pDX, EDIT_ADDNUT, editAddNut);
	DDV_MaxChars(pDX, editAddNut, 39);
	DDX_Control(pDX, CMB_ADDKNUM, cmbAddKind);
	DDX_Control(pDX, CMB_MODKNUM, cmbModKind);
	DDX_Control(pDX, LIST_VEG, listVeg);
	DDX_Control(pDX, CMB_VNUM, cmbVnum);
	DDX_Text(pDX, EDIT_MODNUT, editModNut);
	DDV_MaxChars(pDX, editModNut, 39);
	DDX_Text(pDX, EDIT_VNUM, editVnum);
}

BEGIN_MESSAGE_MAP(CMtainDlg2, CFormView)
	ON_BN_CLICKED(BNT_ADD, &CMtainDlg2::OnBnClickedAdd)
	ON_BN_CLICKED(BNT_ADDC, &CMtainDlg2::OnBnClickedAddc)
	ON_BN_CLICKED(BNT_DEL, &CMtainDlg2::OnBnClickedDel)
	ON_BN_CLICKED(BNT_MOD, &CMtainDlg2::OnBnClickedMod)
	ON_CBN_SELCHANGE(CMB_VNUM, &CMtainDlg2::OnCbnSelchangeVnum)
	ON_BN_CLICKED(BNT_MODC, &CMtainDlg2::OnBnClickedModc)
END_MESSAGE_MAP()


// CMtainDlg2 诊断

#ifdef _DEBUG
void CMtainDlg2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMtainDlg2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMtainDlg2 消息处理程序


//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口数据
void CMtainDlg2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化蔬菜基本信息列表
	CString vegHead[] = { L"蔬菜编号",L"蔬菜名称",L"分类码",L"营养成分" };
	int vegWid[] = { 75,75,60,400 };
	initList(&listVeg, vegHead, vegWid, 4);	//初始化表头
	showAllVeg(&listVeg,1);	//显示所有蔬菜基本信息

	//初始化蔬菜编号
	
	extern int vegIdx;
	editVnum.Format(L"%d", vegIdx + 1);
	UpdateData(FALSE);

	//初始化下拉框
	updateKindCmb(&cmbAddKind);		//更新蔬菜种类下拉框
	updateKindCmb(&cmbModKind);
	updateVegCmb(&cmbVnum);		//更新蔬菜基本信息下拉框
	OnCbnSelchangeVnum();		//根据蔬菜更新数据
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加数蔬菜基本信息
void CMtainDlg2::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//判断输入信息是否完整
	if (editAddName.IsEmpty()|| editAddNut.IsEmpty())
	{
		MessageBox(L"输入信息不完整！", L"警告");
		return;
	}

	//将添加数据存上蔬菜基本信息结构体
	Veg ipt;
	wcscpy(ipt.v_name, editAddName);
	CString addKind;
	cmbAddKind.GetLBText(cmbAddKind.GetCurSel(), addKind);
	ipt.v_kind = addKind[0];
	wcscpy(ipt.v_nutrition, editAddNut);

	
	//判断添加信息是否重复
	if (checkInVegByName(ipt.v_name))
	{
		MessageBox(L"信息存在重复！", L"警告");
		editAddName.Empty();
		editAddNut.Empty();
		UpdateData(FALSE);
		return;
	}
	extern int vegIdx;
	addToVeg(&ipt);		//添加蔬菜基本信息到蔬菜数据结构
	MessageBox(L"录入成功！", L"完成");
	editAddName.Empty();		//清空输入框
	editAddNut.Empty();
	editVnum.Format(L"%d", vegIdx + 1);
	UpdateData(FALSE);			//更新数据
	cmbAddKind.SetCurSel(0);
	showAllVeg(&listVeg,1);
	updateVegCmb(&cmbVnum);
	
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空添加的蔬菜基本信息
void CMtainDlg2::OnBnClickedAddc()
{
	// TODO: 在此添加控件通知处理程序代码
	editAddName.Empty();
	editAddNut.Empty();
	UpdateData(FALSE);
	cmbAddKind.SetCurSel(0);
	showAllVeg(&listVeg, 1);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：删除蔬菜基本信息
void CMtainDlg2::OnBnClickedDel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	extern int vegCnt;
	int i;		//i记录删除蔬菜信息的索引i>=0
	for (i = 0; i < vegCnt; ++i)		//获取列表当前选中项
	{
		if (listVeg.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			break;
	}
	if (i == vegCnt)		//若未选中
	{
		MessageBox(L"请选中待删除的蔬菜基本信息！", L"警告");
		return;
	}
	//提示框 询问确定删除
	CString str = L"确定删除 " + listVeg.GetItemText(i, 1) + L" ?";
	if (IDCANCEL == MessageBox(str, L"警告", MB_OKCANCEL))		//若按下取消按钮则返回
		return;
	//删除数据
	str = listVeg.GetItemText(i, 0);
	delInVeg(_wtoi(str));	//将该蔬菜基本信息从蔬菜数据结构中删除
	listVeg.DeleteItem(i);		//将列表中第i项删除
	updateVegCmb(&cmbVnum);		//更新下拉框
	OnCbnSelchangeVnum();		//更新蔬菜数据
}


//////////////////////////////////////////////////////////////////////////
//函数功能：修改蔬菜基本信息
void CMtainDlg2::OnBnClickedMod()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//文本框为空则返回
	if (editModName.IsEmpty() || editModNut.IsEmpty())
	{
		MessageBox(L"无修改信息！", L"警告");
		OnCbnSelchangeVnum();
		return;
	}

	int idx = cmbVnum.GetCurSel();	//记录修改的蔬菜信息索引
	CString vNum=listVeg.GetItemText(idx,0),	//记录修改项的原信息
		oldName=listVeg.GetItemText(idx,1),
		oldKind = listVeg.GetItemText(idx, 2),
		oldNut = listVeg.GetItemText(idx, 3);

	CString modKind;
	cmbModKind.GetLBText(cmbModKind.GetCurSel(), modKind);	//获取修改后的蔬菜的种类

	int f1 = (editModName == oldName) ? 1 : 0;		//名称标记
	int f2 = (oldKind[0] == modKind[0]) ? 1 : 0;	//种类标记
	int f3 = (editModNut == oldNut) ? 1 : 0;	//营养标记
	
	if (f1 && f2 && f3)		//未修改信息则返回
	{
		MessageBox(L"修改信息与原来相同！", L"警告");
		OnCbnSelchangeVnum();
		return;
	}

	Veg ipt;	//将修改后信息存入结构体
	ipt.v_num = _wtoi(vNum);
	wcscpy(ipt.v_name, editModName);
	ipt.v_kind = modKind[0];
	wcscpy(ipt.v_nutrition, editModNut);

	
	if (f1 == 0 && checkInVegByName(ipt.v_name))	//判断修改后的信息是否存在
	{
		MessageBox(L"修改信息已存在！", L"警告");
		OnCbnSelchangeVnum();
		return;
	}

	modInVeg(&ipt, oldKind[0]);	//修改蔬菜基本信息
	MessageBox(L"修改成功!", L"完成");
	showAllVeg(&listVeg,1);		//更新蔬菜信息列表
	//updateVegCmb(cmbVnum);
	UpdateData(FALSE);
	OnCbnSelchangeVnum();
}


//////////////////////////////////////////////////////////////////////////
//函数功能：根据当前选择的蔬菜更新数据
void CMtainDlg2::OnCbnSelchangeVnum()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Veg tmpv;
	int idx = OnCbnChangeVeg(&cmbVnum,&tmpv);	//确定当前选择的蔬菜基本信息
	if (idx == -1)		//若当前蔬菜基本信息为0(idx=-1)则返回
		return;
	//更新蔬菜基本信息到文本框
	editModName = tmpv.v_name;
	editModNut = tmpv.v_nutrition;
	cmbModKind.SetCurSel(idx);	//更新下拉框
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：重置修改的蔬菜种类信息
void CMtainDlg2::OnBnClickedModc()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCbnSelchangeVnum();
}
