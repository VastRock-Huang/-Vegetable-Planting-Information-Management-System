// CIoDlg.cpp: 实现文件
//数据导入导出窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CIoDlg1.h"
#include "VegData.h"
#include "CInHelpDlg.h"


// CIoDlg

IMPLEMENT_DYNCREATE(CIoDlg1, CFormView)

CIoDlg1::CIoDlg1()
	: CFormView(DIG_IO)
	, editInPath(_T(""))
	, editOutPath1(_T(""))
	, editOutYear(_T(""))
	, editOutVname(_T(""))
	, editOutPath2(_T(""))
{

}

CIoDlg1::~CIoDlg1()
{
}

void CIoDlg1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, EDIT_INPATH, editInPath);
	DDX_Control(pDX, CMB_INMODE, cmbInMode);
	DDX_Text(pDX, EDIT_OUTPATH1, editOutPath1);
	DDX_Text(pDX, EDIT_OUTYEAR, editOutYear);
	DDV_MaxChars(pDX, editOutYear, 4);
	DDX_Control(pDX, LIST_OUT1, listOut1);
	DDX_Control(pDX, LIST_OUT2, listOut2);
	DDX_Text(pDX, EDIT_OUTVNAME, editOutVname);
	DDV_MaxChars(pDX, editOutVname, 9);
	DDX_Text(pDX, EDIT_OUTPATH2, editOutPath2);
}

BEGIN_MESSAGE_MAP(CIoDlg1, CFormView)
	ON_BN_CLICKED(BNT_PATH, &CIoDlg1::OnBnClickedPath)
	ON_BN_CLICKED(BNT_IN, &CIoDlg1::OnBnClickedIn)
	ON_BN_CLICKED(BNT_INC, &CIoDlg1::OnBnClickedInc)
	ON_BN_CLICKED(BNT_INHELP, &CIoDlg1::OnBnClickedInhelp)
	ON_BN_CLICKED(BNT_OUTPATH1, &CIoDlg1::OnBnClickedOutpath1)
	ON_BN_CLICKED(BNT_OUT1, &CIoDlg1::OnBnClickedOut1)
	ON_BN_CLICKED(BNT_GET1, &CIoDlg1::OnBnClickedGet1)
	ON_BN_CLICKED(BNT_OEPT1, &CIoDlg1::OnBnClickedOept1)
//	ON_WM_DESTROY()
	ON_BN_CLICKED(BNT_GET2, &CIoDlg1::OnBnClickedGet2)
	ON_BN_CLICKED(BNT_OUTPATH2, &CIoDlg1::OnBnClickedOutpath2)
	ON_BN_CLICKED(BNT_OUT2, &CIoDlg1::OnBnClickedOut2)
	ON_BN_CLICKED(BNT_OEPT2, &CIoDlg1::OnBnClickedOept2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CIoDlg 诊断

#ifdef _DEBUG
void CIoDlg1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CIoDlg1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CIoDlg 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：选择导入路径
void CIoDlg1::OnBnClickedPath()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, L"CSV (逗号分隔)(*.csv)|*.csv||", NULL);
	if (dlgFile.DoModal())
	{
		editInPath = dlgFile.GetPathName();		//获取导入文件路径
		UpdateData(FALSE);
	}

}


//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口
void CIoDlg1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	UpdateData(FALSE);

	cmbInMode.SetCurSel(0);

	CString head1[] = { L"年份",L"分类名称",L"种植面积",L"收获总重量" },
		head2[] = {L"年份",L"蔬菜名称",L"分类名称",L"种植面积",L"收获重量"};
	int wid1[] = { 52,73,73,90 }, wid2[] ={ 52,73,73,73,73 };
	initList(&listOut1, head1, wid1, 4);
	initList(&listOut2, head2, wid2, 5);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：导入表格数据
void CIoDlg1::OnBnClickedIn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	//确定导入信息类型
	int dataTpye;
	if (((CButton*)GetDlgItem(RAD_IN1))->GetCheck() == 1)
		dataTpye = 1;	//蔬菜种类信息
	else if (((CButton*)GetDlgItem(RAD_IN2))->GetCheck() == 1)
		dataTpye = 2;	//蔬菜基本信息
	else if (((CButton*)GetDlgItem(RAD_IN3))->GetCheck() == 1)
		dataTpye = 3;	//蔬菜种植信息
	else	//未选择文件类型返回
	{
		MessageBox(L"请选择导入文件类型", L"警告");
		return;
	}

	if (editInPath.IsEmpty())	//路径为空返回
	{
		MessageBox(L"请选择导入文件路径", L"警告");
		return;
	}
	if (IDCANCEL == MessageBox(L"请确保文件类型正确，否则会出现错误!!!", L"警告", MB_OKCANCEL))
		return;
	
	wchar_t filePah[100];	//文件路径
	int mode = cmbInMode.GetCurSel();	//导入模式: 0兼容, 1严格
	
	wcscpy(filePah, editInPath);

	int opt = inputFile(dataTpye, filePah, mode);	//导入数据
	switch(opt)
	{
	case 0:
		MessageBox(L"文件打开失败！", L"警告");
		editInPath.Empty();
		UpdateData(FALSE);
		return;

	case -1:
		MessageBox(L"数据内容有误！", L"警告");
		editInPath.Empty();
		UpdateData(FALSE);
		return;

	case 1:
		MessageBox(L"数据导入成功！", L"完成");
		OnBnClickedInc();
	}

}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空导入数据
void CIoDlg1::OnBnClickedInc()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(RAD_IN1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(RAD_IN2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(RAD_IN3))->SetCheck(FALSE);
	editInPath.Empty();
	cmbInMode.SetCurSel(0);
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：打开"帮助"对话框
void CIoDlg1::OnBnClickedInhelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CInHelpDlg tmp;
	tmp.DoModal();
}


