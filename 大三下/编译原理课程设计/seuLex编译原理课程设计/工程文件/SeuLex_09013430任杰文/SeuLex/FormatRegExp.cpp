#include "FormatRegExp.h"

FormatRegExp::FormatRegExp( const map<string,string> &RDP )
{
	regDefPart = RDP;
}
FormatRegExp::~FormatRegExp()
{
}
//��������ʽ��׺ת��׺
string FormatRegExp::postfix( string regExpr )
{
	//���غ�׺���ʽ
	string postFixExpr = "";
	//ת����ջ
	stack<char> opStack;
	//Ԥ��������˫���ţ������ţ������ţ�ͨ�����ת���ַ������������С����תΪ����
	regExpr = preProcess(regExpr);
	for ( int i = 0; i < regExpr.length(); i++) 
	{
		char c = regExpr[ i ];
		if ( c == LEFT_LITTLE_BRACKET ) {
			opStack.push(LEFT_LITTLE_BRACKET);
		} else if ( c== STAR ) {
			postFixExpr += STAR;
		} else if ( c== ONE_OR_MORE) {
			postFixExpr += ONE_OR_MORE;
		} else if ( c== ONE_OR_NONE ) {
			postFixExpr += ONE_OR_NONE;
		} else if ( c== RIGHT_LITTLE_BRACKET ) {
			while ( opStack.top() !=  LEFT_LITTLE_BRACKET ) {
				postFixExpr += opStack.top();
				opStack.pop();
			}
			opStack.pop();
		} else if ( c == OR ) {
			while ( !opStack.empty() && opStack.top() != LEFT_LITTLE_BRACKET ) {
				postFixExpr += opStack.top();
				opStack.pop();
			}
			opStack.push(OR);
		}else if ( c == AND ) {
			while (  !opStack.empty() && opStack.top() == AND  ) {
				postFixExpr += opStack.top();
				opStack.pop();
			}
			opStack.push(AND);
		}else {
			postFixExpr += c;
		}
	}
	//ȫ����ջ
	while ( !opStack.empty() )
	{
		postFixExpr += opStack.top();
		opStack.pop();
	}
	return postFixExpr;
}
//Ԥ��������˫���ţ������ţ������ţ�ͨ�����ת���ַ����������תΪ����
string FormatRegExp::preProcess(string s) 
{
	//Ԥ�������ַ���
	string res = "";
	//��ǰɨ���ַ���
	char cur = 0;
	//�����ַ���
	for ( int i = 0; i < s.length();  i++ ) 
	{
		cur = s[i];
		//��ǰΪ �������� �� 
		if ( cur == '[' )
		{
			string bs = "";
			while ( s[++i]!=']' ) {
				if( s[i] == '\\')
					bs += escape( s[ ++i ]);
				else
					bs += s[i];
			}
			res += proBrackets(bs);
		}
		//ͨ�������ַ�
		else if ( cur == '.' )
		{
			char c1 = 1;
			char c2 = 127;
			string s = "-";
			s = c1 + s;
			s = s + c2;
			res += proBrackets(s);
		} 
		//��ǰΪת���ַ���ֱ�Ӵ���Ϊ�����ַ�
		else if ( cur == '\\' )
		{
			res +=  escape( s[ ++i ]);
		} 
		//Ϊ�����ţ��������涨��
		else if ( cur == '{' ) 
		{
			string regDef = "";
			while ( s[++i] != '}'  ){
					regDef += s[i];
			}
			//�����涨�崦��ɼ�����ʽ
			res += proBrackets( regDefPart[regDef].substr(1, regDefPart[regDef].size() - 2) );
		} 
		//˫���������ݰ����ַ�����
		else if ( cur == '"') 
		{
			while ( s[++i] !='"' )
				res += s[i];
		}else if ( cur==  '*' ) {
			res += STAR;
		} else if ( cur== '+' ) {
			res += ONE_OR_MORE;
		} else if ( cur== '?' ) {
			res += ONE_OR_NONE;
		} else if ( cur == '|'){
			res += OR;
		}else if( cur == '('){
			res += LEFT_LITTLE_BRACKET;
		}else if( cur == ')'){
			res += RIGHT_LITTLE_BRACKET;
		}
		//�����ַ�
		else {
			res += cur;
		}
	}

	string addAND;
	for( int i = 0; i < res.size() ; i++)
	{
		addAND += res[i];
		//������������
		if ( i+1 < res.length() ) {
			char d = res[i+1];
			//��ǰ�ַ����Ƕ�Ԫ������������ţ���һ���ַ������������������
			if (  !isBinOp(res[i])  && res[i]!=LEFT_LITTLE_BRACKET 
				&& d!=RIGHT_LITTLE_BRACKET && !isUniOp(d) && !isBinOp(d))
				addAND += AND;
		}
	}
	res = addAND;
	return res;
}
//���������ţ�����������������  |  ������������������� ������ʾ��������
string FormatRegExp::proBrackets( string s )
{
	//���ش�
	string res = "";
	res += LEFT_LITTLE_BRACKET;
	//��һ��ɨ���ַ�
	char last = 0;
	//��ǰɨ���ַ�
	char cur;

	//�����ַ���
	for ( int i=0; i<s.length(); i++ )
	{
		cur = s[i];
		//����� - ���ӱ�ʾ
		if ( cur == '-' )
		{
			cur = s[i+1];
			i++;
			//����һ���ַ�����ǰ�ַ�
			for ( int x = last+1; x <= cur; x++ )
			{
				////����������������
				res += x;
				//����������һ���ַ�
				if ( x < cur ) 
					res += OR;
			}
		} 
		//����������ַ�
		else 
		{
			res += cur;
			last = cur;
		}
		//�û����������
		if ( i+1<s.length() ) {
			res += OR;
		}
	}
	//����
	res += RIGHT_LITTLE_BRACKET;
	return res;
}
//����ת���ַ�
char FormatRegExp::escape( char d ) {
	if ( d == 'n' ) {
		return '\n';
	} else if ( d=='r' ) {
		return '\r';
	} else if ( d == 't' ) {
		return '\t';
	} else {
		return d;
	}
}
//һԪ�����
bool FormatRegExp::isUniOp(char c) {
	if ( c == STAR)
		return true;
	else if ( c ==ONE_OR_MORE )
		return true;
	else if ( c ==ONE_OR_NONE )
		return true;
	else
		return false;
}
//��Ԫ�����
bool FormatRegExp::isBinOp(char c) {
	if ( c == OR)
		return true;
	else 
		return false;
}