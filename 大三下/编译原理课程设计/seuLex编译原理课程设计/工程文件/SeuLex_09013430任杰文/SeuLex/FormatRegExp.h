#ifndef INFIXTOPOSTFIX_H
#define INFIXTOPOSTFIX_H
#include "headers.h"

//��׼��������ʽ
class FormatRegExp
{
public:
	FormatRegExp( const map<string,string> &RDP );
	~FormatRegExp();
	//��������ʽ��׺ת��׺
	string postfix( string regExpr );
private:
	//Ԥ��������˫���ţ������ţ������ţ�ͨ�����ת���ַ���"{}[].\    ;  ���������תΪ���룺ֻ���������ʾ�� ����+*��| & 
	string preProcess(string s);
	//���������ţ�����������������  |  ������������������� ������ʾ��������
	string proBrackets( string s );
	//����ת���ַ�
	char escape( char d );
	//һԪ�����
	bool isUniOp(char c);
	//��Ԫ�����
	bool isBinOp(char c);
private:
	//cminus.l ���涨��
	map<string,string> regDefPart;
};
#endif