// CIoDlg2.cpp: 实现文件
//自定义数据导出窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CIoDlg2.h"
#include "VegData.h"

// CIoDlg2

IMPLEMENT_DYNCREATE(CIoDlg2, CFormView)

CIoDlg2::CIoDlg2()
	: CFormView(DIG_IO2)
	, editVname(_T(""))
	, editYear1(_T(""))
	, editYear2(_T(""))
	, editArea1(_T(""))
	, editArea2(_T(""))
	, editWig1(_T(""))
	, editWig2(_T(""))
{

}

CIoDlg2::~CIoDlg2()
{
}

void CIoDlg2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, CMB_KIND, cmbKind);
	DDX_Control(pDX, LIST_OUT, listOut);
	DDX_Text(pDX, EDIT_VNAME, editVname);
	DDV_MaxChars(pDX, editVname, 9);
	DDX_Text(pDX, EDIT_YEAR1, editYear1);
	DDV_MaxChars(pDX, editYear1, 4);
	DDX_Text(pDX, EDIT_YEAR2, editYear2);
	DDV_MaxChars(pDX, editYear2, 4);
	DDX_Text(pDX, EDIT_AREA1, editArea1);
	DDX_Text(pDX, EDIT_AREA2, editArea2);
	DDX_Text(pDX, EDIT_WIG1, editWig1);
	DDX_Text(pDX, EDIT_WIG2, editWig2);
}

BEGIN_MESSAGE_MAP(CIoDlg2, CFormView)
	ON_BN_CLICKED(RAD_KIND, &CIoDlg2::OnBnClickedKind)
	ON_BN_CLICKED(RAD_VEG, &CIoDlg2::OnBnClickedVeg)
	ON_BN_CLICKED(RAD_YEAR1, &CIoDlg2::OnBnClickedYear1)
	ON_BN_CLICKED(RAD_YEAR2, &CIoDlg2::OnBnClickedYear2)
	ON_BN_CLICKED(CHK_KNAME, &CIoDlg2::OnBnClickedKname)
	ON_BN_CLICKED(CHK_VNAME, &CIoDlg2::OnBnClickedVname)
	ON_BN_CLICKED(CHE_KNUM, &CIoDlg2::OnBnClickedKnum)
	ON_BN_CLICKED(CHK_NUT, &CIoDlg2::OnBnClickedNut)
	ON_BN_CLICKED(CHK_AREA, &CIoDlg2::OnBnClickedArea)
	ON_BN_CLICKED(CHK_WIG, &CIoDlg2::OnBnClickedWig)
	ON_BN_CLICKED(CHK_YEAR, &CIoDlg2::OnBnClickedYear)
	ON_BN_CLICKED(BNT_OUT, &CIoDlg2::OnBnClickedOut)
	ON_BN_CLICKED(BNT_GET, &CIoDlg2::OnBnClickedGet)
	ON_BN_CLICKED(CHK_ALL, &CIoDlg2::OnBnClickedAll)
	ON_WM_DESTROY()
	ON_BN_CLICKED(BNT_OPT, &CIoDlg2::OnBnClickedOpt)
	ON_EN_CHANGE(EDIT_YEAR1, &CIoDlg2::OnEnChangeYear1)
	ON_EN_CHANGE(EDIT_YEAR2, &CIoDlg2::OnEnChangeYear2)
	ON_EN_CHANGE(EDIT_AREA1, &CIoDlg2::OnEnChangeArea1)
	ON_EN_CHANGE(EDIT_AREA2, &CIoDlg2::OnEnChangeArea2)
	ON_EN_CHANGE(EDIT_WIG1, &CIoDlg2::OnEnChangeWig1)
	ON_EN_CHANGE(EDIT_WIG2, &CIoDlg2::OnEnChangeWig2)
END_MESSAGE_MAP()


// CIoDlg2 诊断

