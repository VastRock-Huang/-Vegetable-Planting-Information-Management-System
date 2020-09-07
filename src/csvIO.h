//////////////////////////////////////////////////////////////////////////
//CSV文件读写库
//操作思想: 将csv文件数据每个字段提取存在一个二维可变长数组中,数组中每个元素为一个宽字节字符串
//数组整体如同一张表格.读写csv文件数据时首先生成二维数组, 读csv文件时将数据加载到数组, 写文件时
//将数组数据写入文件, 具体操作如图操作Excel表格, 对数组中某行某列元素进行操作.
//	*本库与libxl函数执行流程相近, 用于简单替换libxl函数库从而实现由Excel到csv文件的处理, 仅限简单的
//csv文件操作, 对于文件过大或者特殊的文本","并未做处理; 也并不是最优的读写csv文件的方案
//by hhy

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>		//宽字节库
#include <locale.h>		//语言本地化库

#define LINEINC 10		//每次分配csv二维数组行数增量
#define BLOCKSIZE 40	//csv二维数组中每个单元可存放的宽字符数
#define FILEEND 65535	//文件结束符,类似于EOF,宽字节时仍未正数
#define CSVOK  1		//函数执行成功
#define CSVERROR -1		//函数执行失败

typedef struct CsvFile	//csv文件结构体
{
	FILE* fp;			//文件指针
	wchar_t*** data;	//csv二维数组指针(三维字符数组)
	int filecol;		//文件列数
	int filerow;		//文件行数
	int linesize;		//文件行容量
	int blocksize;		//单元格容量
}CsvFile;

typedef CsvFile* CsvHandle;

int csvOpen(CsvHandle csv, int col);		//初始化csv结构
int csvGetColCnt(const wchar_t* filepath);	//获取csv文件列数
int csvLoad(CsvHandle csv,const wchar_t* filepath);		//加载csv文件
int csvExpan(CsvHandle csv);	//扩容csv数组
double csvReadNum(CsvHandle csv, int row, int col);		//读取数字
wchar_t* csvReadStr(CsvHandle csv, int row, int col);	//读取字符串
int csvWriteNum(CsvHandle csv, int row, int col, double num);	//写入数字
int csvWriteStr(CsvHandle csv, int row, int col, const wchar_t* str);	//写入字符串
int csvSave(CsvHandle csv, const wchar_t* filepath);	//保存文件
void csvRelease(CsvHandle csv);		//释放csv结构