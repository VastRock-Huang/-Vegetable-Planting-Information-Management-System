#include "pch.h"
#include "csvIO.h"


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ʼ��csv�ṹ
//��ڲ�����csvָ��CSVFile�ṹ���ָ��,col�ļ�����
//����ֵ��CSVOK����ִ�гɹ�,CSVERROR����ִ��ʧ��
int csvOpen(CsvHandle csv, int col)
{
	int i,j;
	setlocale(LC_ALL, "chs");	//��������
	//����csv��ʼLINEINC��col�еĶ�ά����(ÿ��Ԫ��Ϊһ��BLCOKSIZE��С���ַ���),ʵ��Ϊ��ά����
	csv->data = (wchar_t***)malloc(sizeof(wchar_t**) * LINEINC);
	if (csv->data == NULL)
		return CSVERROR;
	for (i = 0; i < LINEINC; ++i)
	{
		csv->data[i] = (wchar_t**)malloc(sizeof(wchar_t*) * col);
		if (csv->data[i] == NULL)
			return CSVERROR;
		for (j = 0; j < col; j++)
		{
			csv->data[i][j] = (wchar_t*)calloc(1,sizeof(wchar_t) * BLOCKSIZE);
			if (csv->data[i][j] == NULL)
				return CSVERROR;
		}
	}
	//��ʼ��csv�ṹ��������Ա
	csv->fp = NULL;
	csv->filecol = col;
	csv->linesize = LINEINC;
	csv->blocksize = BLOCKSIZE;
	csv->filerow = 0;
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ȡcsv(���)����
//��ڲ�����filepath csv�ļ�·��
//����ֵ������, CSVERROR��ʧ��
int csvGetColCnt(const wchar_t* filepath)
{
	FILE* fp;
	int colcnt = 1 ,i;
	wchar_t tmp;
	if ((fp = _wfopen(filepath, L"r")) == NULL)		//���ļ�
		return CSVERROR;
	//�Ի��з����ļ�������Ϊ��ֹ����csvÿ�еķָ���","����, ������","��+1��Ϊ�������� 
	do 
	{
		i = 1;
		while ((tmp=fgetwc(fp))!=L'\n'&&tmp!=FILEEND)
		{
			if (tmp == L',')
				i++;
		}
		if (i > colcnt)
			colcnt = i;
		if (tmp == FILEEND)
			break;
	} while (1);
	fclose(fp);
	return colcnt;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����csv�ļ�
//��ڲ�����csvָ��CSVFile�ṹ���ָ��, filepath�ļ�·��
//����ֵ��CSVOK����ִ�гɹ�,CSVERROR����ִ��ʧ��
int csvLoad(CsvHandle csv,const wchar_t* filepath)
{
	int i, j, k;
	wchar_t tmpwc;
	int readflag = 1;	//����ʶ: 0��ʾ������ȡ
	int lineflag;	//�б�ʶ: ���ڿ��н����޳�, 0Ϊ����

	//���ļ�
	if (csv->fp)
		fclose(csv->fp);
	if ((csv->fp = _wfopen(filepath, L"rt")) == NULL)
			return CSVERROR;

	i = 0;	//��¼����
	do	//���ж�ȡ�ļ�
	{
		if (i == csv->linesize)		//����ȡ��������csv�����������csv��������
			if (csvExpan(csv) == CSVERROR)
				return CSVERROR;
		//���ж�ȡ
		for (j = lineflag = 0; j < csv->filecol; j++)
		{
			k = 0;
			//��ȡÿ���ַ�,���� ���� ���з� �ļ������� ��ֹ
			while ((tmpwc = fgetwc(csv->fp)) != L',' && tmpwc != L'\n' && tmpwc != FILEEND)
			{
				if (k >= BLOCKSIZE - 1)		//��ȡ�ַ�������csv����ɴ�����
					return CSVERROR;
				csv->data[i][j][k++] = tmpwc;
				lineflag = 1;	//���������ʾ���в�Ϊ����
			}
			csv->data[i][j][k] = L'\0';
			if (tmpwc == FILEEND)	//�����ļ���βֹͣ��ȡ
			{
				readflag = 0;
				break;
			}
		}
		if(lineflag)	//����+1
			i++;
	} while (readflag);

	csv->filerow = i;	//��¼�ļ�����
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�csv��������(�ڲ�����)
//��ڲ�����csvָ��CSVFile�ṹ���ָ��
//����ֵ��CSVOK����ִ�гɹ�,CSVERROR����ִ��ʧ��
int csvExpan(CsvHandle csv)
{
	int i, j;
	//����LINEINC������
	csv->data = (wchar_t***)realloc(csv->data, sizeof(wchar_t**) * (csv->linesize + LINEINC));
	if (csv->data == NULL)
		return CSVERROR;
	for (i = csv->linesize; i < csv->linesize + LINEINC; i++)
	{
		csv->data[i] = (wchar_t**)malloc(sizeof(wchar_t*) * csv->filecol);
		if (csv->data[i] == NULL)
			return CSVERROR;
		for (j = 0; j < csv->filecol; j++)
		{
			csv->data[i][j] = (wchar_t*)calloc(1,sizeof(wchar_t) * BLOCKSIZE);
			if (csv->data[i][j] == NULL)
				return CSVERROR;
		}
	}
	csv->linesize += LINEINC;	//������������
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ȡ����
//��ڲ�����csvָ��CSVFile�ṹ���ָ��,row�к�,col�к�
//����ֵ����ǰ����Ԫ�ص���ֵ
double csvReadNum(CsvHandle csv, int row, int col)
{
	return _wtof(csv->data[row][col]);
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ȡ�ַ���
//��ڲ�����csvָ��CSVFile�ṹ���ָ��,row�к�,col�к�
//����ֵ��ָ����ַ�����ָ��
wchar_t* csvReadStr(CsvHandle csv, int row, int col)
{
	if (csv->data[row][col][0] == L'\0')
		return NULL;
	return csv->data[row][col];
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�д������
//��ڲ�����csvָ��CSVFile�ṹ���ָ��,row�к�, col�к�, num��д����ֵ
//����ֵ��CSVOK����ִ�гɹ�,CSVERROR����ִ��ʧ��
int csvWriteNum(CsvHandle csv, int row, int col, double num)
{
	while (row >= csv->linesize)	//�����к�����
		if(csvExpan(csv)==CSVERROR)
			return CSVERROR;
	swprintf(csv->data[row][col],BLOCKSIZE, L"%lg", num);	//������ת��Ϊ�ַ�����������
	if (row + 1 > csv->filerow)		//��������
		csv->filerow = row + 1;
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�д���ַ���
//��ڲ�����csvָ��CSVFile�ṹ���ָ��,row�к�,col�к�,strָ��д���ַ�����ָ��
//����ֵ��CSVOK����ִ�гɹ�,CSVERROR����ִ��ʧ��
int  csvWriteStr(CsvHandle csv, int row, int col, const wchar_t* str)
{
	while (row>=csv->linesize)	//�����к�����
		if (csvExpan(csv) == CSVERROR)
			return CSVERROR;
	wcscpy(csv->data[row][col], str);	//���ַ�����������
	if (row + 1 > csv->filerow)		//��������
		csv->filerow = row + 1;
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����csv�ļ�
//��ڲ�����csvָ��CSVFile�ṹ���ָ��,filepath�ļ�·��
//����ֵ��CSVOK����ִ�гɹ�,CSVERROR����ִ��ʧ��
int csvSave(CsvHandle csv, const wchar_t* filepath)
{	
	int i, j, k;
	//���ļ�
	if (csv->fp)
		fclose(csv->fp);
	if ((csv->fp = _wfopen(filepath, L"wt")) == NULL)
		return CSVERROR;
	//д��
	for (i = 0; i < csv->filerow; i++)	//����
	{
		for (j = 0; j < csv->filecol - 1; j++)	//����
		{
			k = 0;
			while (csv->data[i][j][k] != L'\0')
				fputwc(csv->data[i][j][k++], csv->fp);
			fputwc(L',', csv->fp);
		}
		k = 0;	//���һ��Ԫ�ص�������
		while (csv->data[i][j][k] != L'\0')
			fputwc(csv->data[i][j][k++], csv->fp);
		if(i<csv->filerow-1)	//�ļ���β���ӻ��з�
			fputwc(L'\n', csv->fp);
	}
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��ͷ�csv�ṹ
//��ڲ�����csvָ��CSVFile�ṹ���ָ��
//����ֵ����
void csvRelease(CsvHandle csv)
{
	int i, j;
	//�ͷ������ڴ�
	for (i = 0; i < csv->linesize; i++)
	{
		for (j = 0; j < csv->filecol; j++)
			free(csv->data[i][j]);
		free(csv->data[i]);
	}
	free(csv->data);
	//�ر��ļ�ָ��
	if (csv->fp)	
		fclose(csv->fp);
	csv->fp = NULL;
}


