//程序的后端封装函数头文件

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>		//宽字节头文件
#include <afxwin.h>		//MFC头文件
#include <time.h>		//时间库
#include "csvIO.h"		//csv库

#define CHKCNT 7	//自定义导出窗口中复选框的个数
#define DATAFILE "./data/data.dat"	//存储数据的文件


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//数据结构声明
//

typedef struct Kind		//蔬菜种类结构体
{
	wchar_t k_num;		//分类编码
	wchar_t k_name[8];		//分类名称
	struct Kind* k_next;	//指向下一种类结点的指针
	struct Veg* k_vptr;		//指向蔬菜信息链表的指针
}Kind;


typedef struct Veg		//蔬菜基本信息结构体
{
	int v_num;		//蔬菜编号
	wchar_t v_name[10];		//蔬菜名称
	wchar_t v_kind;		//分类码
	wchar_t v_nutrition[40];		//营养成分
	struct Veg* v_next;		//指向下一信息结点的指针
	struct Plant* v_pptr;		//指向种植信息链表的指针
}Veg;


typedef struct Plant		//蔬菜种植信息结构体
{
	int p_num;		//编号
	int p_vnum;		//蔬菜编号
	int p_area;		//种植面积
	float p_wegiht;		//收获重量
	wchar_t p_year[5];		//种植年份
	struct Plant* p_next;	//指向下一种植信息结点的指针
}Plant;


typedef struct StatData		//统计数据结构体
{
	Kind* pKind;	//数据对应的蔬菜种类结点指针
	Veg* pVeg;		//数据对应的蔬菜基本信息结点指针
	wchar_t p_year[5];		//种植年份
	int p_area;		//种植面积
	float p_wegiht;		//收获重量
}StatData;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//函数声明
//

void initData();	//从数据文件中加载数据, 初始化蔬菜数据结构
int saveData();		//存储数据结构中的信息到数据文件中
void freeData();	//销毁蔬菜数据结构
Kind* checkInKind(wchar_t k_num, wchar_t* k_name, int flag);	//按分类编号或分类名称查询该蔬菜种类是否存在
void addToKind(Kind *ipt);		//添加蔬菜种类信息到蔬菜数据结构
void delInKind(int idx);		//删除一个蔬菜蔬菜种类信息从蔬菜数据结构
void modInKind(int idx,Kind* ipt);		//修改某一蔬菜种类信息在蔬菜数据结构中
int checkInVegByName(wchar_t* v_name);		//按蔬菜名称查询该蔬菜基本信息是否存在
int checkInVegByNum(int vNum);		//按蔬菜编号查询该蔬菜基本信息是否存在
int checkInPlant(int pNum);		//按种植信息编号查询该蔬菜种植信息是否存在
void addToVeg(Veg* ipt);		//添加蔬菜基本信息到蔬菜数据结构
void delInVeg(int num);			//删除一个蔬菜基本信息从蔬菜数据结构
void modInVeg(Veg* ipt, wchar_t k_num);		//修改某一蔬菜基本信息在蔬菜数据结构中
void addToPlant(Plant* ipt);	//添加蔬菜种植信息到蔬菜数据结构
void delInPlant(int num);		//删除一个蔬菜种植信息从蔬菜数据结构
void modInPlant(Plant* ipt, int v_num);		//修改某一蔬菜种植信息在蔬菜数据结构中
void secVegByName(wchar_t* name, CListCtrl* list_veg);		//按蔬菜名称子串查找蔬菜基本信息,并添加到列表
void secVegByKindNut(wchar_t kNum, wchar_t* nut, CListCtrl* list_veg);		//按分类码或营养成分查找蔬菜基本信息,并添加到列表
void secPlantByVnameYear(wchar_t* subname, wchar_t* year, CListCtrl* list_plant);	//按蔬菜部分名称或种植年份查找蔬菜种植信息,并添加到列表
void secPlantByVname(wchar_t* vName, CListCtrl* list_plant);	//按蔬菜名称查询蔬菜种植信息,并添加到列表
void statPlantOfKind(StatData* statPla,wchar_t* year);		//统计某年各类蔬菜种植信息
int statPlantOfVegYear(StatData* statPla, wchar_t* year1, wchar_t* year2);		//以起止年份为条件按蔬菜名称统计种植信息
int inputFile(int flag,wchar_t* filePath, int mode);	//导入csv数据表格
int outPlantOfKind(StatData* statPla, wchar_t* filePath);		//导出按种类的统计数据到csv表格
void statVegByLeastYear(StatData *statPla,wchar_t* vName);		//统计某蔬菜近三年的种植信息
int outPlantOfVeg(StatData* statPla, wchar_t* filePath);		//导出按蔬菜的统计数据到csv表格
int custzStatKind(StatData* statPla, int kIdx, wchar_t* year1,wchar_t* year2,
int area1, int area2,float wig1, float wig2);		//自定义按蔬菜种类统计种植信息
int custzStatVeg(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2,
int area1, int area2, float wig1, float wig2);		//自定义按蔬菜基本信息统计种植信息
int outCustzFile(StatData* statPla, wchar_t* filePath, int dataCnt, int colCnt, int* checkFlag);	//自定义导出数据到csv表格
void statVegByYear(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2);		//每种蔬菜按年份统计种植信息



void wcssplit(wchar_t** wpp, wchar_t* src, wchar_t* delim);		//将字符串src按照delim进行分割,存储到wpp指向的数组中
void qucikSortByWeight(StatData* a, int left, int right);		//对数组a按收获重量降序快速排序
void alertMeg(wchar_t* meg);		//警告提示框, 提示错误信息并退出程序
void addVegToList(CListCtrl* lis, Veg* v, int i,int flag);		//将蔬菜基本信息添加到列表第i行
void addKindToList(CListCtrl* lis, Kind* k, int i);		//将蔬菜种类信息添加到列表第i行
void addPlantToList(CListCtrl* lis, Plant* p, int i);		//将蔬菜种植信息添加到列表第i行
void initList(CListCtrl* lis, CString head[], int* width, int n);	//初始化列表
void showAllKind(CListCtrl* list_kind);		//将所有蔬菜种类信息添加到列表显示
void showAllVeg(CListCtrl* list_veg,int flag);		//将全部蔬菜基本信息添加到列表显示
void showAllPlant(CListCtrl* list_plant);		//将全部蔬菜种植信息添加到列表显示
void showVegPlantOfKind(Kind* k, CListCtrl* list_veg, CListCtrl* list_plant);	//显示某蔬菜种类信息下对应的全部蔬菜基本信息和蔬菜种植信息
void updateKindCmb(CComboBox* cmb);		//更新蔬菜种类信息下拉框
void updateVegCmb(CComboBox* cmb);		//更新蔬菜基本信息下拉框
void updatPlantCmb(CComboBox* cmb);		//更新蔬菜种植信息下拉框
int OnCbnChangeVeg(CComboBox* cmb, Veg* dest);	//根据下拉框选项确定对应的蔬菜基本信息结点
int CALLBACK CListCtrlCmp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	//按照编号顺序
int CALLBACK PlantCmpByYear(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	//按照年份或编号顺序
void addStatDataToList(CListCtrl* lis, StatData* p, int n, int flag);		//添加统计数据到列表
void getFolderPath(CString* foderPath, HWND m_hWnd);	//获取文件路径