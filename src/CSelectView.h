#pragma once


// CSelectView 视图, 即左侧树状选择控件

class CSelectView : public CTreeView
{
	DECLARE_DYNCREATE(CSelectView)

protected:
	CSelectView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSelectView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	CTreeCtrl* m_treeCtrl;	//左侧树控件
	CString m_txtList[16] = { L"数据内容",
		L"数据维护",L"蔬菜种类信息维护",L"蔬菜基本信息维护",L"蔬菜种植信息维护"
		,L"数据查询",L"数据统计" ,L"数据导入导出",L"基本数据导入导出" ,L"自定义查询导出" 
	};		//控件标题
	CImageList m_imgList;	//图标列表
	HTREEITEM treeMaintain, treeSearch, treeStat, treeIO;	//父结点

public:
	virtual void OnInitialUpdate();
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);

};


