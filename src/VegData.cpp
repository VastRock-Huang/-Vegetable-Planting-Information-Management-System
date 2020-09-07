#include "pch.h"
#include "VegData.h"

//程序的后端封装函数及变量文件



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//与蔬菜数据结构有关的变量定义
//

Kind* head = NULL;	//蔬菜十字链表数据结构表头
int kindCnt = 0,	//蔬菜种类信息数目
vegCnt = 0,		//蔬菜基本信息数目
plantCnt = 0;	//蔬菜种植信息数目
int vegIdx = 0,		//蔬菜编号最大值
plantIdx = 0;	//种植信息编号最大值
wchar_t minYear[5] = L"9999", maxYear[5] = L"0000";		//种植年份最小年/最大年



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 对蔬菜数据结构(三项十字链表)操作处理的核心功能函数
//


//////////////////////////////////////////////////////////////////////////
//函数功能：从数据文件中加载数据, 初始化蔬菜数据结构(三向十字链表)
//入口参数：无
//返回值：无
void initData()
{
	FILE* fp;
	Kind* kNew, * kEnd;	
	Veg* vNew, * vEnd = NULL;
	Plant* pNew, * pEnd = NULL;
	int dataFlag;	//数据标识:用来区分读取信息的类型

	if ((fp = fopen(DATAFILE, "rb")) == NULL)
		alertMeg(L"数据载入失败！");

	head = (Kind*)malloc(sizeof(Kind));	//创建蔬菜种类信息表头结点
	kEnd = head;
	head->k_next = NULL;
	head->k_vptr = NULL;

	while ((dataFlag=fgetc(fp))!='$')	//从数据文件中读取一个字符,判断是否是结束符$, 不是则继续加载数据
	{
		switch (dataFlag)
		{
		case 'K':	//若为蔬菜种类信息的数据
			kNew = (Kind*)malloc(sizeof(Kind));	
			if (!kNew)
				alertMeg(L"Kind Overflow!");
			fread(kNew, sizeof(Kind), 1, fp);
		
			kNew->k_next = kEnd->k_next;
			kEnd->k_next = kNew;
			kEnd = kNew;
			++kindCnt;
			
			kNew->k_vptr = (Veg*)malloc(sizeof(Veg));
			kNew->k_vptr->v_next = NULL;
			kNew->k_vptr->v_pptr = NULL;
			vEnd = kNew->k_vptr;
			break;

		case 'V':	//若为蔬菜基本信息
			vNew = (Veg*)malloc(sizeof(Veg));
			if(!vNew)
				alertMeg(L"Veg Overflow!");
			fread(vNew, sizeof(Veg), 1, fp);
			
			vNew->v_next = vEnd->v_next;
			vEnd->v_next = vNew;
			vEnd = vNew;
			++vegCnt;
			if (vegIdx < vNew->v_num)
				vegIdx = vNew->v_num;
			
			vNew->v_pptr = (Plant*)malloc(sizeof(Plant));
			vNew->v_pptr->p_next = NULL;
			pEnd = vNew->v_pptr;
			break;

		case 'P':	//若为蔬菜种植信息
			pNew = (Plant*)malloc(sizeof(Plant));
			if(!pNew)
				alertMeg(L"Plant Overflow!");
			fread(pNew, sizeof(Plant), 1, fp);
			
			pNew->p_next = pEnd->p_next;
			pEnd->p_next = pNew;
			pEnd = pNew;
			++plantCnt;	
			if (plantIdx < pNew->p_num)
				plantIdx = pNew->p_num;
			if (wcscmp(minYear, pNew->p_year) > 0)	
				wcscpy(minYear, pNew->p_year);
			if (wcscmp(maxYear, pNew->p_year) < 0)
				wcscpy(maxYear, pNew->p_year);
			break;
		}
	}
	fclose(fp);		//关闭文件指针
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按分类编号或分类名称查询该蔬菜种类是否存在
//入口参数：k_num分类编号, k_name分类名称, flag查询标识:0只检测k_num，1只检测k_name，2两者都检查
//返回值：指向查询到的蔬菜种类结点的指针, 若未查询到返回空指针
Kind* checkInKind(wchar_t k_num,wchar_t* k_name,int flag)
{	
	Kind* tmpk;
	for (tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)	//遍历蔬菜种类信息链表
	{
		if (flag == 0 && k_num == tmpk->k_num)	//若flag==0, 分类编号相同, 则找到
			return tmpk;
		else if (flag == 1 && wcscmp(k_name, tmpk->k_name) == 0)	//若flag==1, 分类名称相同,则找到
			return tmpk;
		else if (flag==2&&(k_num==tmpk->k_num||wcscmp(k_name,tmpk->k_name)==0))	//若flag==2, 分类名称和编号都相同,则找到
			return tmpk;
	}
	return NULL;	//未找到
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜名称查询该蔬菜基本信息是否存在
//入口参数：v_name蔬菜名称
//返回值：1表示查询到, 0表示未找到
int checkInVegByName(wchar_t* v_name)
{
	Kind* tmpk;
	Veg* tmpv;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//遍历每条蔬菜基本信息链表
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (wcscmp(v_name,tmpv->v_name)==0)		//若蔬菜名称相同则找到
				return 1;
	return 0;	//未找到
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜编号查询该蔬菜基本信息是否存在
//入口参数：vNum蔬菜编号
//返回值：1表示查询到, 0表示未找到
int checkInVegByNum(int vNum)
{
	Kind* tmpk;
	Veg* tmpv;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//遍历每条蔬菜基本信息链表
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (tmpv->v_num == vNum)		//若蔬菜编号相同则找到
				return 1;
	return 0;	//未找到
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按种植信息编号查询该蔬菜种植信息是否存在
//入口参数：pNum种植信息编号
//返回值：1表示查询到, 0表示未找到
int checkInPlant(int pNum)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//遍历每条蔬菜基本信息链表
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			for(tmpp=tmpv->v_pptr->p_next;tmpp;tmpp=tmpp->p_next)
				if (tmpp->p_num == pNum)		//若编号相同则找到
					return 1;
	return 0;	//未找到
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加蔬菜种类信息到蔬菜数据结构
//入口参数：ipt指向待添加的蔬菜种类信息的指针
//返回值：无
void addToKind(Kind *ipt)
{
	Kind* tmpk;
	Kind* New = (Kind*)malloc(sizeof(Kind));	//新建蔬菜种类结点
	if (!New)
		alertMeg(L"overflow!");
	*New = *ipt;	//结点内容赋值
	New->k_vptr = (Veg*)malloc(sizeof(Veg));	//为蔬菜种类新结点创建蔬菜基本信息表头结点
	New->k_vptr->v_next = NULL;		//将新结点中的指针初始化为空指针
	New->k_vptr->v_pptr = NULL;

	//按照分类编号升序将新结点插入链表
	for (tmpk = head; tmpk->k_next && New->k_num > tmpk->k_next->k_num; tmpk = tmpk->k_next);
	New->k_next = tmpk->k_next;
	tmpk->k_next = New;
	++kindCnt;	//蔬菜种类信息数目+1
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加蔬菜基本信息到蔬菜数据结构
//入口参数：ipt指向待添加的蔬菜基本信息的指针
//返回值：无
void addToVeg(Veg* ipt)
{
	Kind* tmpk;
	Veg* tmpv;

	Veg* New = (Veg*)malloc(sizeof(Veg));	//新建蔬菜基本信息结点
	if (!New)
		alertMeg(L"overflow!");
	*New = *ipt;	//结点内容赋值
	New->v_num = ++vegIdx;	//新蔬菜基本信息的蔬菜编号
	New->v_next = NULL;		//将新结点中的指针初始化为空指针
	New->v_pptr = (Plant*)malloc(sizeof(Plant));	//为蔬菜种类新结点创建蔬菜基本信息表头结点
	New->v_pptr->p_next = NULL;

	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//找到添加蔬菜所属的蔬菜种类
		if (New->v_kind == tmpk->k_num)
			break;
	for (tmpv = tmpk->k_vptr; tmpv->v_next; tmpv = tmpv->v_next);	//将新蔬菜添加到链表尾
	tmpv->v_next = New;
	++vegCnt;	//蔬菜基本信息数目+1
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加蔬菜种植信息到蔬菜数据结构
//入口参数：ipt指向待添加的蔬菜种植信息的指针
//返回值：无
void addToPlant(Plant* ipt)
{
	Kind* tmpk;
	Veg* tmpv=NULL;
	Plant* tmpp;

	Plant* New = (Plant*)malloc(sizeof(Plant));		//新建蔬菜种植信息结点
	if (!New)
		alertMeg(L"overflow!");
	*New = *ipt;	//结点内容赋值
	New->p_num = ++plantIdx;	//新蔬菜种植信息编号
	New->p_next = NULL;		//将新结点中的指针初始化为空指针

	//更新种植年份最小年/最大年
	if (wcscmp(minYear, New->p_year) > 0)
		wcscpy(minYear, New->p_year);
	if (wcscmp(maxYear, New->p_year) < 0)
		wcscpy(maxYear, New->p_year);

	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (ipt->p_vnum == tmpv->v_num)		//找到添加蔬菜种植信息所属的蔬菜
			{
				for (tmpp = tmpv->v_pptr; tmpp->p_next; tmpp = tmpp->p_next);	//将新种植信息添加到链表尾
				tmpp->p_next = New;
				++plantCnt;	//蔬菜种植信息数目+1
				return;
			}	
}


//////////////////////////////////////////////////////////////////////////
//函数功能：删除一个蔬菜蔬菜种类信息从蔬菜数据结构
//入口参数：idx待删除蔬菜种类在链表中的序号
//返回值：无
void delInKind(int idx)
{
	Kind* prek, * curk;
	Veg* nextv, *curv;
	int i;

	//遍历链表找到待删除的蔬菜种类结点
	for (i=1, prek = head, curk = prek->k_next; curk; curk = curk->k_next,++i)
	{
		if (i==idx)
			break;
		prek = curk;
	}	
	
	//删除该蔬菜种类结点的蔬菜链表
	for (curv=curk->k_vptr->v_next;curv;curv=nextv)
	{
		nextv = curv->v_next;
		delInVeg(curv->v_num);
	}

	//释放该蔬菜种类结点
	prek->k_next = curk->k_next;
	free(curk->k_vptr);
	free(curk);
	--kindCnt;	//蔬菜种类信息数目-1
}


//////////////////////////////////////////////////////////////////////////
//函数功能：删除一个蔬菜基本信息从蔬菜数据结构
//入口参数：num待删除的蔬菜基本信息的蔬菜编号
//返回值：无
void delInVeg(int num)
{
	Kind* tmpk;
	Veg* pre = NULL, * cur = NULL;
	Plant* curp, * nextp;

	//遍历十字链表找到待删除的蔬菜基本信息结点
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (pre = tmpk->k_vptr, cur = pre->v_next; cur; cur = cur->v_next)
		{
			if (cur->v_num == num)
				goto forEnd;
			pre = cur;
		}
forEnd:		//删除该蔬菜基本信息结点的蔬菜种植信息链表
	pre->v_next = cur->v_next;
	for (curp = cur->v_pptr->p_next; curp; curp = nextp)
	{
		nextp = curp->p_next;
		free(curp);
		--plantCnt;
	}
	//释放该蔬菜基本信息结点
	free(cur->v_pptr);
	free(cur);
	--vegCnt;	//蔬菜基本信息数目-1
}


//////////////////////////////////////////////////////////////////////////
//函数功能：删除一个蔬菜种植信息从蔬菜数据结构
//入口参数：num待删除蔬菜种植信息的编号
//返回值：无
void delInPlant(int num)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* curp = NULL, * prep = NULL;

	//遍历蔬菜数据结构找到待删除的蔬菜基本信息结点
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for(tmpv=tmpk->k_vptr->v_next;tmpv;tmpv=tmpv->v_next)
			for (prep = tmpv->v_pptr, curp = prep->p_next; curp; curp = curp->p_next)
			{
				if (curp->p_num == num)
					goto forEnd;
				prep = curp;
			}
forEnd:		//释放该种植信息结点
	prep->p_next = curp->p_next;
	free(curp);
	--plantCnt;	//种植信息数目-1
}


//////////////////////////////////////////////////////////////////////////
//函数功能：修改某一蔬菜基本信息在蔬菜数据结构中
//入口参数：ipt指向新修改内容的蔬菜基本信息结构体Veg的指针, k_num待修改的蔬菜基本信息原来的分类码
//返回值：无
void modInVeg(Veg* ipt, wchar_t k_num)
{
	Kind* tmpk;
	Veg* preV=NULL, * curv=NULL, *tmpv=NULL;

	//通过原分类码找到待修改的蔬菜基本信息结点curv
	for (tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
	{
		if(k_num!=tmpk->k_num)
			continue;
		for (preV = tmpk->k_vptr, curv = preV->v_next; curv; curv = curv->v_next)
		{
			if (curv->v_num == ipt->v_num)
				goto forEnd1;
			preV = curv;
		}
	}

forEnd1:	//找到结点后将修改后的内容赋值到结点中
	wcscpy(curv->v_name, ipt->v_name);
	wcscpy(curv->v_nutrition, ipt->v_nutrition);

	//若为对蔬菜的分类编号修改,则已完成修改返回
	if (k_num == ipt->v_kind)
		return;

	//若对蔬菜的分类编号进行了修改,则需将该结点插入新蔬菜种类信息下的蔬菜链表
	preV->v_next = curv->v_next;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		if (tmpk->k_num == ipt->v_kind)
		{
			for (tmpv = tmpk->k_vptr; tmpv->v_next; tmpv = tmpv->v_next)
				if (tmpv->v_next->v_num > curv->v_num)	//插入位置时按照蔬菜编号升序
					break;

			curv->v_next = tmpv->v_next;
			tmpv->v_next = curv;
			curv->v_kind = ipt->v_kind;	//修改分类编号
			return;
		}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：修改某一蔬菜种类信息在蔬菜数据结构中
//入口参数：idx>=1待修改种类结点在种类链表中的次序, ipt指向新修改内容种类结构体Kind的指针
//返回值：无
void modInKind(int idx,Kind* ipt)
{
	Kind* pre, * cur,*tmpk;
	Veg* tmpv;
	int i; 

	for (i = 1, pre = head, cur = pre->k_next; i<idx; cur = cur->k_next, ++i)	//遍历到待修改蔬菜种类结点
		pre = cur;

	wcscpy(cur->k_name, ipt->k_name);	//修改分类名称

	if(ipt->k_num==cur->k_num)	//若未修改分类编号, 则返回
	{
		return;
	}

	//若修改了分类编号
	cur->k_num = ipt->k_num;
	pre->k_next = cur->k_next;		//将该结点cur从原链表中取出

	//将分类编号修改后的结点重新按分类编号升序插入到链表中
	for (tmpk = head; tmpk->k_next && cur->k_num > tmpk->k_next->k_num; tmpk = tmpk->k_next);
	cur->k_next = tmpk->k_next;
	tmpk->k_next = cur;
	//wcscpy(cur->k_name, ipt->k_name);

	//将该结点下的蔬菜基本信息链表中该种类的分类编号进行修改
	for (tmpv = cur->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		tmpv->v_kind = cur->k_num;

}


//////////////////////////////////////////////////////////////////////////
//函数功能：修改某一蔬菜种植信息在蔬菜数据结构中
//入口参数：ipt指向新修改内容种植信息结构体Plant的指针,v_num待修改的蔬菜种植信息原来的蔬菜编号
//返回值：无
void modInPlant(Plant* ipt, int v_num)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* prep = NULL, * curp = NULL, * tmpp = NULL;

	//遍历到待修改蔬菜种植信息结点
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if (tmpv->v_num != v_num)
				continue;
			for (prep = tmpv->v_pptr, curp = prep->p_next; curp; curp = curp->p_next)
			{
				if (curp->p_num == ipt->p_num)
					goto forEnd1;
				prep = curp;
			}
		}

forEnd1:	//修改内容赋值
	curp->p_area = ipt->p_area;
	curp->p_wegiht = ipt->p_wegiht;
	wcscpy(curp->p_year, ipt->p_year);

	//若未修改蔬菜编号则返回
	if (ipt->p_vnum == v_num)
		return;

	//若修改了蔬菜编号则找到修改后的蔬菜结点
	prep->p_next = curp->p_next;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if(tmpv->v_num==ipt->p_vnum)
				for (tmpp = tmpv->v_pptr; tmpp->p_next; tmpp = tmpp->p_next)
					if (tmpp->p_next->p_num > curp->p_num)	//插入位置时按照种植信息编号升序
						goto forEnd2;
forEnd2:	
	curp->p_next = tmpp->p_next;
	tmpp->p_next = curp;
	curp->p_vnum = ipt->p_vnum;		//修改蔬菜编号
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜名称子串查找蔬菜基本信息,并添加到列表
//入口参数：subname查询的蔬菜名称子串, list_veg用于显示查询结果的MFC列表控件
//返回值：无
void secVegByName(wchar_t* subname, CListCtrl* list_veg)
{
	Kind* tmpk;
	Veg* tmpv;
	int i = 0;
	//遍历所有蔬菜链表
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (wcswcs(tmpv->v_name, subname))	//若某个蔬菜的名称中包含子串, 则将该结点的数据添加到列表
				addVegToList(list_veg, tmpv, i++, 1);

	if (i == 0)		//若未找到, 在列表添加 未找到 
		list_veg->InsertItem(0, L"未找到!!!");
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按分类码或营养成分查找蔬菜基本信息,并添加到列表
//入口参数：kNum查询的分类码, nut查询的营养成分, list_veg用于显示查询结果的MFC列表控件
//返回值：无
void secVegByKindNut(wchar_t kNum, wchar_t* nut,CListCtrl* list_veg)
{
	wchar_t* wpp[5];	//营养成分数组
	Kind* tmpk;
	Veg* tmpv;
	int i = 0;	//统计符合条件的信息数目
	int j;	//用于遍历营养成分数组
	int flag = 1;	//条件标识: 0表示不符合条件, 1表示符合条件

	wcssplit(wpp, nut, L"、");	//将营养成分nut按照"、"号分割成字符串并存到wpp中,每个字符串代表一个营养成分
	
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		if (kNum != 0 && tmpk->k_num != kNum)	//若kNum不为空字符串且当前种类结点不满足查询的分类码, 则跳过
			continue;
		//若kNum为空字符串(即未输入查询的分类码)或当前种类结点满足分类码,则遍历其蔬菜基本信息链表
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			flag = 1;
			if (nut[0] != 0)	//若查询的营养成分不为空字符串
				for(j = 0; wpp[j]; ++j)		//则判断该蔬菜结点是否同时包含查询的每种营养成分
					if (wcswcs(tmpv->v_nutrition, wpp[j]) == NULL)	//若有一直营养成分不包含,则flag置0
					{
						flag = 0;
						break;
					}
			if (flag == 0)	//若flag==0则跳过,继续判断下一蔬菜结点
				continue;
			addVegToList(list_veg, tmpv, i++, 1);	//符合条件则将该结点数据添加到列表
		}
	}
	if (i == 0)		//若未找到, 在列表添加 未找到 
		list_veg->InsertItem(0, L"未找到!!!");
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜部分名称或种植年份查找蔬菜种植信息,并添加到列表
//入口参数：subname查询的蔬菜部分名称, year查询的种植年份, list_plant用于显示查询结果的MFC列表控件
//返回值：无
void secPlantByVnameYear(wchar_t* subname, wchar_t* year, CListCtrl* list_plant)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	//遍历三向十字链表
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if(subname[0]!=0&&wcswcs(tmpv->v_name,subname)==NULL)	//若subname不为空字符串且当前蔬菜结点不包含查询的子串, 则跳过
				continue;
			//若subname为空字符串(即未输入查询的蔬菜部分名称)或当前蔬菜结点包含查询的子串,则遍历其蔬菜种植信息链表
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				if(year[0]!=0&&wcscmp(year,tmpp->p_year)!=0)	//若查询年份不为空字符串且种植信息结点的种植年份与查询不同则跳过
					continue;
				addPlantToList(list_plant, tmpp,i++);	//符合条件则将该结点数据添加到列表
			}
		}
	if (i == 0)		//未找到
		list_plant->InsertItem(0, L"未找到!!!");
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按蔬菜名称查询蔬菜种植信息,并添加到列表
//入口参数：vName查询的蔬菜名称, list_plant用于显示查询结果的MFC列表控件
//返回值：无
void secPlantByVname(wchar_t* vName, CListCtrl* list_plant)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	//遍历三向十字链表
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if (wcscmp(vName, tmpv->v_name) == 0)	//若当前蔬菜结点的蔬菜名称与待查询的蔬菜名称一致
				for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)	//则将其下所有种植信息结点添加到列表
					addPlantToList(list_plant, tmpp, i++);
		}
	if (i == 0)		//未找到
		list_plant->InsertItem(0, L"未找到!!!");

}


//////////////////////////////////////////////////////////////////////////
//函数功能：统计某年各类蔬菜种植信息
//入口参数：statPla用于存放符合条件的统计数据, year统计的种植年份
//返回值：无
void statPlantOfKind(StatData* statPla,wchar_t* year)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	//分别统计不同种类蔬菜的种植信息
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next, ++i)
	{
		statPla[i].pKind = tmpk;	//蔬菜种类
		statPla[i].p_area = 0;
		statPla[i].p_wegiht = 0;
		wcscpy(statPla[i].p_year, year);
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				if (year[0]!=0&&wcscmp(year,tmpp->p_year)!=0)	//不符合种植年份则跳过
					continue;
				//对种植面积 收获重量统计
				statPla[i].p_area += tmpp->p_area;
				statPla[i].p_wegiht += tmpp->p_wegiht;
			}
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：以起止年份为条件按蔬菜名称统计种植信息
//入口参数：statPla用于存放符合条件的统计数据, year1统计的起始年份, year2统计的终止年份
//返回值：统计数据的条数
int statPlantOfVegYear(StatData* statPla, wchar_t* year1, wchar_t* year2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	int flag;	//统计标识,0表示该蔬菜的种植信息未被统计, 1表示该蔬菜种植信息被统计了

	statPla[i].p_area = 0;
	statPla[i].p_wegiht = 0;

	//变量三向十字链表
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			flag = 0;	//重置统计标识
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				//若种植信息结点的年份不满足起始年份区间则跳过
				if (wcscmp(year1,tmpp->p_year)>0)
					continue;
				if(wcscmp(year2,tmpp->p_year)<0)
					continue;
				//满足条件的结点对数据进行统计
				statPla[i].p_area += tmpp->p_area;
				statPla[i].p_wegiht += tmpp->p_wegiht;
				flag = 1;	//设置统计标识, 该蔬菜结点的种植信息被统计了
			}
			if (flag)	//若该蔬菜结点有信息被统计, 则完善统计信息
			{
				statPla[i].pKind = tmpk;
				statPla[i].pVeg = tmpv;
				++i;
				statPla[i].p_area = 0;
				statPla[i].p_wegiht = 0;
			}
		}
	if(i>1)		//若统计数据数目大于1则按收获重量进行排序
		qucikSortByWeight(statPla, 0, i - 1);
	return i;	//返回统计数据条数
}


