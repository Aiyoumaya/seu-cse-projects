#include "headers.h"
#include "FileReader.h"
#include "FormatRegExp.h"
#include "NFA.h"
#include "MergedNFA.h"
#include "DFA.h"
#include "CodeGenerator.h"

int main()
{
	//��ȡlex�����ļ�
	string lexFile = "Cminus.l";

	//��cminus.l �ļ��ָ�ΪC ���Զ���Σ����涨��Σ�����Σ���������ʽ�������� C �������飬�±��Ӧ�����û��ӳ����
	FileReader lexFileReader(lexFile);
	//���Զ����
	vector<string> defPart = lexFileReader.getDefPart();
	//���涨���
	map<string,string> regDefPart = lexFileReader.getRegDefPart();
	//�����
	vector<string> regularPart = lexFileReader.getRegularExpression();
	vector<string> regularPartCode = lexFileReader.getCode();
	int numOfRegularPart = regularPart.size();
	//�û��ӳ����
	vector<string> demoOfUser = lexFileReader.getLastPart();

	//��ÿ��������ʽ����NFA
	cout<<"��ÿ��������ʽ����NFA��"<<endl;
	vector<NFA*> nfas( numOfRegularPart );
	for ( int i = 0; i <numOfRegularPart;  i++ ) 
	{
		nfas[i] = new NFA(regularPart[i], regDefPart);
		cout<<setw(25)<<left<<regularPart[i].c_str()<<"   ��ӦNFA���У�"<<nfas[i]->numStateOfNFA<<"��״̬��"<<endl;
	}
	//�ϲ�NFA
	cout<<"�ϲ�NFA��"<<endl;
	MergedNFA mn(nfas);
	cout<<"NFA�ϲ���ɣ���ǰNFA�У�"<<mn.numStateOfNFA<<"��״̬"<<endl;

	//����DFA
	cout<<"����NFA��"<<endl;
	DFA dfa(&mn);
	cout<<"����NFA��ɣ���ǰDFA�У�"<<dfa.getNumStates()<<"��״̬"<<endl;
	cout<<"��С��NFA��"<<endl;
	dfa.minimize();
	cout<<"��С��NFA��ɣ���ǰDFA�У�"<<dfa.getNumStates()<<"��״̬"<<endl;

	//����DFA��״̬ת�Ʊ�����������
	cout<<"���ɴʷ��������� yySeuLex.cpp��"<<endl;
	CodeGenerator codeGenerator("yySeuLex.cpp");
	//����C���Զ����
	for( int i = 0; i < defPart.size(); i++)
		codeGenerator.writeln( defPart[i] );
	//����DFA״̬ת����
	codeGenerator.genTable(*dfa.getTable(), "TABLE");
	//����DFAʶ��token���ұ�
	codeGenerator.genVector(*dfa.getStatePattern(), "STATE_PATTERN");
	//������������
	codeGenerator.genDriver(regularPartCode,dfa.getEnterState());
	//�����û������̶�
	for(int i=0; i<demoOfUser.size(); i++)
		codeGenerator.writeln(demoOfUser[i]);
	cout<<"�ʷ��������� yySeuLex.cpp���ɳɹ���"<<endl;

	system("pause");
	return 0;
}