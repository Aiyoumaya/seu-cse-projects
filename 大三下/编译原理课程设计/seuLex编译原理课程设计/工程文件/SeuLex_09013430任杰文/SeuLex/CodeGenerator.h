#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H
#include "headers.h"

//����DFA ��״̬ת�Ʊ�����������
class CodeGenerator
{
public:
	//���ļ���������ͷ�ļ�
	CodeGenerator(string fileName);
	~CodeGenerator();
	//���ݹ���������������,Ĭ�����״̬Ϊ0
	void genDriver( vector<string> rules,int enterState = 0 );
	//����άvectorת��Ϊ��ά��������
	void genTable(vector<vector<int> > t,string);
	//��vectorת��Ϊ��������
	void genVector(vector<int> v,string);
	//д�ļ�������
	void writeln(string);
	//д�ļ�
	void write(string);

private:
	fstream File;
};

#endif