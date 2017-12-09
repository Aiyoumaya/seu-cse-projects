#ifndef TRANSTABLE_H
#define TRANSTABLE_H
#include "headers.h"
//NFA״̬ת����
class Table 
{
public:
	Table(int pNumRows);
	~Table();
	//��λһ��ת������
	void set(int row, char column, hash_set<int> *items);
	//��ȡ״̬ row ���շ���column��ת�Ƶ���״̬����
	hash_set<int>* get(int row, char column);
	//����һ��ת������
	void add(int row, int column, int item);
	//ǳ����һ�б���
	void copyLineByRef(Table *from, int fromLine, int toLine);
	//���ر�����
	int getNumRows();

private:
	//״̬ת����������״̬����
	int numRows;
	//�洢hash_set<int>ָ��Ķ�ά����
	vector<vector<hash_set<int>*> > tableItems; 
};
#endif