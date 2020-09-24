//����ĺ�˷�װ����ͷ�ļ�

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>		//���ֽ�ͷ�ļ�
#include <afxwin.h>		//MFCͷ�ļ�
#include <time.h>		//ʱ���
#include "csvIO.h"		//csv��

#define CHKCNT 7	//�Զ��嵼�������и�ѡ��ĸ���
#define DATAFILE "./data/data.dat"	//�洢���ݵ��ļ�


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ݽṹ����
//

typedef struct Kind		//�߲�����ṹ��
{
	wchar_t k_num;		//�������
	wchar_t k_name[8];		//��������
	struct Kind* k_next;	//ָ����һ�������ָ��
	struct Veg* k_vptr;		//ָ���߲���Ϣ�����ָ��
}Kind;


typedef struct Veg		//�߲˻�����Ϣ�ṹ��
{
	int v_num;		//�߲˱��
	wchar_t v_name[10];		//�߲�����
	wchar_t v_kind;		//������
	wchar_t v_nutrition[40];		//Ӫ���ɷ�
	struct Veg* v_next;		//ָ����һ��Ϣ����ָ��
	struct Plant* v_pptr;		//ָ����ֲ��Ϣ�����ָ��
}Veg;


typedef struct Plant		//�߲���ֲ��Ϣ�ṹ��
{
	int p_num;		//���
	int p_vnum;		//�߲˱��
	int p_area;		//��ֲ���
	float p_wegiht;		//�ջ�����
	wchar_t p_year[5];		//��ֲ���
	struct Plant* p_next;	//ָ����һ��ֲ��Ϣ����ָ��
}Plant;


typedef struct StatData		//ͳ�����ݽṹ��
{
	Kind* pKind;	//���ݶ�Ӧ���߲�������ָ��
	Veg* pVeg;		//���ݶ�Ӧ���߲˻�����Ϣ���ָ��
	wchar_t p_year[5];		//��ֲ���
	int p_area;		//��ֲ���
	float p_wegiht;		//�ջ�����
}StatData;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
//

void initData();	//�������ļ��м�������, ��ʼ���߲����ݽṹ
int saveData();		//�洢���ݽṹ�е���Ϣ�������ļ���
void freeData();	//�����߲����ݽṹ
Kind* checkInKind(wchar_t k_num, wchar_t* k_name, int flag);	//�������Ż�������Ʋ�ѯ���߲������Ƿ����
void addToKind(Kind *ipt);		//����߲�������Ϣ���߲����ݽṹ
void delInKind(int idx);		//ɾ��һ���߲��߲�������Ϣ���߲����ݽṹ
void modInKind(int idx,Kind* ipt);		//�޸�ĳһ�߲�������Ϣ���߲����ݽṹ��
int checkInVegByName(wchar_t* v_name);		//���߲����Ʋ�ѯ���߲˻�����Ϣ�Ƿ����
int checkInVegByNum(int vNum);		//���߲˱�Ų�ѯ���߲˻�����Ϣ�Ƿ����
int checkInPlant(int pNum);		//����ֲ��Ϣ��Ų�ѯ���߲���ֲ��Ϣ�Ƿ����
void addToVeg(Veg* ipt);		//����߲˻�����Ϣ���߲����ݽṹ
void delInVeg(int num);			//ɾ��һ���߲˻�����Ϣ���߲����ݽṹ
void modInVeg(Veg* ipt, wchar_t k_num);		//�޸�ĳһ�߲˻�����Ϣ���߲����ݽṹ��
void addToPlant(Plant* ipt);	//����߲���ֲ��Ϣ���߲����ݽṹ
void delInPlant(int num);		//ɾ��һ���߲���ֲ��Ϣ���߲����ݽṹ
void modInPlant(Plant* ipt, int v_num);		//�޸�ĳһ�߲���ֲ��Ϣ���߲����ݽṹ��
void secVegByName(wchar_t* name, CListCtrl* list_veg);		//���߲������Ӵ������߲˻�����Ϣ,����ӵ��б�
void secVegByKindNut(wchar_t kNum, wchar_t* nut, CListCtrl* list_veg);		//���������Ӫ���ɷֲ����߲˻�����Ϣ,����ӵ��б�
void secPlantByVnameYear(wchar_t* subname, wchar_t* year, CListCtrl* list_plant);	//���߲˲������ƻ���ֲ��ݲ����߲���ֲ��Ϣ,����ӵ��б�
void secPlantByVname(wchar_t* vName, CListCtrl* list_plant);	//���߲����Ʋ�ѯ�߲���ֲ��Ϣ,����ӵ��б�
void statPlantOfKind(StatData* statPla,wchar_t* year);		//ͳ��ĳ������߲���ֲ��Ϣ
int statPlantOfVegYear(StatData* statPla, wchar_t* year1, wchar_t* year2);		//����ֹ���Ϊ�������߲�����ͳ����ֲ��Ϣ
int inputFile(int flag,wchar_t* filePath, int mode);	//����csv���ݱ��
int outPlantOfKind(StatData* statPla, wchar_t* filePath);		//�����������ͳ�����ݵ�csv���
void statVegByLeastYear(StatData *statPla,wchar_t* vName);		//ͳ��ĳ�߲˽��������ֲ��Ϣ
int outPlantOfVeg(StatData* statPla, wchar_t* filePath);		//�������߲˵�ͳ�����ݵ�csv���
int custzStatKind(StatData* statPla, int kIdx, wchar_t* year1,wchar_t* year2,
int area1, int area2,float wig1, float wig2);		//�Զ��尴�߲�����ͳ����ֲ��Ϣ
int custzStatVeg(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2,
int area1, int area2, float wig1, float wig2);		//�Զ��尴�߲˻�����Ϣͳ����ֲ��Ϣ
int outCustzFile(StatData* statPla, wchar_t* filePath, int dataCnt, int colCnt, int* checkFlag);	//�Զ��嵼�����ݵ�csv���
void statVegByYear(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2);		//ÿ���߲˰����ͳ����ֲ��Ϣ