//////////////////////////////////////////////////////////////////////////
//函数功能：统计某蔬菜近三年的种植信息
//入口参数：statPla用于存放统计数据, VName待统计的蔬菜名称
//返回值：无
void statVegByLeastYear(StatData* statPla, wchar_t* vName)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i;

	time_t now = time(0);
	struct tm* myTime = localtime(&now);
	int year = myTime->tm_year + 1900;		//获取当前年份

	//遍历三向十字链表
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr; tmpv; tmpv = tmpv->v_next)
		{
			if (wcscmp(vName, tmpv->v_name) == 0)	//找到待统计蔬菜
			{
				for (i = 0; i < 3; ++i)		//统计近三年数据
				{
					_itow_s(year - i, statPla[i].p_year, 10);
					statPla[i].pVeg = tmpv;
					statPla[i].pKind = tmpk;
					statPla[i].p_wegiht = 0;
					statPla[i].p_area = 0;
					for (tmpp = tmpv->v_pptr; tmpp; tmpp = tmpp->p_next)
						if (wcscmp(tmpp->p_year, statPla[i].p_year) == 0)
						{
							statPla[i].p_area = tmpp->p_area;
							statPla[i].p_wegiht = tmpp->p_wegiht;
						}
				}
			}
		}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：每种蔬菜按年份统计种植信息
