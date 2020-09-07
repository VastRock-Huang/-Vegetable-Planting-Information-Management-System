#include "pch.h"
#include "VegData.h"

//����ĺ�˷�װ�����������ļ�



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���߲����ݽṹ�йصı�������
//

Kind* head = NULL;	//�߲�ʮ���������ݽṹ��ͷ
int kindCnt = 0,	//�߲�������Ϣ��Ŀ
vegCnt = 0,		//�߲˻�����Ϣ��Ŀ
plantCnt = 0;	//�߲���ֲ��Ϣ��Ŀ
int vegIdx = 0,		//�߲˱�����ֵ
plantIdx = 0;	//��ֲ��Ϣ������ֵ
wchar_t minYear[5] = L"9999", maxYear[5] = L"0000";		//��ֲ�����С��/�����



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���߲����ݽṹ(����ʮ������)��������ĺ��Ĺ��ܺ���
//


//////////////////////////////////////////////////////////////////////////
//�������ܣ��������ļ��м�������, ��ʼ���߲����ݽṹ(����ʮ������)
//��ڲ�������
//����ֵ����
void initData()
{
	FILE* fp;
	Kind* kNew, * kEnd;	
	Veg* vNew, * vEnd = NULL;
	Plant* pNew, * pEnd = NULL;
	int dataFlag;	//���ݱ�ʶ:�������ֶ�ȡ��Ϣ������

	if ((fp = fopen(DATAFILE, "rb")) == NULL)
		alertMeg(L"��������ʧ�ܣ�");

	head = (Kind*)malloc(sizeof(Kind));	//�����߲�������Ϣ��ͷ���
	kEnd = head;
	head->k_next = NULL;
	head->k_vptr = NULL;

	while ((dataFlag=fgetc(fp))!='$')	//�������ļ��ж�ȡһ���ַ�,�ж��Ƿ��ǽ�����$, �����������������
	{
		switch (dataFlag)
		{
		case 'K':	//��Ϊ�߲�������Ϣ������
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

		case 'V':	//��Ϊ�߲˻�����Ϣ
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

		case 'P':	//��Ϊ�߲���ֲ��Ϣ
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
	fclose(fp);		//�ر��ļ�ָ��
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��������Ż�������Ʋ�ѯ���߲������Ƿ����
//��ڲ�����k_num������, k_name��������, flag��ѯ��ʶ:0ֻ���k_num��1ֻ���k_name��2���߶����
//����ֵ��ָ���ѯ�����߲��������ָ��, ��δ��ѯ�����ؿ�ָ��
Kind* checkInKind(wchar_t k_num,wchar_t* k_name,int flag)
{	
	Kind* tmpk;
	for (tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)	//�����߲�������Ϣ����
	{
		if (flag == 0 && k_num == tmpk->k_num)	//��flag==0, ��������ͬ, ���ҵ�
			return tmpk;
		else if (flag == 1 && wcscmp(k_name, tmpk->k_name) == 0)	//��flag==1, ����������ͬ,���ҵ�
			return tmpk;
		else if (flag==2&&(k_num==tmpk->k_num||wcscmp(k_name,tmpk->k_name)==0))	//��flag==2, �������ƺͱ�Ŷ���ͬ,���ҵ�
			return tmpk;
	}
	return NULL;	//δ�ҵ�
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲����Ʋ�ѯ���߲˻�����Ϣ�Ƿ����
//��ڲ�����v_name�߲�����
//����ֵ��1��ʾ��ѯ��, 0��ʾδ�ҵ�
int checkInVegByName(wchar_t* v_name)
{
	Kind* tmpk;
	Veg* tmpv;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//����ÿ���߲˻�����Ϣ����
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (wcscmp(v_name,tmpv->v_name)==0)		//���߲�������ͬ���ҵ�
				return 1;
	return 0;	//δ�ҵ�
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲˱�Ų�ѯ���߲˻�����Ϣ�Ƿ����
//��ڲ�����vNum�߲˱��
//����ֵ��1��ʾ��ѯ��, 0��ʾδ�ҵ�
int checkInVegByNum(int vNum)
{
	Kind* tmpk;
	Veg* tmpv;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//����ÿ���߲˻�����Ϣ����
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (tmpv->v_num == vNum)		//���߲˱����ͬ���ҵ�
				return 1;
	return 0;	//δ�ҵ�
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����ֲ��Ϣ��Ų�ѯ���߲���ֲ��Ϣ�Ƿ����
//��ڲ�����pNum��ֲ��Ϣ���
//����ֵ��1��ʾ��ѯ��, 0��ʾδ�ҵ�
int checkInPlant(int pNum)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//����ÿ���߲˻�����Ϣ����
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			for(tmpp=tmpv->v_pptr->p_next;tmpp;tmpp=tmpp->p_next)
				if (tmpp->p_num == pNum)		//�������ͬ���ҵ�
					return 1;
	return 0;	//δ�ҵ�
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����߲�������Ϣ���߲����ݽṹ
//��ڲ�����iptָ�����ӵ��߲�������Ϣ��ָ��
//����ֵ����
void addToKind(Kind *ipt)
{
	Kind* tmpk;
	Kind* New = (Kind*)malloc(sizeof(Kind));	//�½��߲�������
	if (!New)
		alertMeg(L"overflow!");
	*New = *ipt;	//������ݸ�ֵ
	New->k_vptr = (Veg*)malloc(sizeof(Veg));	//Ϊ�߲������½�㴴���߲˻�����Ϣ��ͷ���
	New->k_vptr->v_next = NULL;		//���½���е�ָ���ʼ��Ϊ��ָ��
	New->k_vptr->v_pptr = NULL;

	//���շ����������½���������
	for (tmpk = head; tmpk->k_next && New->k_num > tmpk->k_next->k_num; tmpk = tmpk->k_next);
	New->k_next = tmpk->k_next;
	tmpk->k_next = New;
	++kindCnt;	//�߲�������Ϣ��Ŀ+1
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����߲˻�����Ϣ���߲����ݽṹ
//��ڲ�����iptָ�����ӵ��߲˻�����Ϣ��ָ��
//����ֵ����
void addToVeg(Veg* ipt)
{
	Kind* tmpk;
	Veg* tmpv;

	Veg* New = (Veg*)malloc(sizeof(Veg));	//�½��߲˻�����Ϣ���
	if (!New)
		alertMeg(L"overflow!");
	*New = *ipt;	//������ݸ�ֵ
	New->v_num = ++vegIdx;	//���߲˻�����Ϣ���߲˱��
	New->v_next = NULL;		//���½���е�ָ���ʼ��Ϊ��ָ��
	New->v_pptr = (Plant*)malloc(sizeof(Plant));	//Ϊ�߲������½�㴴���߲˻�����Ϣ��ͷ���
	New->v_pptr->p_next = NULL;

	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)	//�ҵ�����߲��������߲�����
		if (New->v_kind == tmpk->k_num)
			break;
	for (tmpv = tmpk->k_vptr; tmpv->v_next; tmpv = tmpv->v_next);	//�����߲���ӵ�����β
	tmpv->v_next = New;
	++vegCnt;	//�߲˻�����Ϣ��Ŀ+1
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����߲���ֲ��Ϣ���߲����ݽṹ
//��ڲ�����iptָ�����ӵ��߲���ֲ��Ϣ��ָ��
//����ֵ����
void addToPlant(Plant* ipt)
{
	Kind* tmpk;
	Veg* tmpv=NULL;
	Plant* tmpp;

	Plant* New = (Plant*)malloc(sizeof(Plant));		//�½��߲���ֲ��Ϣ���
	if (!New)
		alertMeg(L"overflow!");
	*New = *ipt;	//������ݸ�ֵ
	New->p_num = ++plantIdx;	//���߲���ֲ��Ϣ���
	New->p_next = NULL;		//���½���е�ָ���ʼ��Ϊ��ָ��

	//������ֲ�����С��/�����
	if (wcscmp(minYear, New->p_year) > 0)
		wcscpy(minYear, New->p_year);
	if (wcscmp(maxYear, New->p_year) < 0)
		wcscpy(maxYear, New->p_year);

	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (ipt->p_vnum == tmpv->v_num)		//�ҵ�����߲���ֲ��Ϣ�������߲�
			{
				for (tmpp = tmpv->v_pptr; tmpp->p_next; tmpp = tmpp->p_next);	//������ֲ��Ϣ��ӵ�����β
				tmpp->p_next = New;
				++plantCnt;	//�߲���ֲ��Ϣ��Ŀ+1
				return;
			}	
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�ɾ��һ���߲��߲�������Ϣ���߲����ݽṹ
//��ڲ�����idx��ɾ���߲������������е����
//����ֵ����
void delInKind(int idx)
{
	Kind* prek, * curk;
	Veg* nextv, *curv;
	int i;

	//���������ҵ���ɾ�����߲�������
	for (i=1, prek = head, curk = prek->k_next; curk; curk = curk->k_next,++i)
	{
		if (i==idx)
			break;
		prek = curk;
	}	
	
	//ɾ�����߲���������߲�����
	for (curv=curk->k_vptr->v_next;curv;curv=nextv)
	{
		nextv = curv->v_next;
		delInVeg(curv->v_num);
	}

	//�ͷŸ��߲�������
	prek->k_next = curk->k_next;
	free(curk->k_vptr);
	free(curk);
	--kindCnt;	//�߲�������Ϣ��Ŀ-1
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�ɾ��һ���߲˻�����Ϣ���߲����ݽṹ
//��ڲ�����num��ɾ�����߲˻�����Ϣ���߲˱��
//����ֵ����
void delInVeg(int num)
{
	Kind* tmpk;
	Veg* pre = NULL, * cur = NULL;
	Plant* curp, * nextp;

	//����ʮ�������ҵ���ɾ�����߲˻�����Ϣ���
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (pre = tmpk->k_vptr, cur = pre->v_next; cur; cur = cur->v_next)
		{
			if (cur->v_num == num)
				goto forEnd;
			pre = cur;
		}
forEnd:		//ɾ�����߲˻�����Ϣ�����߲���ֲ��Ϣ����
	pre->v_next = cur->v_next;
	for (curp = cur->v_pptr->p_next; curp; curp = nextp)
	{
		nextp = curp->p_next;
		free(curp);
		--plantCnt;
	}
	//�ͷŸ��߲˻�����Ϣ���
	free(cur->v_pptr);
	free(cur);
	--vegCnt;	//�߲˻�����Ϣ��Ŀ-1
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�ɾ��һ���߲���ֲ��Ϣ���߲����ݽṹ
//��ڲ�����num��ɾ���߲���ֲ��Ϣ�ı��
//����ֵ����
void delInPlant(int num)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* curp = NULL, * prep = NULL;

	//�����߲����ݽṹ�ҵ���ɾ�����߲˻�����Ϣ���
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for(tmpv=tmpk->k_vptr->v_next;tmpv;tmpv=tmpv->v_next)
			for (prep = tmpv->v_pptr, curp = prep->p_next; curp; curp = curp->p_next)
			{
				if (curp->p_num == num)
					goto forEnd;
				prep = curp;
			}
forEnd:		//�ͷŸ���ֲ��Ϣ���
	prep->p_next = curp->p_next;
	free(curp);
	--plantCnt;	//��ֲ��Ϣ��Ŀ-1
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��޸�ĳһ�߲˻�����Ϣ���߲����ݽṹ��
//��ڲ�����iptָ�����޸����ݵ��߲˻�����Ϣ�ṹ��Veg��ָ��, k_num���޸ĵ��߲˻�����Ϣԭ���ķ�����
//����ֵ����
void modInVeg(Veg* ipt, wchar_t k_num)
{
	Kind* tmpk;
	Veg* preV=NULL, * curv=NULL, *tmpv=NULL;

	//ͨ��ԭ�������ҵ����޸ĵ��߲˻�����Ϣ���curv
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

forEnd1:	//�ҵ������޸ĺ�����ݸ�ֵ�������
	wcscpy(curv->v_name, ipt->v_name);
	wcscpy(curv->v_nutrition, ipt->v_nutrition);

	//��Ϊ���߲˵ķ������޸�,��������޸ķ���
	if (k_num == ipt->v_kind)
		return;

	//�����߲˵ķ����Ž������޸�,���轫�ý��������߲�������Ϣ�µ��߲�����
	preV->v_next = curv->v_next;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		if (tmpk->k_num == ipt->v_kind)
		{
			for (tmpv = tmpk->k_vptr; tmpv->v_next; tmpv = tmpv->v_next)
				if (tmpv->v_next->v_num > curv->v_num)	//����λ��ʱ�����߲˱������
					break;

			curv->v_next = tmpv->v_next;
			tmpv->v_next = curv;
			curv->v_kind = ipt->v_kind;	//�޸ķ�����
			return;
		}
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��޸�ĳһ�߲�������Ϣ���߲����ݽṹ��
//��ڲ�����idx>=1���޸������������������еĴ���, iptָ�����޸���������ṹ��Kind��ָ��
//����ֵ����
void modInKind(int idx,Kind* ipt)
{
	Kind* pre, * cur,*tmpk;
	Veg* tmpv;
	int i; 

	for (i = 1, pre = head, cur = pre->k_next; i<idx; cur = cur->k_next, ++i)	//���������޸��߲�������
		pre = cur;

	wcscpy(cur->k_name, ipt->k_name);	//�޸ķ�������

	if(ipt->k_num==cur->k_num)	//��δ�޸ķ�����, �򷵻�
	{
		return;
	}

	//���޸��˷�����
	cur->k_num = ipt->k_num;
	pre->k_next = cur->k_next;		//���ý��cur��ԭ������ȡ��

	//���������޸ĺ�Ľ�����°�������������뵽������
	for (tmpk = head; tmpk->k_next && cur->k_num > tmpk->k_next->k_num; tmpk = tmpk->k_next);
	cur->k_next = tmpk->k_next;
	tmpk->k_next = cur;
	//wcscpy(cur->k_name, ipt->k_name);

	//���ý���µ��߲˻�����Ϣ�����и�����ķ����Ž����޸�
	for (tmpv = cur->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		tmpv->v_kind = cur->k_num;

}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��޸�ĳһ�߲���ֲ��Ϣ���߲����ݽṹ��
//��ڲ�����iptָ�����޸�������ֲ��Ϣ�ṹ��Plant��ָ��,v_num���޸ĵ��߲���ֲ��Ϣԭ�����߲˱��
//����ֵ����
void modInPlant(Plant* ipt, int v_num)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* prep = NULL, * curp = NULL, * tmpp = NULL;

	//���������޸��߲���ֲ��Ϣ���
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

forEnd1:	//�޸����ݸ�ֵ
	curp->p_area = ipt->p_area;
	curp->p_wegiht = ipt->p_wegiht;
	wcscpy(curp->p_year, ipt->p_year);

	//��δ�޸��߲˱���򷵻�
	if (ipt->p_vnum == v_num)
		return;

	//���޸����߲˱�����ҵ��޸ĺ���߲˽��
	prep->p_next = curp->p_next;
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if(tmpv->v_num==ipt->p_vnum)
				for (tmpp = tmpv->v_pptr; tmpp->p_next; tmpp = tmpp->p_next)
					if (tmpp->p_next->p_num > curp->p_num)	//����λ��ʱ������ֲ��Ϣ�������
						goto forEnd2;
forEnd2:	
	curp->p_next = tmpp->p_next;
	tmpp->p_next = curp;
	curp->p_vnum = ipt->p_vnum;		//�޸��߲˱��
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲������Ӵ������߲˻�����Ϣ,����ӵ��б�
//��ڲ�����subname��ѯ���߲������Ӵ�, list_veg������ʾ��ѯ�����MFC�б�ؼ�
//����ֵ����
void secVegByName(wchar_t* subname, CListCtrl* list_veg)
{
	Kind* tmpk;
	Veg* tmpv;
	int i = 0;
	//���������߲�����
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			if (wcswcs(tmpv->v_name, subname))	//��ĳ���߲˵������а����Ӵ�, �򽫸ý���������ӵ��б�
				addVegToList(list_veg, tmpv, i++, 1);

	if (i == 0)		//��δ�ҵ�, ���б���� δ�ҵ� 
		list_veg->InsertItem(0, L"δ�ҵ�!!!");
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����������Ӫ���ɷֲ����߲˻�����Ϣ,����ӵ��б�
//��ڲ�����kNum��ѯ�ķ�����, nut��ѯ��Ӫ���ɷ�, list_veg������ʾ��ѯ�����MFC�б�ؼ�
//����ֵ����
void secVegByKindNut(wchar_t kNum, wchar_t* nut,CListCtrl* list_veg)
{
	wchar_t* wpp[5];	//Ӫ���ɷ�����
	Kind* tmpk;
	Veg* tmpv;
	int i = 0;	//ͳ�Ʒ�����������Ϣ��Ŀ
	int j;	//���ڱ���Ӫ���ɷ�����
	int flag = 1;	//������ʶ: 0��ʾ����������, 1��ʾ��������

	wcssplit(wpp, nut, L"��");	//��Ӫ���ɷ�nut����"��"�ŷָ���ַ������浽wpp��,ÿ���ַ�������һ��Ӫ���ɷ�
	
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		if (kNum != 0 && tmpk->k_num != kNum)	//��kNum��Ϊ���ַ����ҵ�ǰ�����㲻�����ѯ�ķ�����, ������
			continue;
		//��kNumΪ���ַ���(��δ�����ѯ�ķ�����)��ǰ���������������,��������߲˻�����Ϣ����
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			flag = 1;
			if (nut[0] != 0)	//����ѯ��Ӫ���ɷֲ�Ϊ���ַ���
				for(j = 0; wpp[j]; ++j)		//���жϸ��߲˽���Ƿ�ͬʱ������ѯ��ÿ��Ӫ���ɷ�
					if (wcswcs(tmpv->v_nutrition, wpp[j]) == NULL)	//����һֱӪ���ɷֲ�����,��flag��0
					{
						flag = 0;
						break;
					}
			if (flag == 0)	//��flag==0������,�����ж���һ�߲˽��
				continue;
			addVegToList(list_veg, tmpv, i++, 1);	//���������򽫸ý��������ӵ��б�
		}
	}
	if (i == 0)		//��δ�ҵ�, ���б���� δ�ҵ� 
		list_veg->InsertItem(0, L"δ�ҵ�!!!");
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲˲������ƻ���ֲ��ݲ����߲���ֲ��Ϣ,����ӵ��б�
//��ڲ�����subname��ѯ���߲˲�������, year��ѯ����ֲ���, list_plant������ʾ��ѯ�����MFC�б�ؼ�
//����ֵ����
void secPlantByVnameYear(wchar_t* subname, wchar_t* year, CListCtrl* list_plant)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	//��������ʮ������
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if(subname[0]!=0&&wcswcs(tmpv->v_name,subname)==NULL)	//��subname��Ϊ���ַ����ҵ�ǰ�߲˽�㲻������ѯ���Ӵ�, ������
				continue;
			//��subnameΪ���ַ���(��δ�����ѯ���߲˲�������)��ǰ�߲˽�������ѯ���Ӵ�,��������߲���ֲ��Ϣ����
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				if(year[0]!=0&&wcscmp(year,tmpp->p_year)!=0)	//����ѯ��ݲ�Ϊ���ַ�������ֲ��Ϣ������ֲ������ѯ��ͬ������
					continue;
				addPlantToList(list_plant, tmpp,i++);	//���������򽫸ý��������ӵ��б�
			}
		}
	if (i == 0)		//δ�ҵ�
		list_plant->InsertItem(0, L"δ�ҵ�!!!");
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲����Ʋ�ѯ�߲���ֲ��Ϣ,����ӵ��б�
//��ڲ�����vName��ѯ���߲�����, list_plant������ʾ��ѯ�����MFC�б�ؼ�
//����ֵ����
void secPlantByVname(wchar_t* vName, CListCtrl* list_plant)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	//��������ʮ������
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if (wcscmp(vName, tmpv->v_name) == 0)	//����ǰ�߲˽����߲����������ѯ���߲�����һ��
				for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)	//������������ֲ��Ϣ�����ӵ��б�
					addPlantToList(list_plant, tmpp, i++);
		}
	if (i == 0)		//δ�ҵ�
		list_plant->InsertItem(0, L"δ�ҵ�!!!");

}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�ͳ��ĳ������߲���ֲ��Ϣ
//��ڲ�����statPla���ڴ�ŷ���������ͳ������, yearͳ�Ƶ���ֲ���
//����ֵ����
void statPlantOfKind(StatData* statPla,wchar_t* year)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	//�ֱ�ͳ�Ʋ�ͬ�����߲˵���ֲ��Ϣ
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next, ++i)
	{
		statPla[i].pKind = tmpk;	//�߲�����
		statPla[i].p_area = 0;
		statPla[i].p_wegiht = 0;
		wcscpy(statPla[i].p_year, year);
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				if (year[0]!=0&&wcscmp(year,tmpp->p_year)!=0)	//��������ֲ���������
					continue;
				//����ֲ��� �ջ�����ͳ��
				statPla[i].p_area += tmpp->p_area;
				statPla[i].p_wegiht += tmpp->p_wegiht;
			}
	}
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����ֹ���Ϊ�������߲�����ͳ����ֲ��Ϣ
//��ڲ�����statPla���ڴ�ŷ���������ͳ������, year1ͳ�Ƶ���ʼ���, year2ͳ�Ƶ���ֹ���
//����ֵ��ͳ�����ݵ�����
int statPlantOfVegYear(StatData* statPla, wchar_t* year1, wchar_t* year2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	int flag;	//ͳ�Ʊ�ʶ,0��ʾ���߲˵���ֲ��Ϣδ��ͳ��, 1��ʾ���߲���ֲ��Ϣ��ͳ����

	statPla[i].p_area = 0;
	statPla[i].p_wegiht = 0;

	//��������ʮ������
	for(tmpk=head->k_next;tmpk;tmpk=tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			flag = 0;	//����ͳ�Ʊ�ʶ
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				//����ֲ��Ϣ������ݲ�������ʼ�������������
				if (wcscmp(year1,tmpp->p_year)>0)
					continue;
				if(wcscmp(year2,tmpp->p_year)<0)
					continue;
				//���������Ľ������ݽ���ͳ��
				statPla[i].p_area += tmpp->p_area;
				statPla[i].p_wegiht += tmpp->p_wegiht;
				flag = 1;	//����ͳ�Ʊ�ʶ, ���߲˽�����ֲ��Ϣ��ͳ����
			}
			if (flag)	//�����߲˽������Ϣ��ͳ��, ������ͳ����Ϣ
			{
				statPla[i].pKind = tmpk;
				statPla[i].pVeg = tmpv;
				++i;
				statPla[i].p_area = 0;
				statPla[i].p_wegiht = 0;
			}
		}
	if(i>1)		//��ͳ��������Ŀ����1���ջ�������������
		qucikSortByWeight(statPla, 0, i - 1);
	return i;	//����ͳ����������
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�ͳ��ĳ�߲˽��������ֲ��Ϣ
//��ڲ�����statPla���ڴ��ͳ������, VName��ͳ�Ƶ��߲�����
//����ֵ����
void statVegByLeastYear(StatData* statPla, wchar_t* vName)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i;

	time_t now = time(0);
	struct tm* myTime = localtime(&now);
	int year = myTime->tm_year + 1900;		//��ȡ��ǰ���

	//��������ʮ������
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr; tmpv; tmpv = tmpv->v_next)
		{
			if (wcscmp(vName, tmpv->v_name) == 0)	//�ҵ���ͳ���߲�
			{
				for (i = 0; i < 3; ++i)		//ͳ�ƽ���������
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
//�������ܣ�ÿ���߲˰����ͳ����ֲ��Ϣ
//��ڲ�����statPla���ڴ��ͳ����Ϣ, vName�߲�����, year1��ʼ���, year2��ֹ���
//����ֵ����
void statVegByYear(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i;

	int minY = _wtoi(year1), maxY = _wtoi(year2);
	int yearCnt = maxY - minY + 1;	//�������
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr; tmpv; tmpv = tmpv->v_next)
		{
			if (wcscmp(vName, tmpv->v_name) == 0)	//�ҵ������߲�
			{
				for (i = 0; i < yearCnt; ++i)	//ͳ��ÿ���߲�ÿһ�����ֲ��Ϣ
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
//�������ܣ�����Excel���ݱ��
//��ڲ�����dataType������Ϣ����(1�߲�������Ϣ,2�߲˻�����Ϣ,3�߲���ֲ��Ϣ), filePath�����ļ�·��, mode����ģʽ(0����,1�ϸ�)
//����ֵ��0��ʾ�ļ���ʧ��,-1������������, 1���ݵ���ɹ�
int inputFile(int dataType, wchar_t *filePath, int mode)
{	//0����ģʽ���ϸ�ģʽ
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
	csvOpen(csv,filecol);	//�����ݱ�񼰱�
	if (csvLoad(csv, filePath)==-1)
		return 0;

	switch(dataType)	//�ж���Ϣ����
	{
	case 1:		//�����߲�������Ϣ
		if (mode == 1)	//�ϸ�ģʽ
		{
			//�����������
			for (col = 1; col<filecol; ++col)
			{
				//�������������������Ѿ�����������(ϵͳ)�д���,�򷵻�
				tmpk.k_num = (int)csvReadNum(csv, 0, col) + L'0';
				strp = csvReadStr(csv, 1, col);
				if (strp == NULL || checkInKind(tmpk.k_num, (wchar_t*)strp, 2)!=NULL)
					return -1;
			}
		}
		//����������ݲ�����
		for (col = 1; col < filecol; ++col)
		{
			tmpk.k_num = (int)csvReadNum(csv, 0, col) + L'0';
			strp = csvReadStr(csv, 1, col);
			//������ģʽ, ��ǰ������ϵͳ���Ѵ���������
			if (mode == 0 && (strp==NULL || checkInKind(tmpk.k_num,(wchar_t*)strp, 2)!=NULL))
					continue;
			wcscpy(tmpk.k_name, strp);

			addToKind(&tmpk);	//��������ӵ�ϵͳ��
		}
		break;

	case 2:		//�����߲˻�����Ϣ
		if (mode == 1)	//�ϸ�ģʽ
		{
			//�������, ���������������
			for (row = 1; row<csv->filerow; ++row)
			{
				if (checkInVegByNum((int)csvReadNum(csv, row, 0))==1)	//�������ݱ����ϵͳ�г��ֹ�
					return -1;
				strp = csvReadStr(csv, row, 1);
				if (strp==NULL || checkInVegByName((wchar_t*)strp) == 1)	//���߲�����Ϊ�ջ���ϵͳ���Ѵ���
					return -1;
				tmpv.v_kind = (int)csvReadNum(csv, row, 2) + L'0';
				if (checkInKind(tmpv.v_kind, L"", 0) == NULL)	//�߲˶�Ӧ�����಻����
					return -1;
				if (csvReadStr(csv, row, 3) == NULL)	//Ӫ���ɷ�Ϊ��
					return -1;
			}
		}
		//�������������,����ģʽ�������������
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
			addToVeg(&tmpv);	//����߲���Ϣ��ϵͳ
		}
		break;

	case 3:		//�����߲���ֲ��Ϣ
		if (mode == 1)	//�ϸ�ģʽ
		{	
			//�������, ���������������
			for (row = 1; row < csv->filerow; ++row)
			{
				if (checkInPlant((int)csvReadNum(csv, row, 0)) == 1)	//���߲���ֲ��Ϣ�����ϵͳ�г��ֹ�
					return -1;
				temp = (int)csvReadNum(csv, row, 1);
				if (checkInVegByNum(temp) == 0)	//����ֲ��Ϣ��Ӧ�߲˲�����
					return -1;
			}
		}
		//�������������,����ģʽ�������������
		for (row = 1; row < csv->filerow; ++row)
		{
			tmpp.p_vnum = (int)csvReadNum(csv, row, 1);
			if(mode==0&&(tmpp.p_vnum>vegCnt||tmpp.p_vnum==0))
				continue;
			tmpp.p_area = (int)csvReadNum(csv, row, 2);
			tmpp.p_wegiht = (float)csvReadNum(csv, row, 3);
			temp = (int)csvReadNum(csv, row, 4);
			_itow_s(temp, tmpp.p_year, 10);
			addToPlant(&tmpp);		//����߲���ֲ��Ϣ��ϵͳ
		}
		break;
	}

	csvRelease(csv);	//�رձ���ļ�
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ������������ͳ�����ݵ�Excel���
//��ڲ�����statPla������������, �����ļ�·��
//����ֵ��1��ʾ�����ɹ�, 0��ʾ����ʧ��
int outPlantOfKind(StatData* statPla,wchar_t* filePath)
{
	int i;
	CsvFile csvf;
	CsvHandle csv=&csvf;
	csvOpen(csv,4);

	//д���ͷ
	csvWriteStr(csv, 0, 0, L"���");
	csvWriteStr(csv, 0, 1, L"��������");
	csvWriteStr(csv, 0, 2, L"��ֲ������֣�");
	csvWriteStr(csv, 0, 3, L"�ջ���������kg��");
	for (i = 0; i < kindCnt; ++i)	//��������
	{
		csvWriteNum(csv, i + 1, 0, _wtoi(statPla[i].p_year));
		csvWriteStr(csv, i + 1, 1, statPla[i].pKind->k_name);
		csvWriteNum(csv, i + 1, 2, statPla[i].p_area);
		csvWriteNum(csv, i + 1, 3, statPla[i].p_wegiht);
	}
	if (!csvSave(csv, filePath))	//������
		return 0;
	csvRelease(csv);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��������߲˵�ͳ�����ݵ�Excel���
//��ڲ�����statPla������������, �����ļ�·��
//����ֵ��1��ʾ�����ɹ�, 0��ʾ����ʧ��
int outPlantOfVeg(StatData* statPla, wchar_t* filePath)
{
	int i;
	CsvFile csvf;
	CsvHandle csv = &csvf;
	csvOpen(csv,5);
	
	//д���ͷ
	csvWriteStr(csv, 0, 0, L"���");
	csvWriteStr(csv, 0, 1, L"�߲�����");
	csvWriteStr(csv, 0, 2, L"��������");
	csvWriteStr(csv, 0, 3, L"��ֲ������֣�");
	csvWriteStr(csv, 0, 4, L"�ջ�������kg��");
	for (i = 0; i < 3; ++i)		//��������
	{
		csvWriteNum(csv, i + 1, 0, _wtoi(statPla[i].p_year));
		csvWriteStr(csv, i + 1, 1, statPla[i].pVeg->v_name);
		csvWriteStr(csv, i + 1, 2, statPla[i].pKind->k_name);
		csvWriteNum(csv, i + 1, 3, statPla[i].p_area);
		csvWriteNum(csv, i + 1, 4, statPla[i].p_wegiht);
	}
	if (!csvSave(csv, filePath))	//������
		return 0;
	csvRelease(csv);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ��Զ��尴�߲�����ͳ����ֲ��Ϣ
//��ڲ�����statPla���ڴ��ͳ������,kIdx��ͳ���߲���������������Ĵ���, year1��ʼ���, year2��ֹ���
//			area1��С��ֲ���, area2�����ֲ���, wig1��С�ջ�����, wig2����ջ�����
//����ֵ����������
int custzStatKind(StatData* statPla, int kIdx, wchar_t* year1,
	wchar_t* year2, int area1, int area2, float wig1, float wig2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	int idx;
	int yearFlag = wcscmp(year2, L"NaN");	//��ݱ�ʶ,��Ϊ0��ʾ�������,0��ʾ�������

	//��������ʮ������
	for (idx=0,tmpk = head->k_next; tmpk; tmpk = tmpk->k_next,++idx)
	{
		if(kIdx>=0&&idx!=kIdx)	//���������ƥ��������,kIdx<0��ʾȫ������
			continue;
		for (tmpv=tmpk->k_vptr->v_next;tmpv;tmpv=tmpv->v_next)
		{
			for (tmpp=tmpv->v_pptr->p_next;tmpp;tmpp=tmpp->p_next)
			{
				if (yearFlag != 0)	//���������, �������������������
				{
					if (year1[0]!=0&&wcscmp(tmpp->p_year, year1) < 0)
						continue;
					if(year2[0]!=0&&wcscmp(tmpp->p_year,year2)>0)
						continue;
				}
				else if (wcscmp(year1, tmpp->p_year) != 0)	//���������, �����ھ������������
					continue;

				//������ֲ�������������
				if (area1>0&&tmpp->p_area<area1)
					continue;
				if(area2>0&&tmpp->p_area>area2)
					continue;
				//�����ջ���������������
				if(wig1>0&&tmpp->p_wegiht<wig1)
					continue;
				if(wig2>0&&tmpp->p_wegiht>wig2)
					continue;
				//��������������ͳ��
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
//�������ܣ��Զ��尴�߲˻�����Ϣͳ����ֲ��Ϣ
//��ڲ�����statPla���ڴ��ͳ������,VName��ͳ���߲˵�����, year1��ʼ���, year2��ֹ���
//			area1��С��ֲ���, area2�����ֲ���, wig1��С�ջ�����, wig2����ջ�����
//����ֵ����������
int custzStatVeg(StatData* statPla, wchar_t* vName, wchar_t* year1, wchar_t* year2, int area1, int area2, float wig1, float wig2)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	int i = 0;
	int yearFlag = wcscmp(year2, L"NaN");	//��ݱ�ʶ,��Ϊ0��ʾ�������,0��ʾ�������
	//��������ʮ������
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			if(vName[0]!=0&&wcscmp(vName,tmpv->v_name)!=0)	//�����Ʋ���ͬ������
				continue;
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				if (yearFlag != 0)	//���������, �������������������
				{
					if (year1[0] != 0 && wcscmp(tmpp->p_year, year1) < 0)
						continue;
					if (year2[0] != 0 && wcscmp(tmpp->p_year, year2) > 0)
						continue;
				}
				else if (wcscmp(year1, tmpp->p_year) != 0)	//���������, �����ھ������������
					continue;
				//������ֲ�������������
				if (area1 > 0 && tmpp->p_area < area1)
					continue;
				if (area2 > 0 && tmpp->p_area > area2)
					continue;
				//�����ջ���������������
				if (wig1 > 0 && tmpp->p_wegiht < wig1)
					continue;
				if (wig2 > 0 && tmpp->p_wegiht > wig2)
					continue;
				//��������������ͳ��
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
//�������ܣ��Զ��嵼�����ݵ�Excel���
//��ڲ�����statPla������������, filePath�����ļ�·��, dataCnt������������, colCnt�����������Ե�����, checkFlag�������ԵĴ���
//����ֵ��1�����ɹ�, 0����ʧ��
int outCustzFile(StatData* statPla, wchar_t* filePath, int dataCnt, int colCnt, int* checkFlag)
{
	int i, j, k;
	CsvFile csvf;
	CsvHandle csv = &csvf;
	csvOpen(csv, colCnt);

	for (i = 0; i < colCnt; ++i)
	{
		for (j = 0; j < CHKCNT; ++j)	//����checkFlag
		{
			if (checkFlag[j] == i + 1)	//�����к�����ִ����Ӧʱ, ���ո����Գ��ֵĴ��򵼳�����
			{
					switch (j)	//check[j]�������Զ�Ӧ��ϵ
					{
					case 0:csvWriteStr(csv, 0, i, L"��������");
						for (k = 0; k < dataCnt; ++k)
							csvWriteStr(csv, k + 1, i, statPla[k].pKind->k_name);
						break;

					case 1:csvWriteStr(csv, 0, i, L"�߲�����");
						for (k = 0; k < dataCnt; ++k)
							csvWriteStr(csv, k + 1, i, statPla[k].pVeg->v_name);
						break;

					case 2:csvWriteStr(csv, 0, i, L"������");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, statPla[k].pKind->k_num - L'0');
						break;

					case 3:csvWriteStr(csv, 0, i, L"Ӫ���ɷ�");
						for (k = 0; k < dataCnt; ++k)
							csvWriteStr(csv, k + 1, i, statPla[k].pVeg->v_nutrition);
						break;

					case 4:csvWriteStr(csv, 0, i, L"��ֲ������֣�");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, statPla[k].p_area);
						break;

					case 5:csvWriteStr(csv, 0, i, L"�ջ�������kg��");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, statPla[k].p_wegiht);
						break;

					case 6:csvWriteStr(csv, 0, i, L"��ֲ���");
						for (k = 0; k < dataCnt; ++k)
							csvWriteNum(csv, k + 1, i, _wtoi(statPla[k].p_year));
						break;
					}
					break;
			}
		}

	}
	if (!csvSave(csv, filePath))	//������
		return 0;
	csvRelease(csv);
	return 1;

}


//////////////////////////////////////////////////////////////////////////
//�������ܣ������߲����ݽṹ(����ʮ������)
//��ڲ�������
//����ֵ����
void freeData()
{
	Kind* curk, *nextk;
	Veg* curv, *nextv;
	Plant* curp,* nextp;
	
	//���ڲ�����Ľ�㵽�ⲿ���������ͷ�
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
//�������ܣ��洢���ݽṹ�е���Ϣ�������ļ���
//��ڲ�������
//����ֵ����
int saveData()
{
	FILE* fp;
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	
	if ((fp = fopen(DATAFILE, "wb")) == NULL)	//�������ļ�
	{
		MessageBox(NULL, L"��������ʧ�������ԣ�", L"����", MB_OK);
		return 0;
	}

	//��������ʮ������������д�������ļ�
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		fputc('K', fp);		//�߲�������Ϣǰ�ӱ�ʶ��'K'
		fwrite(tmpk, sizeof(Kind), 1, fp);
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
		{
			fputc('V', fp);		//�߲˻�����Ϣǰ�ӱ�ʶ��'V'
			fwrite(tmpv, sizeof(Veg), 1, fp);
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			{
				fputc('P', fp);		//�߲���ֲ��Ϣǰ�ӱ�ʶ��'P'
				fwrite(tmpp, sizeof(Plant), 1, fp);
			}
		}
	}
	putc('$', fp);	//�洢������Ϻ�ӽ�����'$'
	fclose(fp);
	return 1;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�������ݲ�����װ����
//


//////////////////////////////////////////////////////////////////////////
//�������ܣ����ַ���src����delim���зָ�,�洢��wppָ���������
//��ڲ�����wpp����ָ���ŷָ���ַ�������Ķ���ָ��, srcԴ�ַ���, delim�ָ��
//����ֵ����
void wcssplit(wchar_t** wpp, wchar_t* src, wchar_t* delim)
{
	wchar_t* buffer;	//buffer�����ݴ�Դ�ַ���
	int i = 0;
	//��Դ�ַ�����delim�ָ�,����һ���ָ���Ĵ���������(��delim��Դ�ַ���������,Ҳ�Ὣ�����ַ�������wpp[i]��)
	wpp[i] = wcstok(src, delim, &buffer);
	while (wpp[i])	//������ֵ��Ϊ���ַ���
	{
		i++;
		wpp[i] = wcstok(NULL, delim, &buffer);	//��Դ�ַ���srcʣ�ಿ�ּ����ָ�
	}
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�������a���ջ����������������
//��ڲ�����aָ������������ָ��, left�����򲿷�������СԪ��, right�����򲿷��������Ԫ��
//����ֵ����
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
//����˷�װ����,
// *���ڽ���ʹ��mfc���,����ʹ�ö�������,����ΪC�����﷨
//


//////////////////////////////////////////////////////////////////////////
//�������ܣ�������ʾ��, ��ʾ������Ϣ���˳�����
//��ڲ�����meg������Ϣ
//����ֵ����
void alertMeg(wchar_t* meg)
{
	MessageBox(NULL, meg, L"����", MB_OK);
	exit(1);
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ʼ���б�
//��ڲ�����lis�б�, head�б��ͷ���ַ�������, width�б�ÿһ�еĿ��, n�б�����
//����ֵ����
void initList(CListCtrl* lis, CString head[], int* width, int n)
{
	int i;
	//�����б���չ���
	lis->SetExtendedStyle(lis->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//��ӱ�ͷ
	for (i = 0; i < n; i++)
		lis->InsertColumn(i, head[i], LVCFMT_LEFT, width[i]);
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲˻�����Ϣ��ӵ��б��i��
//��ڲ�����lis�б�, v����ӵ��߲˻�����Ϣ, i����ӵ���, flag��ʾ��ʶ: 1����ʾ������,0����ʾ
//����ֵ����
void addVegToList(CListCtrl* lis, Veg* v, int i, int flag)
{
	CString str;
	int col = 0;
	//�������
	str.Format(L"%d", v->v_num);
	lis->InsertItem(i, str);
	lis->SetItemText(i, ++col, v->v_name);
	if (flag)
	{
		str.Format(L"%c", v->v_kind);
		lis->SetItemText(i, ++col, str);
	}
	lis->SetItemText(i, ++col, v->v_nutrition);
	lis->SetItemData(i, v->v_num);	//Ϊ�б������������ID��Ϊ�߲˱��
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲�������Ϣ��ӵ��б��i��
//��ڲ�����lis�б�, k����ӵ��߲�������Ϣ, i����ӵ���
//����ֵ����
void addKindToList(CListCtrl* lis, Kind* k, int i)
{
	CString str;
	//�������
	str.Format(L"%c", k->k_num);
	lis->InsertItem(i, str);
	lis->SetItemText(i, 1, k->k_name);	//Ϊ�б������������ID��Ϊ������
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����߲���ֲ��Ϣ��ӵ��б��i��
//��ڲ�����lis�б�, p����ӵ��߲���ֲ��Ϣ, i����ӵ���
//����ֵ����
void addPlantToList(CListCtrl* lis, Plant* p, int i)
{
	CString str;
	int tmp = p->p_num * 100 + _wtoi(p->p_year) % 100;	//��ֲ��Ϣ��ź�������һ����ʶ
	//�������
	str.Format(L"%d", p->p_num);
	lis->InsertItem(i, str);
	str.Format(L"%d", p->p_vnum);
	lis->SetItemText(i, 1, str);
	str.Format(L"%d", p->p_area);
	lis->SetItemText(i, 2, str);
	str.Format(L"%.2f", p->p_wegiht);
	lis->SetItemText(i, 3, str);
	lis->SetItemText(i, 4, p->p_year);
	lis->SetItemData(i, tmp);	//Ϊ�б������������ID��Ϊtmp
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ����ͳ�����ݵ��б�
//��ڲ�����lis�б�, pָ��ͳ�����ݵ�ָ��, n��������, flag��Ϣ���ͱ�ʶ:0������Ϣ��1�߲���Ϣ��2���������Ϣ
//����ֵ����
void addStatDataToList(CListCtrl* lis, StatData* p, int n, int flag)
{
	CString str;
	int col;
	lis->DeleteAllItems();	//����б�
	for (int i = 0; i < n; ++i)		//������Ϣ���ͽ�������ӵ��б�
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
//�������ܣ��������߲�������Ϣ��ӵ��б���ʾ
//��ڲ�����lis_kind�б�
//����ֵ����
void showAllKind(CListCtrl* list_kind)
{
	
	Kind* tmpk;

	list_kind->DeleteAllItems();
	int i = 0;
	//�����߲�������Ϣ������ӵ��б�
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next, ++i)
		addKindToList(list_kind, tmpk, i);
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ȫ���߲˻�����Ϣ��ӵ��б���ʾ
//��ڲ�����lis_veg�б�, flag:��ʾ��ʶ: 1����ʾ������,0����ʾ
//����ֵ����
void showAllVeg(CListCtrl* list_veg, int flag)
{
	
	Kind* tmpk;
	Veg* tmpv;
	int i = 0;

	list_veg->DeleteAllItems();	//ɾ���߲˻�����Ϣ�б���ԭ����
	//���߲����ݽṹ�����е��߲˻�����Ϣ���뵽lis��
	for (tmpk = head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			addVegToList(list_veg, tmpv, i++, flag);

	list_veg->SortItems(CListCtrlCmp, (DWORD_PTR)list_veg);	//���б�ÿ�����ݰ����߲˱������
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ȫ���߲���ֲ��Ϣ��ӵ��б���ʾ
//��ڲ�����list_plant�б�
//����ֵ����
void showAllPlant(CListCtrl* list_plant)
{
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	
	int i = 0;

	list_plant->DeleteAllItems();	//ɾ���б�ԭ����
	//���߲����ݽṹ�����е��߲���ֲ��Ϣ���뵽lis��
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
	list_plant->SortItems(CListCtrlCmp, (DWORD_PTR)list_plant);//���б�ÿ�����ݰ�����ֲ��Ϣ�������
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ʾĳ�߲�������Ϣ�¶�Ӧ��ȫ���߲˻�����Ϣ���߲���ֲ��Ϣ
//��ڲ�����k�߲�������Ϣ, list_veg�߲˻�����Ϣ�б�, list_plant�߲���ֲ��Ϣ�б�
//����ֵ��
void showVegPlantOfKind(Kind* k, CListCtrl* list_veg, CListCtrl* list_plant)
{
	Veg* tmpv = k->k_vptr->v_next;
	Plant* tmpp;
	list_veg->DeleteAllItems();
	list_plant->DeleteAllItems();
	for (int i = 0, j = 0; tmpv; tmpv = tmpv->v_next, ++i)	//�����߲˻�����Ϣ����
	{
		addVegToList(list_veg, tmpv, i, 0);		//���߲˻�����Ϣ��ӵ��б�
		for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next)
			addPlantToList(list_plant, tmpp, j++);		//����ֲ��Ϣ��ӵ��б�
	}
	list_plant->SortItems(CListCtrlCmp, (DWORD_PTR)list_plant);	//����ֲ��Ϣ�б�ÿ�����ݰ�����ֲ��Ϣ�������
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ������߲�������Ϣ������
//��ڲ�����cmb�߲�������Ϣ������
//����ֵ����
void updateKindCmb(CComboBox* cmb)
{
	cmb->ResetContent();	//���������
	
	Kind* tmpk;
	CString str;
	//�����߲�����������ӵ�������
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next)
	{
		str.Format(L"%c  %s", tmpk->k_num, tmpk->k_name);
		cmb->AddString(str);
	}
	if (cmb->GetCount())
		cmb->SetCurSel(0);	//����Ĭ��ѡ��
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ������߲˻�����Ϣ������
//��ڲ�����cmb�߲˻�����Ϣ������
//����ֵ����
void updateVegCmb(CComboBox* cmb)
{
	cmb->ResetContent();		//���������
	
	Kind* tmpk;
	Veg* tmpv;
	CString str;
	int cmbCnt = cmb->GetCount();	//��������ѡ����
	int i;
	//�����߲˻�����Ϣ������ӵ�������
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next, ++cmbCnt)
		{
			//���ʱ�����߲˱���������
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
		cmb->SetCurSel(0);	//����Ĭ��ѡ��
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ������߲���ֲ��Ϣ������
//��ڲ�����cmb�߲���ֲ��Ϣ������
//����ֵ����
void updatPlantCmb(CComboBox* cmb)
{
	cmb->ResetContent();		//���������
	
	Kind* tmpk;
	Veg* tmpv;
	Plant* tmpp;
	CString str;
	int cmbCnt = cmb->GetCount();	//��������ѡ����
	int i;
	//���������߲���ֲ��Ϣ������ӵ�������
	for (tmpk =head->k_next; tmpk; tmpk = tmpk->k_next)
		for (tmpv = tmpk->k_vptr->v_next; tmpv; tmpv = tmpv->v_next)
			for (tmpp = tmpv->v_pptr->p_next; tmpp; tmpp = tmpp->p_next, ++cmbCnt)
			{
				//���ʱ������ֲ��Ϣ����������
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
		cmb->SetCurSel(0);	//����Ĭ��ѡ��
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�����������ѡ��ȷ����Ӧ���߲˻�����Ϣ���
//��ڲ�����cmb������, dest��Ӧ�߲˻�����Ϣ��ָ��
//����ֵ�����߲˻�����Ϣ�ķ���������������Ĵ���, -1��ʾδ�ҵ�
int OnCbnChangeVeg(CComboBox* cmb, Veg* dest)
{
	int idx;
	CString str;
	if (cmb->GetCurSel() == CB_ERR)	//��������Ϊ���򷵻�
		return -1;
	cmb->GetLBText(cmb->GetCurSel(), str);	//��ȡ������ǰѡ���ֵ
	int num = _wtoi(str);
	
	Kind* tmpk;
	Veg* tmpv;
	//��������ʮ�������ҵ���Ӧ���߲˻�����Ϣ���
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
//�������ܣ����ձ��˳��,�����б�����ıȽϺ���,����������
//��ڲ�����lParam1,lParam2���Ƚϵ�������, lParamSort�����ʶ
//����ֵ���Ƚϴ�С�Ľ��
int CALLBACK CListCtrlCmp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return lParam1 > lParam2;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ�������ݻ���˳��, �����б�����ıȽϺ���,���������� 
//��ڲ�����lParam1,lParam2���Ƚϵ�������, lParamSort�����ʶ
//����ֵ���Ƚϴ�С�Ľ��
int CALLBACK PlantCmpByYear(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (lParam1 % 100 == lParam2 % 100)
		return lParam1 > lParam2;
	return lParam1 % 100 > lParam2 % 100;
}


//////////////////////////////////////////////////////////////////////////
//�������ܣ���ȡ�ļ�·��
//��ڲ�����foderPath���ڴ���ļ�·��, m_hWnd���ھ��
//����ֵ����
void getFolderPath(CString* foderPath, HWND m_hWnd)
{
	wchar_t szPath[MAX_PATH] = { 0 };     //��ʱ���ѡ���Ŀ¼·�� 
	CString str;

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = L"��ѡ�������ļ�����Ŀ¼��";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
		*foderPath = szPath;
	CoTaskMemFree(lp);
}