#include "pch.h"
#include "csvIO.h"


//////////////////////////////////////////////////////////////////////////
//函数功能：初始化csv结构
//入口参数：csv指向CSVFile结构体的指针,col文件列数
//返回值：CSVOK函数执行成功,CSVERROR函数执行失败
int csvOpen(CsvHandle csv, int col)
{
	int i,j;
	setlocale(LC_ALL, "chs");	//设置中文
	//构建csv初始LINEINC行col列的二维数组(每个元素为一个BLCOKSIZE大小的字符串),实际为三维数组
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
	//初始化csv结构体其他成员
	csv->fp = NULL;
	csv->filecol = col;
	csv->linesize = LINEINC;
	csv->blocksize = BLOCKSIZE;
	csv->filerow = 0;
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：获取csv(最大)列数
//入口参数：filepath csv文件路径
//返回值：列数, CSVERROR则失败
int csvGetColCnt(const wchar_t* filepath)
{
	FILE* fp;
	int colcnt = 1 ,i;
	wchar_t tmp;
	if ((fp = _wfopen(filepath, L"r")) == NULL)		//打开文件
		return CSVERROR;
	//以换行符或文件结束符为终止计算csv每行的分隔符","个数, 以最多的","数+1作为列数返回 
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
//函数功能：加载csv文件
//入口参数：csv指向CSVFile结构体的指针, filepath文件路径
//返回值：CSVOK函数执行成功,CSVERROR函数执行失败
int csvLoad(CsvHandle csv,const wchar_t* filepath)
{
	int i, j, k;
	wchar_t tmpwc;
	int readflag = 1;	//读标识: 0表示结束读取
	int lineflag;	//行标识: 对于空行进行剔除, 0为空行

	//打开文件
	if (csv->fp)
		fclose(csv->fp);
	if ((csv->fp = _wfopen(filepath, L"rt")) == NULL)
			return CSVERROR;

	i = 0;	//记录行数
	do	//按行读取文件
	{
		if (i == csv->linesize)		//若读取行数超过csv数组行数则对csv数组扩容
			if (csvExpan(csv) == CSVERROR)
				return CSVERROR;
		//按列读取
		for (j = lineflag = 0; j < csv->filecol; j++)
		{
			k = 0;
			//读取每个字符,遇到 逗号 换行符 文件结束符 终止
			while ((tmpwc = fgetwc(csv->fp)) != L',' && tmpwc != L'\n' && tmpwc != FILEEND)
			{
				if (k >= BLOCKSIZE - 1)		//读取字符数超过csv数组可存容量
					return CSVERROR;
				csv->data[i][j][k++] = tmpwc;
				lineflag = 1;	//有数据则表示该行不为空行
			}
			csv->data[i][j][k] = L'\0';
			if (tmpwc == FILEEND)	//遇到文件结尾停止读取
			{
				readflag = 0;
				break;
			}
		}
		if(lineflag)	//行数+1
			i++;
	} while (readflag);

	csv->filerow = i;	//记录文件行数
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：csv数组扩容(内部函数)
//入口参数：csv指向CSVFile结构体的指针
//返回值：CSVOK函数执行成功,CSVERROR函数执行失败
int csvExpan(CsvHandle csv)
{
	int i, j;
	//扩充LINEINC行数组
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
	csv->linesize += LINEINC;	//更新数组容量
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：读取数字
//入口参数：csv指向CSVFile结构体的指针,row行号,col列号
//返回值：当前数组元素的数值
double csvReadNum(CsvHandle csv, int row, int col)
{
	return _wtof(csv->data[row][col]);
}


//////////////////////////////////////////////////////////////////////////
//函数功能：读取字符串
//入口参数：csv指向CSVFile结构体的指针,row行号,col列号
//返回值：指向该字符串的指针
wchar_t* csvReadStr(CsvHandle csv, int row, int col)
{
	if (csv->data[row][col][0] == L'\0')
		return NULL;
	return csv->data[row][col];
}


//////////////////////////////////////////////////////////////////////////
//函数功能：写入数字
//入口参数：csv指向CSVFile结构体的指针,row行号, col列号, num待写入数值
//返回值：CSVOK函数执行成功,CSVERROR函数执行失败
int csvWriteNum(CsvHandle csv, int row, int col, double num)
{
	while (row >= csv->linesize)	//依据行号扩容
		if(csvExpan(csv)==CSVERROR)
			return CSVERROR;
	swprintf(csv->data[row][col],BLOCKSIZE, L"%lg", num);	//将数字转换为字符串存入数组
	if (row + 1 > csv->filerow)		//更新行数
		csv->filerow = row + 1;
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：写入字符串
//入口参数：csv指向CSVFile结构体的指针,row行号,col列号,str指向写入字符串的指针
//返回值：CSVOK函数执行成功,CSVERROR函数执行失败
int  csvWriteStr(CsvHandle csv, int row, int col, const wchar_t* str)
{
	while (row>=csv->linesize)	//依据行号扩容
		if (csvExpan(csv) == CSVERROR)
			return CSVERROR;
	wcscpy(csv->data[row][col], str);	//将字符串存入数组
	if (row + 1 > csv->filerow)		//更新行数
		csv->filerow = row + 1;
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：保存csv文件
//入口参数：csv指向CSVFile结构体的指针,filepath文件路径
//返回值：CSVOK函数执行成功,CSVERROR函数执行失败
int csvSave(CsvHandle csv, const wchar_t* filepath)
{	
	int i, j, k;
	//打开文件
	if (csv->fp)
		fclose(csv->fp);
	if ((csv->fp = _wfopen(filepath, L"wt")) == NULL)
		return CSVERROR;
	//写入
	for (i = 0; i < csv->filerow; i++)	//按行
	{
		for (j = 0; j < csv->filecol - 1; j++)	//按列
		{
			k = 0;
			while (csv->data[i][j][k] != L'\0')
				fputwc(csv->data[i][j][k++], csv->fp);
			fputwc(L',', csv->fp);
		}
		k = 0;	//最后一个元素单独处理
		while (csv->data[i][j][k] != L'\0')
			fputwc(csv->data[i][j][k++], csv->fp);
		if(i<csv->filerow-1)	//文件结尾不加换行符
			fputwc(L'\n', csv->fp);
	}
	return CSVOK;
}


//////////////////////////////////////////////////////////////////////////
//函数功能：释放csv结构
//入口参数：csv指向CSVFile结构体的指针
//返回值：无
void csvRelease(CsvHandle csv)
{
	int i, j;
	//释放数组内存
	for (i = 0; i < csv->linesize; i++)
	{
		for (j = 0; j < csv->filecol; j++)
			free(csv->data[i][j]);
		free(csv->data[i]);
	}
	free(csv->data);
	//关闭文件指针
	if (csv->fp)	
		fclose(csv->fp);
	csv->fp = NULL;
}


