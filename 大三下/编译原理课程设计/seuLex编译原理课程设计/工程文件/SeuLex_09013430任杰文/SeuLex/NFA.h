#ifndef NFA_H
#define NFA_H
#include "headers.h"
#include "TransTable.h"
#include "FormatRegExp.h"



//��ȷ�������Զ���
class NFA 
{
public:
	//���캯��
	NFA();
	NFA( int _numState );
	NFA( string regExpr , const map<string,string> &regDefPart);
	
	//NFA����
	NFA* or(NFA *m, NFA *n);
	NFA* and(NFA *m, NFA *n);
	NFA* star(NFA *m);
	NFA* oneMore(NFA *m);
	NFA* oneNone(NFA *m);

	//��NFA״̬ת������״̬���ȫ���� s
	void shift(int s);
	//����NFA״̬ת����
	Table* getTrsTbl();

	//��ȡ״̬state ���շ���symbol��ת�Ƶ���״̬����
	hash_set<int>* move( int state, char symbol );
public:
	//״̬ת����
	Table *transTabOfNFA;
	//NFA״̬��
	int numStateOfNFA;
};

#endif