//////////////////////////////////////////////////////////////////////////
//函数功能：获取导出路径1
void CIoDlg1::OnBnClickedOutpath1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	getFolderPath(&editOutPath1, m_hWnd);
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：数据导出1
void CIoDlg1::OnBnClickedOut1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (editOutPath1.IsEmpty())
	{
		MessageBox(L"请选择导出目录！", L"警告");
		return;
	}
	if (IDCANCEL == MessageBox(L"确定导出？", L"警告", MB_OKCANCEL))
		return;
	
	
	wchar_t filePath[MAX_PATH];		//导出路径
	editOutPath1 += L"\\按年份各类蔬菜信息统计表.csv";
	wcscpy(filePath, editOutPath1);
	if (outPlantOfKind(statPla, filePath) == 1)		//数据导出
	{
		MessageBox(L"导出成功！", L"完成");
	}
	else
	{
		MessageBox(L"导出失败！", L"警告");
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：统计并显示某年各类蔬菜种植信息
void CIoDlg1::OnBnClickedGet1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (editOutYear.IsEmpty())
	{
		MessageBox(L"请输入年份！", L"警告");
		return;
	}
	if (editOutYear.GetLength() < 4)
	{
		MessageBox(L"输入年份有误！", L"警告");
		editOutYear.Empty();
		return;
	}
	
	
	extern int kindCnt;
	if (statPla)
		delete[] statPla;
	statPla = new StatData[kindCnt];
	
	wchar_t year[5];
	wcscpy(year, editOutYear);
	statPlantOfKind(statPla, year);		//统计year年的种植信息
	listOut1.DeleteAllItems();		//清空列表
	addStatDataToList(&listOut1, statPla, kindCnt, 2);	//添加到列表
	((CButton*)GetDlgItem(BNT_OUT1))->EnableWindow(TRUE); //启用 导出 按钮
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空内容1
void CIoDlg1::OnBnClickedOept1()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(BNT_OUT1))->EnableWindow(FALSE); //关闭导出按钮
	listOut1.DeleteAllItems();
	editOutYear.Empty();
	editOutPath1.Empty();
	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//函数功能：统计并显示某蔬菜（如菠菜、黄瓜等）三年内种植信息
void CIoDlg1::OnBnClickedGet2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (editOutVname.IsEmpty())
	{
		MessageBox(L"请输入蔬菜名称！", L"警告");
		return;
	}
	
	wchar_t vName[10];
	wcscpy(vName, editOutVname);
	listOut2.DeleteAllItems();

	//判断蔬菜是否存在
	if (checkInVegByName(vName)==0)	
	{
		MessageBox(L"该蔬菜不存在！", L"警告");
		editOutVname.Empty();
		UpdateData(FALSE);
		return;
	}

	//仅输出近三年信息
	if (statPla)
		delete[] statPla;
	statPla = new StatData[3];
	statVegByLeastYear(statPla, vName);		//统计近三年蔬菜种植信息
	addStatDataToList(&listOut2, statPla, 3, 3);		//添加到列表
	((CButton*)GetDlgItem(BNT_OUT2))->EnableWindow(TRUE); //启用导出按钮
}


//////////////////////////////////////////////////////////////////////////
//函数功能：获取导出路径2
void CIoDlg1::OnBnClickedOutpath2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	getFolderPath(&editOutPath2, m_hWnd);
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：数据导出2
void CIoDlg1::OnBnClickedOut2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (editOutPath2.IsEmpty())
	{
		MessageBox(L"请选择导出目录！", L"警告");
		return;
	}
	if (IDCANCEL == MessageBox(L"确定导出？", L"警告", MB_OKCANCEL))
		return;

	
	wchar_t filePath[MAX_PATH];
	editOutPath2 += L"\\按蔬菜名称统计信息表.csv";
	wcscpy(filePath, editOutPath2);
	if (outPlantOfVeg(statPla, filePath) == 1)		//导出数据
	{
		MessageBox(L"导出成功！", L"完成");
	}
	else
	{
		MessageBox(L"导出失败！", L"警告");
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空内容2
void CIoDlg1::OnBnClickedOept2()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(BNT_OUT2))->EnableWindow(FALSE); //启用按钮
	listOut2.DeleteAllItems();
	;
	editOutVname.Empty();
	editOutPath2.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：注销窗口前数据处理
void CIoDlg1::OnDestroy()
{
	CFormView::OnDestroy();
	// TODO: 在此处添加消息处理程序代码

	if (statPla)	//若存有蔬菜数据, 则释放
		delete[] statPla;
}
