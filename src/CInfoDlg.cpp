// CInfoDlg.cpp: 实现文件
//数据内容窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CInfoDlg.h"
#include "VegData.h"

// CInfoDlg

IMPLEMENT_DYNCREATE(CInfoDlg, CFormView)

CInfoDlg::CInfoDlg()
	: CFormView(DIG_INFO)
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_KIND, list_kind);
	DDX_Control(pDX, LIST_VEG, list_veg);
	DDX_Control(pDX, LIST_PLANT, list_plant);
}

BEGIN_MESSAGE_MAP(CInfoDlg, CFormView)
	ON_NOTIFY(LVN_ITEMCHANGED, LIST_KIND, &CInfoDlg::OnLvnItemchangedKind)
	ON_NOTIFY(LVN_ITEMCHANGED, LIST_VEG, &CInfoDlg::OnLvnItemchangedVeg)
	ON_BN_CLICKED(BUT_SHOW, &CInfoDlg::OnBnClickedShow)
END_MESSAGE_MAP()


// CInfoDlg 诊断

#ifdef _DEBUG
void CInfoDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoDlg 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口内容
void CInfoDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//列表控件初始化

	//初始化表头
	CString kindHead[] = { L"分类编码",L"分类名称" }, vegHead[] = { L"蔬菜编号",L"蔬菜名称",L"营养成分" },
		plantHead[] = { L"编号",L"蔬菜编号",L"种植面积",L"收获重量",L"种植年份" };
	int kindWid[] = { 75,75 }, vegWid[] = { 75,75,400 }, plantWid[] = { 73,73,73,73,73 };

	//初始化3列表格
	initList(&list_kind, kindHead, kindWid, 2);
	initList(&list_veg, vegHead, vegWid, 3);
	initList(&list_plant, plantHead, plantWid, 5);

	//默认显示全部蔬菜种类信息、蔬菜基本信息、蔬菜种植信息
	showAllKind(&list_kind);
	showAllVeg(&list_veg,0);
	showAllPlant(&list_plant);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：种类列表选项改变响应
void CInfoDlg::OnLvnItemchangedKind(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//获取种类列表当前选中项
	CString checked;
	for (int i = 0; i < list_kind.GetItemCount(); i++)
		if (list_kind.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			checked = list_kind.GetItemText(i, 0);
			break;
		}

	if (checked.IsEmpty())		//若当前无选中项则结束
		return;
	
	//确定当前选项在数据结构中对应的蔬菜种类结构体
	
	Kind* tmpk;
	CString str;
	extern Kind* head;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		str.Format(L"%c", tmpk->k_num);
		if (str == checked)
			break;
	}

	//将选中蔬菜种类对应的蔬菜基本信息添加到蔬菜基本信息列表
	showVegPlantOfKind(tmpk, &list_veg, &list_plant);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：基本信息列表选项改变响应
void CInfoDlg::OnLvnItemchangedVeg(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//获取基本信息列表当前选中项
	CString checked;
	for (int i = 0; i < list_veg.GetItemCount(); i++)
	{
		if (list_veg.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			checked = list_veg.GetItemText(i, 0);
			break;
		}
	}
	if (checked.IsEmpty())	//若无选中项则结束
		return;

	//删除种植信息列表原有项
	list_plant.DeleteAllItems();

	//确定当前选项在数据结构中对应的蔬菜基本信息结构体
	
	extern Kind* head;
	Kind* tmpk;
	Veg* tmpv=NULL;
	Plant* tmpp;
	CString str;
	int flag = 1;
	for (tmpk = head->k_next; flag && tmpk; tmpk = tmpk->k_next)
	{
		for (tmpv=tmpk->k_vptr->v_next;tmpv;tmpv=tmpv->v_next)
		{
			str.Format(L"%d", tmpv->v_num);
			if (str==checked)
			{
				flag = 0;
				break;
			}
		}
	}
	
	//将选中蔬菜基本信息对应的种植信息添加到种植信息列表
	int i;
	for (i=0, tmpp = tmpv->v_pptr->p_next; tmpp;tmpp=tmpp->p_next,++i)
	{
		addPlantToList(&list_plant, tmpp, i);
	}
}

//////////////////////////////////////////////////////////////////////////
//函数功能：显示全部 按钮响应
void CInfoDlg::OnBnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码
	showAllVeg(&list_veg,0);
	showAllPlant(&list_plant);
}