//入口参数：statPla用于存放统计信息, vName蔬菜名称, year1起始年份, year2终止年份
//返回值：无
void statVegByYear(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i;

	int minY = _wtoi(year1), maxY = _wtoi(year2);
	int yearCnt = maxY - minY + 1;	//计算年份
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr; tmpv; tmpv = tmpv->v_next)
		{
			if (wcscmp(vName, tmpv->v_name) == 0)	//找到该种蔬菜
			{
				for (i = 0; i < yearCnt; ++i)	//统计每种蔬菜每一年的种植信息
				{
					_itow_s(minY + i, statPla[i].p_year, 10);
					statPla[i].pVeg = tmpv;
					statPla[i].pKind = tmpk;
					statPla[i].p_wegiht = 0;
					statPla[i].p_area = 0;
					for (tmpp = tmpv->v_pptr; tmpp; tmpp = tmpp->p_next)
					{
						if (wcscmp(tmpp->p_year, statPla[i].p_year) == 0)
						{
							statPla[i].p_area = tmpp->p_area;
							statPla[i].p_wegiht = tmpp->p_wegiht;
						}
					}

				}
			}
		}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：导入Excel数据表格
//入口参数：dataType导入信息类型(1蔬菜种类信息,2蔬菜基本信息,3蔬菜种植信息), filePath导入文件路径, mode导入模式(0兼容,1严格)
//返回值：0表示文件打开失败,-1数据内容有误, 1数据导入成功
int inputFile(int dataType, wchar_t *filePath, int mode)
{	//0兼容模式，严格模式
	CsvFile csvf;
	CsvHandle csv=&csvf;
	Kind tmpk;
	Veg tmpv;
	Plant tmpp;
	int filecol;
	int col, row;
	int temp;
	const wchar_t* strp;

	filecol = csvGetColCnt((const wchar_t*)filePath);
	csvOpen(csv,filecol);	//打开数据表格及表单
	if (csvLoad(csv, filePath)==-1)
		return 0;

	switch(dataType)	//判断信息类型
	{
	case 1:		//导入蔬菜种类信息
		if (mode == 1)	//严格模式
		{
			//遍历表格数据
			for (col = 1; col<filecol; ++col)
			{
				//若待导入表格中有数据已经在三项链表(系统)中存在,则返回
				tmpk.k_num = (int)csvReadNum(csv, 0, col) + L'0';
				strp = csvReadStr(csv, 1, col);
				if (strp == NULL || checkInKind(tmpk.k_num, (wchar_t*)strp, 2)!=NULL)
					return -1;
			}
		}
		//遍历表格数据并导入
		for (col = 1; col < filecol; ++col)
		{
			tmpk.k_num = (int)csvReadNum(csv, 0, col) + L'0';
			strp = csvReadStr(csv, 1, col);
			//若兼容模式, 当前数据在系统中已存在则跳过
			if (mode == 0 && (strp==NULL || checkInKind(tmpk.k_num,(wchar_t*)strp, 2)!=NULL))
					continue;
			wcscpy(tmpk.k_name, strp);

			addToKind(&tmpk);	//将数据添加到系统中
		}
		break;

	case 2:		//导入蔬菜基本信息
		if (mode == 1)	//严格模式
		{
			//遍历表格, 有以下情况均返回
			for (row = 1; row<csv->filerow; ++row)
			{
				if (checkInVegByNum((int)csvReadNum(csv, row, 0))==1)	//若有数据编号在系统中出现过
					return -1;
				strp = csvReadStr(csv, row, 1);
				if (strp==NULL || checkInVegByName((wchar_t*)strp) == 1)	//若蔬菜名称为空或在系统中已存在
					return -1;
				tmpv.v_kind = (int)csvReadNum(csv, row, 2) + L'0';
				if (checkInKind(tmpv.v_kind, L"", 0) == NULL)	//蔬菜对应的种类不存在
					return -1;
				if (csvReadStr(csv, row, 3) == NULL)	//营养成分为空
					return -1;
			}
		}
		//遍历表格导入数据,兼容模式以上情况均跳过
		for (row = 1; row < csv->filerow; ++row)
		{
			strp = csvReadStr(csv, row, 1);
			if (mode == 0 && (strp==NULL || checkInVegByName((wchar_t*)strp)==1))
					continue;
			wcscpy(tmpv.v_name, strp);
			tmpv.v_kind = (int)csvReadNum(csv, row, 2) + L'0';
			if (mode == 0 && checkInKind(tmpv.v_kind, L"", 0) == NULL)
				continue;
			strp = csvReadStr(csv, row, 3);
			if (mode == 0 && strp == NULL)
				continue;
			wcscpy(tmpv.v_nutrition, strp);
			addToVeg(&tmpv);	//添加蔬菜信息到系统
		}
		break;

	case 3:		//导入蔬菜种植信息
		if (mode == 1)	//严格模式
		{	
			//遍历表格, 有以下情况均返回
			for (row = 1; row < csv->filerow; ++row)
			{
				if (checkInPlant((int)csvReadNum(csv, row, 0)) == 1)	//若蔬菜种植信息编号在系统中出现过
					return -1;
				temp = (int)csvReadNum(csv, row, 1);
				if (checkInVegByNum(temp) == 0)	//若种植信息对应蔬菜不存在
					return -1;
			}
		}
		//遍历表格导入数据,兼容模式以上情况均跳过
		for (row = 1; row < csv->filerow; ++row)
		{
			tmpp.p_vnum = (int)csvReadNum(csv, row, 1);
			if(mode==0&&(tmpp.p_vnum>vegCnt||tmpp.p_vnum==0))
				continue;
			tmpp.p_area = (int)csvReadNum(csv, row, 2);
			tmpp.p_wegiht = (float)csvReadNum(csv, row, 3);
			temp = (int)csvReadNum(csv, row, 4);
			_itow_s(temp, tmpp.p_year, 10);
			addToPlant(&tmpp);		//添加蔬菜种植信息到系统
		}
		break;
	}

	csvRelease(csv);	//关闭表格文件
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：导出按种类的统计数据到Excel表格
//入口参数：statPla待导出的数据, 导出文件路径
//返回值：1表示导出成功, 0表示导出失败
int outPlantOfKind(StatData* statPla,wchar_t* filePath)
{
	int i;
	CsvFile csvf;
	CsvHandle csv=&csvf;
	csvOpen(csv,4);

	//写入表头
	csvWriteStr(csv, 0, 0, L"年份");
	csvWriteStr(csv, 0, 1, L"分类名称");
	csvWriteStr(csv, 0, 2, L"种植面积（分）");
	csvWriteStr(csv, 0, 3, L"收获总重量（kg）");
	for (i = 0; i < kindCnt; ++i)	//导出数据
	{
		csvWriteNum(csv, i + 1, 0, _wtoi(statPla[i].p_year));
		csvWriteStr(csv, i + 1, 1, statPla[i].pKind->k_name);
		csvWriteNum(csv, i + 1, 2, statPla[i].p_area);
		csvWriteNum(csv, i + 1, 3, statPla[i].p_wegiht);
	}
	if (!csvSave(csv, filePath))	//保存表格
		return 0;
	csvRelease(csv);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：导出按蔬菜的统计数据到Excel表格
//入口参数：statPla待导出的数据, 导出文件路径
//返回值：1表示导出成功, 0表示导出失败
int outPlantOfVeg(StatData* statPla, wchar_t* filePath)
{
	int i;
	CsvFile csvf;
	CsvHandle csv = &csvf;
	csvOpen(csv,5);
	
	//写入表头
	csvWriteStr(csv, 0, 0, L"年份");
	csvWriteStr(csv, 0, 1, L"蔬菜名称");
	csvWriteStr(csv, 0, 2, L"分类名称");
	csvWriteStr(csv, 0, 3, L"种植面积（分）");
	csvWriteStr(csv, 0, 4, L"收获重量（kg）");
	for (i = 0; i < 3; ++i)		//导出数据
	{
		csvWriteNum(csv, i + 1, 0, _wtoi(statPla[i].p_year));
		csvWriteStr(csv, i + 1, 1, statPla[i].pVeg->v_name);
		csvWriteStr(csv, i + 1, 2, statPla[i].pKind->k_name);
		csvWriteNum(csv, i + 1, 3, statPla[i].p_area);
		csvWriteNum(csv, i + 1, 4, statPla[i].p_wegiht);
	}
	if (!csvSave(csv, filePath))	//保存表格
		return 0;
	csvRelease(csv);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：自定义按蔬菜种类统计种植信息
//入口参数：statPla用于存放统计数据,kIdx待统计蔬菜种类在种类链表的次序, year1起始年份, year2终止年份
//			area1最小种植面积, area2最大种植面积, wig1最小收获重量, wig2最大收获重量
//返回值：数据条数
int custzStatKind(StatData* statPla, int kIdx, wchar_t* year1,
	wchar_t* year2, int area1, int area2, float wig1, float wig2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	int idx;
	int yearFlag = wcscmp(year2, L"NaN");	//年份标识,不为0表示年份区间,0表示具体年份

	//遍历三向十字链表
	for (idx=0,tmpk = head->k_next; tmpk; tmpk = tmpk->k_next,++idx)
	{
		if(kIdx>=0&&idx!=kIdx)	//若种类次序不匹配则跳过,kIdx<0表示全部种类
			continue;
		for (tmpv=tmpk->k_vptr->v_next;tmpv;tmpv=tmpv->v_next)
		{
			for (tmpp=tmpv->v_pptr->p_next;tmpp;tmpp=tmpp->p_next)
			{
				if (yearFlag != 0)	//若年份区间, 不满足年份区间则跳过
				{
					if (year1[0]!=0&&wcscmp(tmpp->p_year, year1) < 0)
						continue;
					if(year2[0]!=0&&wcscmp(tmpp->p_year,year2)>0)
						continue;
				}
				else if (wcscmp(year1, tmpp->p_year) != 0)	//若具体年份, 不等于具体年份则跳过
					continue;

				//不在种植面积区间则跳过
				if (area1>0&&tmpp->p_area<area1)
					continue;
				if(area2>0&&tmpp->p_area>area2)
					continue;
				//不在收获重量区间则跳过
				if(wig1>0&&tmpp->p_wegiht<wig1)
					continue;
				if(wig2>0&&tmpp->p_wegiht>wig2)
					continue;
				//满足所有条件则统计
				statPla[i].pKind = tmpk;
				statPla[i].pVeg = tmpv;
				wcscpy(statPla[i].p_year, tmpp->p_year);
				statPla[i].p_area = tmpp->p_area;
				statPla[i].p_wegiht = tmpp->p_wegiht;
				++i;
			}
		}
		
	}
	return i;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：自定义按蔬菜基本信息统计种植信息
//入口参数：statPla用于存放统计数据,VName待统计蔬菜的名称, year1起始年份, year2终止年份
//			area1最小种植面积, area2最大种植面积, wig1最小收获重量, wig2最大收获重量
//返回值：数据条数
int custzStatVeg(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2, int area1, int area2, float wig1, float wig2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	int yearFlag = wcscmp(year2, L"NaN");	//年份标识,不为0表示年份区间,0表示具体年份
	//遍历三向十字链表
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if(vName[0]!=0&&wcscmp(vName,tmpv->v_name)!=0)	//若名称不相同则跳过
				continue;
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				if (yearFlag != 0)	//若年份区间, 不满足年份区间则跳过
				{
					if (year1[0] != 0 && wcscmp(tmpp->p_year, year1) < 0)
						continue;
					if (year2[0] != 0 && wcscmp(tmpp->p_year, year2) > 0)
						continue;
				}
				else if (wcscmp(year1, tmpp->p_year) != 0)	//若具体年份, 不等于具体年份则跳过
					continue;
				//不在种植面积区间则跳过
				if (area1 > 0 && tmpp->p_area < area1)
					continue;
				if (area2 > 0 && tmpp->p_area > area2)
					continue;
				//不在收获重量区间则跳过
				if (wig1 > 0 && tmpp->p_wegiht < wig1)
					continue;
				if (wig2 > 0 && tmpp->p_wegiht > wig2)
					continue;
				//满足所有条件则统计
				statPla[i].pKind = tmpk;
				statPla[i].pVeg = tmpv;
				wcscpy(statPla[i].p_year, tmpp->p_year);
				statPla[i].p_area = tmpp->p_area;
				statPla[i].p_wegiht = tmpp->p_wegiht;
				++i;
			}
		}

	}
	return i;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：自定义导出数据到Excel表格
//入口参数：statPla待导出的数据, filePath导出文件路径, dataCnt导出数据条数, colCnt导出数据属性的列数, checkFlag各项属性的次序
//返回值：1导出成功, 0导出失败
int outCustzFile(StatData* statPla, wchar_t* filePath, int dataCnt, int colCnt, int* checkFlag)
{
	int i, j, k;
	CsvFile csvf;
	CsvHandle csv = &csvf;
	csvOpen(csv, colCnt);

	for (i = 0; i < colCnt; ++i)
	{
		for (j = 0; j < CHKCNT; ++j)	//遍历checkFlag
		{
			if (checkFlag[j] == i + 1)	//满足列号与出现次序对应时, 按照各属性出现的次序导出数据
			{
					switch (j)	//check[j]的与属性对应关系
					{
					case 0:csvWriteStr(csv, 0, i, L"分类名称");
						for (k = 0; k < dataCnt; ++k)
							csvWriteStr(csv, k + 1, i, statPla[k].pKind->k_name);
						break;

					case 1:csvWriteStr(csv, 0, i, L"蔬菜名称");
						for (k = 0; k < dataCnt; ++k)
							csvWriteStr(csv, k + 1, i, statPla[k].pVeg->v_name);
						break;

					case 2:csvWriteStr(csv, 0, i, L"分类码");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, statPla[k].pKind->k_num - L'0');
						break;

					case 3:csvWriteStr(csv, 0, i, L"营养成分");
						for (k = 0; k < dataCnt; ++k)
							csvWriteStr(csv, k + 1, i, statPla[k].pVeg->v_nutrition);
						break;

					case 4:csvWriteStr(csv, 0, i, L"种植面积（分）");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, statPla[k].p_area);
						break;

					case 5:csvWriteStr(csv, 0, i, L"收获重量（kg）");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, statPla[k].p_wegiht);
						break;

					case 6:csvWriteStr(csv, 0, i, L"种植年份");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, _wtoi(statPla[k].p_year));
						break;
					}
					break;
			}
		}

	}
	if (!csvSave(csv, filePath))	//保存表格
		return 0;
	csvRelease(csv);
	return 1;

}