#ifdef _DEBUG
void CIoDlg2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CIoDlg2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CIoDlg2 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：初始化窗口
void CIoDlg2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	updateKindCmb(&cmbKind);		//更新蔬菜种类下拉框
	cmbKind.InsertString(0, L"全部");		//插入"全部"选项
	//cmbKind.SetCurSel(0);		
	((CButton*)GetDlgItem(RAD_KIND))->SetCheck(TRUE);		//默认导出类型
	OnBnClickedKind();
	((CButton*)GetDlgItem(RAD_YEAR2))->SetCheck(TRUE);		//默认年份设置
	OnBnClickedYear2();
	listOut.SetExtendedStyle(listOut.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| WS_HSCROLL);	//初始化列表样式
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选择 蔬菜种类信息导出
void CIoDlg2::OnBnClickedKind()
{
	// TODO: 在此添加控件通知处理程序代码
	outFlag = 0;	//导出标识为蔬菜种类
	//设置蔬菜种类框可见, 蔬菜基本信息框不可见
	((CStatic*)GetDlgItem(STA_KIND))->ShowWindow(TRUE);
	((CComboBox*)GetDlgItem(CMB_KIND))->ShowWindow(TRUE);
	((CStatic*)GetDlgItem(STA_VNAME))->ShowWindow(FALSE);
	((CEdit*)GetDlgItem(EDIT_VNAME))->ShowWindow(FALSE);

	//将所有信息复选框取消选择
	for (int i = 1; i < CHKCNT; ++i)
	{
		((CButton*)GetDlgItem(i+CHK_KNAME))->SetCheck(FALSE);
		checkFlag[i] = 0;
	}
	((CButton*)GetDlgItem(CHK_KNAME))->SetCheck(TRUE);		//分类名称复选框必选
	((CButton*)GetDlgItem(CHK_KNAME))->EnableWindow(FALSE);	//分类名称复选框锁定
	((CButton*)GetDlgItem(CHK_VNAME))->EnableWindow(TRUE);	//蔬菜名称复选框解锁
	((CButton*)GetDlgItem(BNT_OUT))->EnableWindow(FALSE);	//导出按钮关闭
	checkFlag[0] = 1;
	
	//清空列表所有列
	for (int i = 0; i < col; ++i)
		listOut.DeleteColumn(0);
	listOut.DeleteAllItems();
	listOut.InsertColumn(0, checkTxt[0], LVCFMT_LEFT, checkWid[0]);		//插入 分类名称 列
	checkFlag[0] = col = 1;		//复选框 分类名称标记置1, 列表列数为1
	dataCnt = 0;	//数据条数置0
	cmbKind.SetCurSel(0);	//种类默认选项
	editVname.Empty();		//蔬菜名称框清空
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选择 蔬菜基本信息导出
void CIoDlg2::OnBnClickedVeg()
{
	// TODO: 在此添加控件通知处理程序代码
	outFlag = 1;	//导出标识为蔬菜基本信息
	//设置蔬菜种类框不可见, 蔬菜基本信息框可见
	(CStatic*)GetDlgItem(STA_KIND)->ShowWindow(FALSE);
	(CComboBox*)GetDlgItem(CMB_KIND)->ShowWindow(FALSE);
	(CStatic*)GetDlgItem(STA_VNAME)->ShowWindow(TRUE);
	(CEdit*)GetDlgItem(EDIT_VNAME)->ShowWindow(TRUE);

	//将所有信息复选框取消选择
	for (int i = 0; i < CHKCNT; ++i)
	{
		((CButton*)GetDlgItem(i + CHK_KNAME))->SetCheck(FALSE);
		checkFlag[i] = 0;
	}
	((CButton*)GetDlgItem(CHK_VNAME))->SetCheck(TRUE);		//蔬菜名称复选框必选
	((CButton*)GetDlgItem(CHK_VNAME))->EnableWindow(FALSE);	//蔬菜名称复选框锁定
	((CButton*)GetDlgItem(CHK_KNAME))->EnableWindow(TRUE);	//分类名称复选框必选
	((CButton*)GetDlgItem(BNT_OUT))->EnableWindow(FALSE);	//导出按钮关闭

	//清空列表所有列
	for (int i = 0; i < col; ++i)
		listOut.DeleteColumn(0);
	listOut.DeleteAllItems();
	listOut.InsertColumn(0, checkTxt[1], LVCFMT_LEFT, checkWid[1]);		//插入蔬菜名称列
	checkFlag[1] = col = 1;	//复选框 蔬菜名称标记置1, 列表列数为1
	dataCnt = 0;	//数据条数置0
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选择 具体年份
void CIoDlg2::OnBnClickedYear1()
{
	// TODO: 在此添加控件通知处理程序代码
	yearFlag = 0;	//年份标识置0
	//第2个年份框不可见
	((CStatic*)GetDlgItem(STA_YEAR))->ShowWindow(FALSE);
	((CEdit*)GetDlgItem(EDIT_YEAR2))->ShowWindow(FALSE);
	editYear2.Empty();		//清空第2个年份框内容
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选择 年份区间
void CIoDlg2::OnBnClickedYear2()
{
	// TODO: 在此添加控件通知处理程序代码
	yearFlag = 1;	//年份标识置1
	//第2个年份框可见
	((CStatic*)GetDlgItem(STA_YEAR))->ShowWindow(TRUE);
	((CEdit*)GetDlgItem(EDIT_YEAR2))->ShowWindow(TRUE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：点击复选框响应
//入口参数：idx 选择的复选框序号
//返回值：无
void CIoDlg2::OnCheckBox(int idx)
{
	int i;
	CButton* tmp;
	//遍历复选框, 若有可选框未选择,则全选框未选中
	for (i = 0; i < 7; ++i)
	{
		tmp = (CButton*)GetDlgItem(i + CHK_KNAME);
		if (tmp->IsWindowEnabled()==TRUE&&tmp->GetCheck()==FALSE)
		{
			((CButton*)GetDlgItem(CHK_ALL))->SetCheck(FALSE);
			break;
		}
	}
	if(i==7)	//复选框都选中了, 则全选框选中
		((CButton*)GetDlgItem(CHK_ALL))->SetCheck(TRUE);

	int checkState = ((CButton*)GetDlgItem(CHK_KNAME + idx))->GetCheck();	//获取选择的复选框状态
	if (checkState == TRUE)		//若复选框选中
	{
		listOut.InsertColumn(col, checkTxt[idx], LVCFMT_LEFT, checkWid[idx]);	//列表中增加该列
		checkFlag[idx] = ++col;		//列表列数+1, 设定对应复选框标识
		insertToListOut();		//添加数据到列表
	}
	else	//若该复选框未选中,从列表中移除该列数据
	{
		//复选框标识调整
		for (int i = 0; i < CHKCNT; ++i)
		{
			if (checkFlag[i] > checkFlag[idx])
				--checkFlag[i];
		}
		checkFlag[idx] = 0;
		//删除列表中复选框对应列
		TCHAR tcText[10] = { 0 };	  //获取当前点击列 列表头
		HDITEM hdItem;
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = tcText;
		hdItem.cchTextMax = 10;
		//删除该列
		for (int i = 0; listOut.GetHeaderCtrl()->GetItem(i, &hdItem); ++i)
		{
			if (wcscmp(hdItem.pszText, checkTxt[idx]) == 0)
			{
				listOut.DeleteColumn(i);
				--col;
				break;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：选中对应的复选框

void CIoDlg2::OnBnClickedKname()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(0);
}


void CIoDlg2::OnBnClickedVname()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(1);
}



void CIoDlg2::OnBnClickedKnum()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(2);
}


void CIoDlg2::OnBnClickedNut()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(3);
}


void CIoDlg2::OnBnClickedArea()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(4);
}


void CIoDlg2::OnBnClickedWig()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(5);
}


void CIoDlg2::OnBnClickedYear()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCheckBox(6);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：数据导出
void CIoDlg2::OnBnClickedOut()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//打开导出另存为窗口
	CFileDialog fileDlg(FALSE, _T("csv"), L"VegInfo1", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"CSV (逗号分隔)(*.csv)|*.csv||", this);

	if (IDOK == fileDlg.DoModal())
	{
		CString str = fileDlg.GetPathName();	//获取导出路径
		wchar_t filePath[MAX_PATH];
		wcscpy(filePath, str);
		if (outCustzFile(staPla, filePath, dataCnt, col, checkFlag) == 1)	//数据导出
		{
			MessageBox(L"导出成功！", L"完成");
			return;
		}
		else
		{
			MessageBox(L"导出失败！", L"警告");
			return;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：统计数据
void CIoDlg2::OnBnClickedGet()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	int kIdx;	//蔬菜种类索引
	wchar_t vName[10];
	wchar_t year1[5], year2[5];
	int area1 = -1, area2 = -1;
	float wig1 = -1.f, wig2 = -1.f;

	if (outFlag == 0)
	{
		kIdx = cmbKind.GetCurSel() - 1;		//获取蔬菜种类索引
	}
	else if (outFlag == 1)
	{	//判断输入蔬菜是否存在
		wcscpy(vName, editVname);
		if (!editVname.IsEmpty() && checkInVegByName(vName) == 0)
		{
			MessageBox(L"蔬菜不存在！", L"警告");
			editVname.Empty();
			UpdateData(FALSE);
			return;
		}
	}
	if (yearFlag == 0)	 //选则具体年份
	{	
		if (editYear1.IsEmpty())
		{
			MessageBox(L"请输入具体年份", L"警告");
			return;
		}
		if (editYear1.GetLength() < 4)
		{
			MessageBox(L"请输入正确年份", L"警告");
			editYear1.Empty();
			UpdateData(FALSE);
			return;
		}
		wcscpy(year1, editYear1);
		wcscpy(year2, L"NaN");
	}
	else	//选择年份区间
	{
		int f1 = !editYear1.IsEmpty(), f2 = !editYear2.IsEmpty();
		if (f1 == 1 && editYear1.GetLength() < 4
			|| (f2 == 1 && editYear2.GetLength() < 4))
		{
			MessageBox(L"请输入正确年份", L"警告");
			editYear1.Empty();
			editYear2.Empty();
			UpdateData(FALSE);
			return;
		}
		if (f1==1&&f2==1&&editYear1>editYear2)
		{
			MessageBox(L"输入年份顺序有误！", L"警告");
			editYear1.Empty();
			editYear2.Empty();
			UpdateData(FALSE);
			return;
		}
		wcscpy(year1, editYear1);
		wcscpy(year2, editYear2);
	}
	
	if (editArea1)
		area1 = _wtoi(editArea1);
	if (editArea2)
		area2 = _wtoi(editArea2);
	if (!editWig1.IsEmpty() && (wig1 = (float)_wtof(editWig1)) == 0)	//输入收获重量无法转换成非零浮点数
	{
		MessageBox(L"输入收获重量有误！",L"警告");
		editWig1.Empty();
		editWig2.Empty();
		UpdateData(FALSE);
		return;
	}
	if (!editWig2.IsEmpty() && (wig2 = (float)_wtof(editWig2)) == 0)
	{
		MessageBox(L"输入收获重量有误！", L"警告");
		editWig1.Empty();
		editWig2.Empty();
		UpdateData(FALSE);
		return;
	}

	extern int plantCnt;
	if(staPla==NULL)
		staPla = new StatData[plantCnt];
	if (outFlag == 0)
	{
		dataCnt=custzStatKind(staPla,kIdx, year1, year2,
			area1, area2, wig1, wig2);	//自定义蔬菜种类信息统计,返回数据条数
	}
	else
	{
		dataCnt = custzStatVeg(staPla, vName, year1, year2,
			area1, area2, wig1, wig2);	//自定义蔬菜基本信息统计, 返回数据条数
	}
		
	if (dataCnt == 0)	//数据条数为0
	{
		listOut.DeleteAllItems();
		listOut.InsertItem(0, L"未找到！！！");
		return;
	}
	
	insertToListOut();	//将统计数据添加到列表
	((CButton*)GetDlgItem(BNT_OUT))->EnableWindow(TRUE);	//启用导出按钮
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加数据到列表
void CIoDlg2::insertToListOut()
{
	CString str;
	listOut.DeleteAllItems();	//清空列表数据
	//根据导出数据形式添加列表头 分类名称或蔬菜名称
	for (int i = 0; i < dataCnt; i++)
	{
		if (outFlag == 0)
			listOut.InsertItem(i, staPla[i].pKind->k_name);
		else
			listOut.InsertItem(i, staPla[i].pVeg->v_name);
	}

	//按照信息名称在列表中的出现次序将数据添加到列表中
	for (int i = 1; i < col; ++i)
	{
		for (int j = 0; j < CHKCNT; ++j)
		{
			if (checkFlag[j] == i + 1)
			{
				switch (j)
				{
				case 0:for (int k = 0; k < dataCnt; ++k)
				{
					listOut.SetItemText(k, i, staPla[k].pKind->k_name);
				}
					  break;
				case 1:for (int k = 0; k < dataCnt; ++k)
				{
					listOut.SetItemText(k, i, staPla[k].pVeg->v_name);
				}
					  break;
				case 2:for (int k = 0; k < dataCnt; ++k)
				{
					str.Format(L"%c", staPla[k].pKind->k_num);
					listOut.SetItemText(k, i, str);
				}
					  break;
				case 3:for (int k = 0; k < dataCnt; ++k)
				{
					listOut.SetItemText(k, i, staPla[k].pVeg->v_nutrition);
				}
					  break;
				case 4:for (int k = 0; k < dataCnt; ++k)
				{
					str.Format(L"%d", staPla[k].p_area);
					listOut.SetItemText(k, i, str);
				}
					  break;
				case 5:for (int k = 0; k < dataCnt; ++k)
				{
					str.Format(L"%.2f", staPla[k].p_wegiht);
					listOut.SetItemText(k, i, str);
				}
					  break;
				case 6:for (int k = 0; k < dataCnt; ++k)
				{
					listOut.SetItemText(k, i, staPla[k].p_year);
				}
					  break;
				}
				break;
			}
		}
	}
	
}


//////////////////////////////////////////////////////////////////////////
//函数功能：全选复选框响应
void CIoDlg2::OnBnClickedAll()
{
	// TODO: 在此添加控件通知处理程序代码
	int state = ((CButton*)GetDlgItem(CHK_ALL))->GetCheck();	//获取全选框状态
	//将所有复选框统一设定为选中/未选中
	for (int i = 0; i < CHKCNT; ++i)
	{
		CButton* pChk = (CButton*)GetDlgItem(i + CHK_KNAME);
		if (pChk->IsWindowEnabled()&&pChk->GetCheck()==!state)
			checkCheckBox(i,state);
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：点击复选框
//入口参数: idx为复选框序号, state为复选框状态(0未选中, 1选中)
//返回值: 无
void CIoDlg2::checkCheckBox(int idx,int state)
{
	((CButton*)GetDlgItem(idx+CHK_KNAME))->SetCheck(state);		//设置复选框选中状态
	OnCheckBox(idx);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：清空按钮响应
void CIoDlg2::OnBnClickedOpt()
{
	// TODO: 在此添加控件通知处理程序代码
	listOut.DeleteAllItems();
	((CButton*)GetDlgItem(BNT_OUT))->EnableWindow(FALSE);	//关闭导出按钮
	cmbKind.SetCurSel(0);
	dataCnt = 0;	//数据条数置0
	editVname.Empty();
	editYear1.Empty();
	editYear2.Empty();
	editArea1.Empty();
	editArea2.Empty();
	editWig1.Empty();
	editWig2.Empty();
	UpdateData(FALSE);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：注销窗口前数据处理
void CIoDlg2::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (staPla)		//若统计数据存在则释放
		delete[] staPla;
}


//////////////////////////////////////////////////////////////////////////
//OnEnChangeXxx函数功能：文本框中输入内容时自动将对应复选框勾选, 为空则不勾选

void CIoDlg2::OnEnChangeYear1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (!editYear1.IsEmpty())
	{
		if(((CButton*)GetDlgItem(CHK_YEAR))->GetCheck()==FALSE)
			checkCheckBox(6, TRUE);
	}
	else
	{
		if (((CButton*)GetDlgItem(CHK_YEAR))->GetCheck() == TRUE)
			checkCheckBox(6, FALSE);
	}
}


void CIoDlg2::OnEnChangeYear2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!editYear2.IsEmpty())
	{
		if (((CButton*)GetDlgItem(CHK_YEAR))->GetCheck() == FALSE)
			checkCheckBox(6, TRUE);
	}
	else
	{
		if (((CButton*)GetDlgItem(CHK_YEAR))->GetCheck() == TRUE)
			checkCheckBox(6, FALSE);
	}
}


void CIoDlg2::OnEnChangeArea1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!editArea1.IsEmpty()|| !editArea2.IsEmpty())
	{
		if (((CButton*)GetDlgItem(CHK_AREA))->GetCheck() == FALSE)
			checkCheckBox(4, TRUE);
	}
	else
	{
		if (((CButton*)GetDlgItem(CHK_AREA))->GetCheck() == TRUE)
			checkCheckBox(4, FALSE);
	}
}


void CIoDlg2::OnEnChangeArea2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	OnEnChangeArea1();
}


void CIoDlg2::OnEnChangeWig1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!editWig1.IsEmpty()||!editWig2.IsEmpty())
	{
		if (((CButton*)GetDlgItem(CHK_WIG))->GetCheck() == FALSE)
			checkCheckBox(5, TRUE);
	}
	else
	{
		if (((CButton*)GetDlgItem(CHK_WIG))->GetCheck() == TRUE)
			checkCheckBox(5, FALSE);
	}
}


void CIoDlg2::OnEnChangeWig2()
{
	OnEnChangeWig1();
}
