#include "TransTable.h"
Table::Table(int pNumRows) {
	//��ʼ��״̬ת����Ϊ��
	numRows = pNumRows;
	tableItems = vector<vector<hash_set<int>*>> (numRows,vector<hash_set<int>*>(NUM_OF_COLUMNS,NULL));
}
Table::~Table()
{
}
//��λһ��ת������
void Table::set(int row, char column, hash_set<int> *items) {
	tableItems[row][column] = items;
}
//��ȡ״̬ row ���շ���column��ת�Ƶ���״̬����
hash_set<int>* Table::get(int row, char column) 
{
	return tableItems[row][column];
}
//����һ��ת������
void Table::add(int row, int column, int item)
{
	if(tableItems[row][column] == NULL)
		tableItems[row][column] = new hash_set<int>();
	tableItems[row][column]->insert( item );
}
//ǳ����һ�б���
void Table::copyLineByRef(Table *from, int fromLine, int toLine) 
{
	tableItems[toLine] = from->tableItems[fromLine];
}

//���ر�����
int Table::getNumRows() 
{
	return numRows;
}