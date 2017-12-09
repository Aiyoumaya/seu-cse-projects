#include "CodeGenerator.h"

//���ļ���������ͷ�ļ�
CodeGenerator::CodeGenerator(string fileName)
{
	//���ļ���д��Ҫ������ͷ�ļ�
	File.open(fileName, fstream::out | ios_base::trunc);
	File<< "#include <"<< "iostream" <<  ">\n" ;
	File<< "#include <"<< "deque" <<  ">\n" ;
	File<< "#include <"<< "cassert" <<  ">\n" ;
	File<< "#include <"<< "string" <<  ">\n" ;
}
	CodeGenerator::~CodeGenerator()
	{		
		File.close();
	}

//���ݹ���������������
void CodeGenerator::genDriver(vector<string> rules, int enterState)
{
	writeln("string seuLexLastLex;");
	writeln("int seuLex(std::istream& cin = std::cin, std::ostream& cout = std::cout)");
	writeln("{");
	writeln("while ( true )");
	writeln("{");
	File<< "int currentState = " <<enterState<<";\n";
	writeln("int matchedState = 0;");
	writeln("int currentLength = 0;");
	writeln("int matchedLength = 0;");
	writeln("seuLexLastLex.clear();");
	writeln("char c;");
	writeln("std::deque<char> q;");
	writeln("while ( currentState!=-1 && cin.get(c) )");
	writeln("{");
	writeln("q.push_back(c);");
	writeln("currentLength++;");
	writeln("currentState = TABLE[currentState][c];");
	writeln("if ( STATE_PATTERN[currentState] != -1 )");
	writeln("{");
	writeln("matchedState = currentState;");
	writeln("matchedLength = currentLength;");
	writeln("}");
	writeln("}");
	writeln("if ( matchedLength>0 )");
	writeln("{");
	writeln("while ( currentLength>matchedLength )");
	writeln("{");
	writeln("cin.putback(q.back());");
	writeln("q.pop_back();");
	writeln("currentLength--;");
	writeln("}");
	writeln("while ( !q.empty() )");
	writeln("{");
	writeln("seuLexLastLex += q.front();");
	writeln("q.pop_front();");
	writeln("}");
	writeln("switch ( STATE_PATTERN[matchedState] )");
	writeln("{");

	for (int i = 0; i < rules.size(); i++)
		File<<"case "<< i << ":\n" << rules[i] << "\nbreak;\n";

	writeln("default:");
	writeln("assert(false);");
	//writeln("}");
	writeln("}");
	writeln("}");
	writeln("else ");
	writeln("{");
	writeln("return -1;");
	writeln("}");
	writeln("}");
	writeln("return 0;");
	writeln("}");
}

//����άvectorת��Ϊ��ά��������
void CodeGenerator::genTable( vector<vector<int> > t, string name)
{
	File<<"const int "<<name<<"[" <<t.size()<< "][" << t[0].size() << "] = {\n";
	for ( int i=0; i<t.size(); i++ )
	{
		File<<'\t';
		for ( int j = 0; j < t[i].size(); j++ )
			File<<t[i][j]<<',';
		File<<'\n';
	}
	File<<"};\n";
}
//��vectorת��Ϊ��������
void CodeGenerator::genVector(vector<int> v,string name)
{
	File<<"const int "<<name<<"[" <<v.size()<< "] = {\n";
	for ( int i=0; i<v.size(); i++ )
		File<<v[i]<<",";
	File<<"};\n";
}
//д�ļ�������
void CodeGenerator::writeln(string str)
{
	write(str+"\n");
}
//д�ļ�
void CodeGenerator::write(string str)
{
	File<<str;
}