//////////////////////////////////////////////////////////////////////////
//函数功能：销毁蔬菜数据结构(三向十字链表)
//入口参数：无
//返回值：无
void freeData()
{
	Kind* curk, *nextk;
	Veg* curv, *nextv;
	Plant* curp,* nextp;
	
	//由内部链表的结点到外部链表结点逐级释放
	for (curk = head; curk; curk = nextk)
	{
		nextk = curk->k_next;
		for (curv = curk->k_vptr; curv; curv = nextv)
		{
			nextv = curv->v_next;
			for (curp = curv->v_pptr; curp; curp = nextp)
			{
				nextp = curp->p_next;
				free(curp);
			}
			free(curv);
		}
		free(curk);
	}

}


//////////////////////////////////////////////////////////////////////////
//函数功能：存储数据结构中的信息到数据文件中
//入口参数：无
//返回值：无
int saveData()
{
	FILE* fp;
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	
	if ((fp = fopen(DATAFILE, "wb")) == NULL)	//打开数据文件
	{
		MessageBox(NULL, L"保存数据失败请重试！", L"错误", MB_OK);
		return 0;
	}

	//遍历三向十字链表并将数据写入数据文件
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		fputc('K', fp);		//蔬菜种类信息前加标识符'K'
		fwrite(tmpk, sizeof(Kind), 1, fp);
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			fputc('V', fp);		//蔬菜基本信息前加标识符'V'
			fwrite(tmpv, sizeof(Veg), 1, fp);
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				fputc('P', fp);		//蔬菜种植信息前加标识符'P'
				fwrite(tmpp, sizeof(Plant), 1, fp);
			}
		}
	}
	putc('$', fp);	//存储数据完毕后加结束符'$'
	fclose(fp);
	return 1;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	其他数据操作封装函数