void wcssplit(wchar_t** wpp, wchar_t* src, wchar_t* delim);		//���ַ���src����delim���зָ�,�洢��wppָ���������
void qucikSortByWeight(StatData* a, int left, int right);		//������a���ջ����������������
void alertMeg(wchar_t* meg);		//������ʾ��, ��ʾ������Ϣ���˳�����
void addVegToList(CListCtrl* lis, Veg* v, int i,int flag);		//���߲˻�����Ϣ��ӵ��б��i��
void addKindToList(CListCtrl* lis, Kind* k, int i);		//���߲�������Ϣ��ӵ��б��i��
void addPlantToList(CListCtrl* lis, Plant* p, int i);		//���߲���ֲ��Ϣ��ӵ��б��i��
void initList(CListCtrl* lis, CString head[], int* width, int n);	//��ʼ���б�
void showAllKind(CListCtrl* list_kind);		//�������߲�������Ϣ��ӵ��б���ʾ
void showAllVeg(CListCtrl* list_veg,int flag);		//��ȫ���߲˻�����Ϣ��ӵ��б���ʾ
void showAllPlant(CListCtrl* list_plant);		//��ȫ���߲���ֲ��Ϣ��ӵ��б���ʾ
void showVegPlantOfKind(Kind* k, CListCtrl* list_veg, CListCtrl* list_plant);	//��ʾĳ�߲�������Ϣ�¶�Ӧ��ȫ���߲˻�����Ϣ���߲���ֲ��Ϣ
void updateKindCmb(CComboBox* cmb);		//�����߲�������Ϣ������
void updateVegCmb(CComboBox* cmb);		//�����߲˻�����Ϣ������
void updatPlantCmb(CComboBox* cmb);		//�����߲���ֲ��Ϣ������
int OnCbnChangeVeg(CComboBox* cmb, Veg* dest);	//����������ѡ��ȷ����Ӧ���߲˻�����Ϣ���
int CALLBACK CListCtrlCmp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	//���ձ��˳��
int CALLBACK PlantCmpByYear(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	//������ݻ���˳��
void addStatDataToList(CListCtrl* lis, StatData* p, int n, int flag);		//���ͳ�����ݵ��б�
void getFolderPath(CString* foderPath, HWND m_hWnd);	//��ȡ�ļ�·��