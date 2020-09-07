// CMtainDlg.cpp: 实现文件
//蔬菜种类信息维护窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CMtainDlg1.h"
#include "VegData.h"

// CMtainDlg

IMPLEMENT_DYNCREATE(CMtainDlg1, CFormView)

CMtainDlg1::CMtainDlg1()
	: CFormView(DIG_MAINTIAN1)
	, editAddNum(_T(""))
	, editModNum(_T(""))
	, editAddName(_T(""))
	, editModName(_T(""))
{

}

CMtainDlg1::~CMtainDlg1()
{
}

void CMtainDlg1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_KIND, listKind);
	DDX_Text(pDX, EDIT_ADDNUM, editAddNum);
	DDV_MaxChars(pDX, editAddNum, 1);
	DDX_Text(pDX, EDIT_MODNUM, editModNum);
	DDV_MaxChars(pDX, editModNum, 1);
	DDX_Control(pDX, CMB_NUM, cmbNum);
	DDX_Text(pDX, EDIT_ADDNAME, editAddName);
	DDV_MaxChars(pDX, editAddName, 6);
	DDX_Text(pDX, EDIT_MODNAME, editModName);
	DDV_MaxChars(pDX, editModName, 6);
}

BEGIN_MESSAGE_MAP(CMtainDlg1, CFormView)
ON_BN_CLICKED(BNT_ADD, &CMtainDlg1::OnBnClickedAdd)
ON_BN_CLICKED(BNT_ADDC, &CMtainDlg1::OnBnClickedAddc)
ON_BN_CLICKED(BNT_DEL, &CMtainDlg1::OnBnClickedDel)
ON_BN_CLICKED(BNT_MOD, &CMtainDlg1::OnBnClickedMod)
ON_BN_CLICKED(BNT_MODC, &CMtainDlg1::OnBnClickedModc)
ON_CBN_SELCHANGE(CMB_NUM, &CMtainDlg1::OnCbnSelchangeNum)
END_MESSAGE_MAP()


// CMtainDlg 诊断

#ifdef _DEBUG
void CMtainDlg1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMtainDlg1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMtainDlg 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口数据
void CMtainDlg1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	//初始化列表
	CString kindHead[] = { L"分类编码",L"分类名称" };
	int kindWid[] = { 90,90 };

	initList(&listKind, kindHead, kindWid, 2);	//初始化种类信息表头
	showAllKind(&listKind);		//显示所有种类信息
	
	updateKindCmb(&cmbNum);		//更新蔬菜种类下拉框
	OnCbnSelchangeNum();		//更新数据
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加蔬菜种类信息
void CMtainDlg1::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);		//更新数据到后台
	//输入信息合法性检测
	if (editAddName.IsEmpty()||editAddNum.IsEmpty())
	{
		MessageBox(L"输入信息不完整！",L"警告");
		return;
	}

	//将添加数据存入蔬菜种类结构体
	Kind ipt;	
	ipt.k_num = editAddNum[0];
	editAddName += L"类";
	wcscpy(ipt.k_name, editAddName);

	
	//判断数据是否重复
	if (checkInKind(ipt.k_num,ipt.k_name,2))
	{
		MessageBox(L"信息存在重复！", L"警告");
		editAddNum.Empty();
		editAddName.Empty();
		UpdateData(FALSE);
		return;
	}
	//添加数据
	addToKind(&ipt);		//将ipt中的信息添加到蔬菜数据结构中
	MessageBox(L"录入成功！", L"完成");
	editAddNum.Empty();		//清空输入框
	editAddName.Empty();
	UpdateData(FALSE);		//更新数据到窗口
	showAllKind(&listKind);		//更新列表
	updateKindCmb(&cmbNum);		//更新下拉框
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空添加的蔬菜种类信息
void CMtainDlg1::OnBnClickedAddc()
{
	// TODO: 在此添加控件通知处理程序代码
	editAddNum.Empty();		//清空输入框
	editAddName.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：删除蔬菜种类信息
void CMtainDlg1::OnBnClickedDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int i, len = listKind.GetItemCount();		//i记录删除种类的索引i>=0
	for (i = 0; i < len; i++)		//获取列表当前选中项
	{
		if (listKind.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			break;
	}
	if (i == len)		//若未选中
	{
		MessageBox(L"请选中待删除的蔬菜种类信息！", L"警告");
		return;
	}
	//提示框 询问确定删除
	CString str = L"确定删除 " + listKind.GetItemText(i, 1) + L" ?";
	if (IDCANCEL == MessageBox(str, L"警告", MB_OKCANCEL))		//若按下取消按钮则返回
		return;
	//删除数据
	
	delInKind(i + 1);	//将第i项蔬菜种类从蔬菜数据结构中删除
	listKind.DeleteItem(i);		//将列表中第i项删除
	updateKindCmb(&cmbNum);		//更新下拉框
	OnCbnSelchangeNum();		//蔬菜种类下拉框中选中一项后数据更新
}


//////////////////////////////////////////////////////////////////////////
//函数功能： 修改蔬菜种类信息
void CMtainDlg1::OnBnClickedMod()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//文本框为空返回
	if (editModNum.IsEmpty() || editModName.IsEmpty())
	{
		MessageBox(L"无修改信息！", L"警告");
		OnCbnSelchangeNum();
		return;
	}

	int idx = cmbNum.GetCurSel();		//idx记录修改种类索引idx>=0
	//记录修改项原信息: 分类编码 分类名称
	CString oldNum = listKind.GetItemText(idx, 0), 
	oldName = listKind.GetItemText(idx, 1);		
	editModName += L"类";	//修改后分类名称

	int f1 = (oldNum == editModNum) ? 1 : 0;	//编号标记: 1表示编号未修改
	int f2 = (oldName == editModName) ? 1 : 0;	//名称标记: 1表示名称未修改

	if (f1 && f2)	//分类名称和编号均为修改则返回
	{
		MessageBox(L"修改信息与原来相同！", L"警告");
		OnCbnSelchangeNum();
		return;
	}

	Kind ipt;	//将修改后信息存入结构体
	ipt.k_num = editModNum[0];
	wcscpy(ipt.k_name, editModName);

	
	//判断输入信息是否已经存在
	if ((f1==0&&checkInKind(ipt.k_num,ipt.k_name, 0))||(f2 == 0 && checkInKind(ipt.k_num,ipt.k_name,1)))
	{
		MessageBox(L"修改信息已存在！", L"警告");
		OnCbnSelchangeNum();
		return;
	}

	//修改数据
	modInKind(idx+1,&ipt);		//修改蔬菜数据结构中第idx+1项数据
	MessageBox(L"修改成功!", L"完成");
	showAllKind(&listKind);		//更新列表
	updateKindCmb(&cmbNum);		//更新下拉框
	OnCbnSelchangeNum();

}


//////////////////////////////////////////////////////////////////////////
//函数功能：重置修改的蔬菜种类信息
void CMtainDlg1::OnBnClickedModc()
{
	// TODO: 在此添加控件通知处理程序代码
	//editModName.Empty();
	//editModNum.Empty();
	OnCbnSelchangeNum();	//重置数据
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：根据当前选择的蔬菜种类更新数据
void CMtainDlg1::OnCbnSelchangeNum()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int idx = cmbNum.GetCurSel();		//确定当前选择的蔬菜种类
	if (idx == -1)		//若当前蔬菜种类为0则(idx==-1)返回
		return;
	//更新蔬菜种类信息到文本框
	editModNum = listKind.GetItemText(idx, 0);
	editModName = listKind.GetItemText(idx, 1);
	editModName.TrimRight(L"类");
	UpdateData(FALSE);
}