//


//////////////////////////////////////////////////////////////////////////
//函数功能：将字符串src按照delim进行分割,存储到wpp指向的数组中
//入口参数：wpp用于指向存放分割后字符串数组的二级指针, src源字符串, delim分割符
//返回值：无
void wcssplit(wchar_t** wpp, wchar_t* src, wchar_t* delim)
{
	wchar_t* buffer;	//buffer用于暂存源字符串
	int i = 0;
	//将源字符串按delim分割,将第一个分割出的存入数组中(若delim在源字符串不存在,也会将整个字符串放入wpp[i]中)
	wpp[i] = wcstok(src, delim, &buffer);
	while (wpp[i])	//若返回值不为空字符串
	{
		i++;
		wpp[i] = wcstok(NULL, delim, &buffer);	//对源字符串src剩余部分继续分割
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：对数组a按收获重量降序快速排序
//入口参数：a指向待排序数组的指针, left待排序部分索引最小元素, right待排序部分索引最大元素
//返回值：无
void qucikSortByWeight(StatData* a, int left, int right)
{
	int i = left, j = right;
	StatData key = a[i];
	if (left >= right)
		return;
	while (i < j)
	{
		while (i < j && a[j].p_wegiht <= key.p_wegiht)
			--j;
		a[i] = a[j];
		while (i < j && a[i].p_wegiht >= key.p_wegiht)
			++i;
		a[j] = a[i];
	}
	a[i] = key;
	qucikSortByWeight(a, left, i - 1);
	qucikSortByWeight(a, i + 1, right);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//界面端封装函数,
// *由于界面使用mfc框架,少量使用对象特性,基本为C语言语法
//


//////////////////////////////////////////////////////////////////////////
//函数功能：警告提示框, 提示错误信息并退出程序
//入口参数：meg错误信息
//返回值：无
void alertMeg(wchar_t* meg)
{
	MessageBox(NULL, meg, L"错误", MB_OK);
	exit(1);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：初始化列表
//入口参数：lis列表, head列表表头的字符串数组, width列表每一列的宽度, n列表列数
//返回值：无
void initList(CListCtrl* lis, CString head[], int* width, int n)
{
	int i;
	//设置列表扩展风格
	lis->SetExtendedStyle(lis->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加表头
	for (i = 0; i < n; i++)
		lis->InsertColumn(i, head[i], LVCFMT_LEFT, width[i]);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：将蔬菜基本信息添加到列表第i行
//入口参数：lis列表, v待添加的蔬菜基本信息, i待添加的行, flag显示标识: 1则显示分类码,0不显示
//返回值：无
void addVegToList(CListCtrl* lis, Veg* v, int i, int flag)
{
	CString str;
	int col = 0;
	//添加数据
	str.Format(L"%d", v->v_num);
	lis->InsertItem(i, str);
	lis->SetItemText(i, ++col, v->v_name);
	if (flag)
	{
		str.Format(L"%c", v->v_kind);
		lis->SetItemText(i, ++col, str);
	}
	lis->SetItemText(i, ++col, v->v_nutrition);
	lis->SetItemData(i, v->v_num);	//为列表该行数据设置ID号为蔬菜编号
}


//////////////////////////////////////////////////////////////////////////
//函数功能：将蔬菜种类信息添加到列表第i行
//入口参数：lis列表, k待添加的蔬菜种类信息, i待添加的行
//返回值：无
void addKindToList(CListCtrl* lis, Kind* k, int i)
{
	CString str;
	//添加数据
	str.Format(L"%c", k->k_num);
	lis->InsertItem(i, str);
	lis->SetItemText(i, 1, k->k_name);	//为列表该行数据设置ID号为分类码
}


//////////////////////////////////////////////////////////////////////////
//函数功能：将蔬菜种植信息添加到列表第i行
//入口参数：lis列表, p待添加的蔬菜种植信息, i待添加的行
//返回值：无
void addPlantToList(CListCtrl* lis, Plant* p, int i)
{
	CString str;
	int tmp = p->p_num * 100 + _wtoi(p->p_year) % 100;	//种植信息编号和年份组成一个标识
	//添加数据
	str.Format(L"%d", p->p_num);
	lis->InsertItem(i, str);
	str.Format(L"%d", p->p_vnum);
	lis->SetItemText(i, 1, str);
	str.Format(L"%d", p->p_area);
	lis->SetItemText(i, 2, str);
	str.Format(L"%.2f", p->p_wegiht);
	lis->SetItemText(i, 3, str);
	lis->SetItemText(i, 4, p->p_year);
	lis->SetItemData(i, tmp);	//为列表该行数据设置ID号为tmp
}


//////////////////////////////////////////////////////////////////////////
//函数功能：添加统计数据到列表
//入口参数：lis列表, p指向统计数据的指针, n数据条数, flag信息类型标识:0种类信息，1蔬菜信息，2年份种类信息
//返回值：无
void addStatDataToList(CListCtrl* lis, StatData* p, int n, int flag)
{
	CString str;
	int col;
	lis->DeleteAllItems();	//清空列表
	for (int i = 0; i < n; ++i)		//按照信息类型将数据添加到列表
	{
		col = 0;
		if (flag == 1)
		{
			lis->InsertItem(i, p[i].pVeg->v_name);
			lis->SetItemText(i, ++col, p[i].pKind->k_name);
		}
		else if (flag == 0)
		{
			lis->InsertItem(i, p[i].pKind->k_name);

		}
		else
		{
			lis->InsertItem(i, p[i].p_year);
			if (flag == 3)
			{
				lis->SetItemText(i, ++col, p[i].pVeg->v_name);
			}
			lis->SetItemText(i, ++col, p[i].pKind->k_name);
		}
		str.Format(L"%d", p[i].p_area);
		lis->SetItemText(i, ++col, str);
		str.Format(L"%.2f", p[i].p_wegiht);
		lis->SetItemText(i, ++col, str);
	}
}


//////////////////////////////////////////////////////////////////////////
//函数功能：将所有蔬菜种类信息添加到列表显示
//入口参数：lis_kind列表
//返回值：无
void showAllKind(CListCtrl* list_kind)
{
	
	Kind* tmpk;

	list_kind->DeleteAllItems();
	int i = 0;
	//遍历蔬菜种类信息链表并添加到列表
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next, ++i)
		addKindToList(list_kind, tmpk, i);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：将全部蔬菜基本信息添加到列表显示
//入口参数：lis_veg列表, flag:显示标识: 1则显示分类码,0不显示
//返回值：无
void showAllVeg(CListCtrl* list_veg, int flag)
{
	
	Kind* tmpk;
	Veg* tmpv;
	int i = 0;

	list_veg->DeleteAllItems();	//删除蔬菜基本信息列表中原有项
	//将蔬菜数据结构中所有的蔬菜基本信息加入到lis中
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			addVegToList(list_veg, tmpv, i++, flag);

	list_veg->SortItems(CListCtrlCmp, (DWORD_PTR)list_veg);	//对列表每行数据按照蔬菜编号排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：将全部蔬菜种植信息添加到列表显示
//入口参数：list_plant列表
//返回值：无
void showAllPlant(CListCtrl* list_plant)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	
	int i = 0;

	list_plant->DeleteAllItems();	//删除列表原有项
	//将蔬菜数据结构中所有的蔬菜种植信息加入到lis中
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				addPlantToList(list_plant, tmpp, i++);
			}
		}
	}
	list_plant->SortItems(CListCtrlCmp, (DWORD_PTR)list_plant);//对列表每行数据按照种植信息编号排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：显示某蔬菜种类信息下对应的全部蔬菜基本信息和蔬菜种植信息
//入口参数：k蔬菜种类信息, list_veg蔬菜基本信息列表, list_plant蔬菜种植信息列表
//返回值：
void showVegPlantOfKind(Kind* k, CListCtrl* list_veg, CListCtrl* list_plant)
{
	Veg* tmpv = k->k_vptr->v_next;
	Plant* tmpp;
	list_veg->DeleteAllItems();
	list_plant->DeleteAllItems();
	for (int i = 0, j = 0; tmpv; tmpv = tmpv->v_next, ++i)	//遍历蔬菜基本信息链表
	{
		addVegToList(list_veg, tmpv, i, 0);		//将蔬菜基本信息添加到列表
		for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			addPlantToList(list_plant, tmpp, j++);		//将种植信息添加到列表
	}
	list_plant->SortItems(CListCtrlCmp, (DWORD_PTR)list_plant);	//对种植信息列表每行数据按照种植信息编号排序
}


//////////////////////////////////////////////////////////////////////////
//函数功能：更新蔬菜种类信息下拉框
//入口参数：cmb蔬菜种类信息下拉框
//返回值：无
void updateKindCmb(CComboBox* cmb)
{
	cmb->ResetContent();	//清空下拉框
	
	Kind* tmpk;
	CString str;
	//遍历蔬菜种类链表并添加到下拉框
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		str.Format(L"%c  %s", tmpk->k_num, tmpk->k_name);
		cmb->AddString(str);
	}
	if (cmb->GetCount())
		cmb->SetCurSel(0);	//设置默认选项
}


//////////////////////////////////////////////////////////////////////////
//函数功能：更新蔬菜基本信息下拉框
//入口参数：cmb蔬菜基本信息下拉框
//返回值：无
void updateVegCmb(CComboBox* cmb)
{
	cmb->ResetContent();		//清空下拉框
	
	Kind* tmpk;
	Veg* tmpv;
	CString str;
	int cmbCnt = cmb->GetCount();	//下拉框中选项数
	int i;
	//遍历蔬菜基本信息链表并添加到下拉框
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next, ++cmbCnt)
		{
			//添加时按照蔬菜编号升序添加
			for (i = 0; i < cmbCnt; ++i)
			{
				cmb->GetLBText(i, str);
				if (tmpv->v_num < _wtoi(str))
				{
					str.Format(L"%d", tmpv->v_num);
					cmb->InsertString(i, str);
					break;
				}
			}
			if (i == cmbCnt)
			{
				str.Format(L"%d", tmpv->v_num);
				cmb->AddString(str);
			}
		}
	if (cmbCnt)
		cmb->SetCurSel(0);	//设置默认选项
}


//////////////////////////////////////////////////////////////////////////
//函数功能：更新蔬菜种植信息下拉框
//入口参数：cmb蔬菜种植信息下拉框
//返回值：无
void updatPlantCmb(CComboBox* cmb)
{
	cmb->ResetContent();		//清空下拉框
	
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	CString str;
	int cmbCnt = cmb->GetCount();	//下拉框中选项数
	int i;
	//遍历所有蔬菜种植信息链表并添加到下拉框
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next, ++cmbCnt)
			{
				//添加时按照种植信息编号升序添加
				for (i = 0; i < cmbCnt; ++i)
				{
					cmb->GetLBText(i, str);
					if (tmpp->p_num < _wtoi(str))
					{
						str.Format(L"%d", tmpp->p_num);
						cmb->InsertString(i, str);
						break;
					}
				}
				if (i == cmbCnt)
				{
					str.Format(L"%d", tmpp->p_num);
					cmb->AddString(str);
				}

			}
	if (cmbCnt)
		cmb->SetCurSel(0);	//设置默认选项
}


//////////////////////////////////////////////////////////////////////////
//函数功能：根据下拉框选项确定对应的蔬菜基本信息结点
//入口参数：cmb下拉框, dest对应蔬菜基本信息的指针
//返回值：该蔬菜基本信息的分类码在种类链表的次序, -1表示未找到
int OnCbnChangeVeg(CComboBox* cmb, Veg* dest)
{
	int idx;
	CString str;
	if (cmb->GetCurSel() == CB_ERR)	//若下拉框为空则返回
		return -1;
	cmb->GetLBText(cmb->GetCurSel(), str);	//获取下拉框当前选项的值
	int num = _wtoi(str);
	
	Kind* tmpk;
	Veg* tmpv;
	//遍历三向十字链表找到对应的蔬菜基本信息结点
	for (idx = 0, tmpk = head->k_next; tmpk; tmpk = tmpk->k_next, ++idx)
	{
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if (tmpv->v_num == num)
			{
				*dest = *tmpv;
				return idx;
			}
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按照编号顺序,用于列表排序的比较函数,仅用作参数
//入口参数：lParam1,lParam2待比较的两个数, lParamSort排序标识
//返回值：比较大小的结果
int CALLBACK CListCtrlCmp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return lParam1 > lParam2;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：按照年份或编号顺序, 用于列表排序的比较函数,仅用作参数 
//入口参数：lParam1,lParam2待比较的两个数, lParamSort排序标识
//返回值：比较大小的结果
int CALLBACK PlantCmpByYear(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (lParam1 % 100 == lParam2 % 100)
		return lParam1 > lParam2;
	return lParam1 % 100 > lParam2 % 100;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：获取文件路径
//入口参数：foderPath用于存放文件路径, m_hWnd窗口句柄
//返回值：无
void getFolderPath(CString* foderPath, HWND m_hWnd)
{
	wchar_t szPath[MAX_PATH] = { 0 };     //临时存放选择的目录路径 
	CString str;

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = L"请选择数据文件导出目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
		*foderPath = szPath;
	CoTaskMemFree(lp);
}