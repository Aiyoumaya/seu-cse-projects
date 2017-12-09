#ifndef LEXFILEREADER_H
#define LEXFILEREADER_H
#include "headers.h"

//Lex�����ļ��Ľ���
class FileReader
{
public:
	FileReader(string fileName);
	~FileReader();
public:
	//����C���Զ��岿�ִ���
	vector<string> getDefPart();
	//�������涨���
	map<string,string> getRegDefPart();
	//����������ʽ
	vector<string> getRegularExpression();
	//����������ʽ���Ӧ����
	vector<string> getCode();
	//�����û�����������
	vector<string> getLastPart();
private:
	// Cminus.l �ļ���
	ifstream fcin;
	//C���Զ��岿�ֵ�����
	int definePartNum;
	// ������ʽ������
	int regularExpressionNum;
	// �û������̶εĴ��������
	int lastPartNum;
};

#endif
