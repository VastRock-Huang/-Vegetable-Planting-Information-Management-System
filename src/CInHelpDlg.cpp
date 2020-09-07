// CInHelpDlg.cpp: 实现文件
//数据导入帮助窗口

#include "pch.h"
#include "VegInfoSystem.h"
#include "CInHelpDlg.h"
#include "afxdialogex.h"


// CInHelpDlg 对话框

IMPLEMENT_DYNAMIC(CInHelpDlg, CDialogEx)

CInHelpDlg::CInHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIG_INHELP, pParent)
{

}

CInHelpDlg::~CInHelpDlg()
{
}

void CInHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInHelpDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CInHelpDlg 消息处理程序

//////////////////////////////////////////////////////////////////////////
//函数功能：绘制帮助文字信息
void CInHelpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CDC* pDc = GetDC();
	POINT pot;
	pot.x = pot.y = 720;
	
	pDc->SetBkMode(TRANSPARENT);

	CString str;
	CFont new_font;

	//创建字体宋体格式  100为字高
	VERIFY(new_font.CreatePointFont(100, _T("宋体"), pDc));

	//选择该字体进入PDC
	CFont* default_font = pDc->SelectObject(&new_font);


	str = L"数据导入帮助";
	pDc->DrawText(str, CRect({ 0,10 },pot), DT_CENTER);

	str = L"一.导入文件类型： \r\n\
1.格式要求：\r\n蔬菜种类信息文件：\r\n\
    | 分类编号 | 1   | 2   |\r\n    | 分类名称 | XXX | XXX |\r\n\
蔬菜基本信息文件：\r\n\
    | 蔬菜编号 | 蔬菜名称 | 分类码 | 营养成分|\r\n\
蔬菜种植信息文件：\r\n\
    | 编号 | 蔬菜编号 | 种植面积 | 收获重量 | 种植年份 |\r\n\
2.导入文件类型必须选择正确，否则导入数据会出现严重错误！！！\r\n\
3.蔬菜编号和种植编号会自动忽略，系统会自动赋值";
	pDc->DrawText(str, CRect({ 60,40 },pot), DT_LEFT| DT_WORDBREAK);

	str = L"二.导入模式：\r\n\
1.兼容模式：对导入文件的非预期数据进行自动略过不导入，导入合法数据（默认模式）。\r\n\
2.严格模式：若导入文件中存在非预期数据，则不进行数据导入。\r\n\
非预期数据包括：\r\n\
    （1）重复蔬菜种类信息（包括分类编号重复和分类名称重复）\r\n\
    （2）重复蔬菜基本信息，即重复蔬菜名称\r\n\
    （3）蔬菜基本信息中分类编号不存在\r\n\
    （4）蔬菜种植信息中蔬菜编号不存在\r\n\
    （5）蔬菜编号和种植信息编号已存在（兼容模式以系统数据顺序对信息重新编号，严格模式则不进行数据导入）\r\n\
    （6）导入文件中某文字的表格为空或内容为数字";
	pDc->DrawText(str, CRect({ 60,260 }, pot), DT_LEFT | DT_WORDBREAK);

	str = L"三.其他说明：\r\n\
1.导入文件中不允许有空行或空列，否则会导致导入数据不完整\r\n\
2.导入文件中某数字的表格为空或内容为文字，读取时会以0代替";
	pDc->DrawText(str, CRect({ 60,480 }, pot), DT_LEFT | DT_WORDBREAK);

}
