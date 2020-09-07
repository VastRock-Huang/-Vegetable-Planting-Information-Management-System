//////////////////////////////////////////////////////////////////////////
//CSV�ļ���д��
//����˼��: ��csv�ļ�����ÿ���ֶ���ȡ����һ����ά�ɱ䳤������,������ÿ��Ԫ��Ϊһ�����ֽ��ַ���
//����������ͬһ�ű��.��дcsv�ļ�����ʱ�������ɶ�ά����, ��csv�ļ�ʱ�����ݼ��ص�����, д�ļ�ʱ
//����������д���ļ�, ���������ͼ����Excel���, ��������ĳ��ĳ��Ԫ�ؽ��в���.
//	*������libxl����ִ���������, ���ڼ��滻libxl������Ӷ�ʵ����Excel��csv�ļ��Ĵ���, ���޼򵥵�
//csv�ļ�����, �����ļ��������������ı�","��δ������; Ҳ���������ŵĶ�дcsv�ļ��ķ���
//by hhy

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>		//���ֽڿ�
#include <locale.h>		//���Ա��ػ���

#define LINEINC 10		//ÿ�η���csv��ά������������
#define BLOCKSIZE 40	//csv��ά������ÿ����Ԫ�ɴ�ŵĿ��ַ���
#define FILEEND 65535	//�ļ�������,������EOF,���ֽ�ʱ��δ����
#define CSVOK  1		//����ִ�гɹ�
#define CSVERROR -1		//����ִ��ʧ��

typedef struct CsvFile	//csv�ļ��ṹ��
{
	FILE* fp;			//�ļ�ָ��
	wchar_t*** data;	//csv��ά����ָ��(��ά�ַ�����)
	int filecol;		//�ļ�����
	int filerow;		//�ļ�����
	int linesize;		//�ļ�������
	int blocksize;		//��Ԫ������
}CsvFile;

typedef CsvFile* CsvHandle;

int csvOpen(CsvHandle csv, int col);		//��ʼ��csv�ṹ
int csvGetColCnt(const wchar_t* filepath);	//��ȡcsv�ļ�����
int csvLoad(CsvHandle csv,const wchar_t* filepath);		//����csv�ļ�
int csvExpan(CsvHandle csv);	//����csv����
double csvReadNum(CsvHandle csv, int row, int col);		//��ȡ����
wchar_t* csvReadStr(CsvHandle csv, int row, int col);	//��ȡ�ַ���
int csvWriteNum(CsvHandle csv, int row, int col, double num);	//д������
int csvWriteStr(CsvHandle csv, int row, int col, const wchar_t* str);	//д���ַ���
int csvSave(CsvHandle csv, const wchar_t* filepath);	//�����ļ�
void csvRelease(CsvHandle csv);		//�ͷ�csv�ṹ