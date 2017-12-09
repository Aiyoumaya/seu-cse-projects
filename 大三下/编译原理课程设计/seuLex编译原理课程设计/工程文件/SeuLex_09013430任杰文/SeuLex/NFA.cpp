#include "NFA.h"

//���캯��
NFA::NFA() {
}
NFA::NFA( int _numState ) {
	numStateOfNFA = _numState;
	transTabOfNFA = new Table(numStateOfNFA);
}
NFA::NFA( string regExpr , const map<string,string> &regDefPart) {
	//��׺ת��׺,��������header�е������滻
	FormatRegExp *ipt = new FormatRegExp(regDefPart);
	string postReg = ipt->postfix( regExpr );

	//������׺���ʽ������NFA
	stack<NFA* > NFAStack;
	NFA* curNFA_1;
	NFA *curNFA_2;
	char curSymbol;
	for ( int i=0; i < postReg.length(); i++ ) 
	{
		curSymbol = postReg[i];
		//�����ǰΪ������� ���ӡ��򡢱հ������հ�����գ������ջNFA��������������ջ
		//�����ǰΪ���ַ�������һ��ֻ��ʼ̬����̬��NFA������ջ
		if ( curSymbol == AND ) {
			curNFA_1 = NFAStack.top();
			NFAStack.pop();
			curNFA_2 = NFAStack.top();
			NFAStack.pop();
			NFAStack.push( and(curNFA_2, curNFA_1) );
		} else if ( curSymbol == OR ) {
			curNFA_1 = NFAStack.top();
			NFAStack.pop();
			curNFA_2 = NFAStack.top();
			NFAStack.pop();
			NFAStack.push(or(curNFA_2,curNFA_1));
		} else if ( curSymbol == STAR ) {
			curNFA_1 = NFAStack.top();
			NFAStack.pop();
			NFAStack.push( star(curNFA_1) );
		} else if ( curSymbol == ONE_OR_MORE ){
			curNFA_1 = NFAStack.top();
			NFAStack.pop();
			NFAStack.push( oneMore(curNFA_1) );
		} else if ( curSymbol == ONE_OR_NONE ){
			curNFA_1 = NFAStack.top();
			NFAStack.pop();
			NFAStack.push(oneNone(curNFA_1));
		}else {
			curNFA_1 = new NFA(2);
			curNFA_1->transTabOfNFA->add(0, curSymbol, 1);
			NFAStack.push(curNFA_1);
		}
	}
	//���ս��NFA
	curNFA_1 = NFAStack.top();
	NFAStack.pop();
	//NFAת����״̬��
	transTabOfNFA = curNFA_1->transTabOfNFA;
	numStateOfNFA = transTabOfNFA->getNumRows();
}

//NFA����
NFA* NFA::or(NFA *m, NFA *n) 
{
	//���ʼ̬����̬
	NFA *res = new NFA( m->numStateOfNFA + n->numStateOfNFA + 2 );
	//���¸�״̬��ţ�������״̬ת�Ʊ�
	m->shift(1);
	n->shift( m->numStateOfNFA+1 );
	for ( int i=0; i<m->numStateOfNFA; i++ ){
		res->transTabOfNFA->copyLineByRef(m->transTabOfNFA, i, i+1);
	}
	for ( int i=0; i<n->numStateOfNFA; i++) {
		res->transTabOfNFA->copyLineByRef(n->transTabOfNFA, i, m->numStateOfNFA+1+i);
	}
	//���EPSILON��
	res->transTabOfNFA->add(0, EPSILON, 1);
	res->transTabOfNFA->add(0, EPSILON, m->numStateOfNFA+1);
	res->transTabOfNFA->add(m->numStateOfNFA, EPSILON, res->numStateOfNFA-1);
	res->transTabOfNFA->add(res->numStateOfNFA-2, EPSILON, res->numStateOfNFA-1);
	return res;
}

NFA* NFA::and(NFA *m, NFA *n) 
{
	//�ϲ�m��̬��nʼ̬
	NFA *res = new NFA(m->numStateOfNFA+n->numStateOfNFA-1);
	//���¸�״̬��ţ�������״̬ת�Ʊ�
	n->shift(m->numStateOfNFA-1);
	for ( int i=0; i<m->numStateOfNFA; i++ ) {
		res->transTabOfNFA->copyLineByRef(m->transTabOfNFA, i, i);
	}
	for ( int i=0; i<n->numStateOfNFA; i++) {
		res->transTabOfNFA->copyLineByRef(n->transTabOfNFA, i, m->numStateOfNFA+i-1);
	}
	return res;
}

NFA* NFA::star(NFA *m) 
{
	//���ʼ̬����̬
	NFA *res = new NFA(m->numStateOfNFA+2);
	//���¸�״̬��ţ�������״̬ת�Ʊ�
	m->shift(1);
	for ( int i=0; i<m->numStateOfNFA; i++ ){
		res->transTabOfNFA->copyLineByRef(m->transTabOfNFA, i, i+1);
	}
	//���EPSILON��
	res->transTabOfNFA->add(0, EPSILON, 1);
	res->transTabOfNFA->add(0, EPSILON, res->numStateOfNFA-1);
	res->transTabOfNFA->add(res->numStateOfNFA-2, EPSILON, 1);
	res->transTabOfNFA->add(res->numStateOfNFA-2, EPSILON, res->numStateOfNFA-1);
	return res;
}

NFA* NFA::oneMore(NFA *m) 
{
	//���ʼ̬����̬
	NFA *res = new NFA(m->numStateOfNFA+2);
	//���¸�״̬��ţ�������״̬ת�Ʊ�
	m->shift(1);
	for ( int i=0; i<m->numStateOfNFA; i++ ){
		res->transTabOfNFA->copyLineByRef(m->transTabOfNFA, i, i+1);
	}
	//���EPSILON��
	res->transTabOfNFA->add(0, EPSILON, 1);
	res->transTabOfNFA->add(res->numStateOfNFA-2, EPSILON, 1);
	res->transTabOfNFA->add(res->numStateOfNFA-2, EPSILON, res->numStateOfNFA-1);
	return res;
}

NFA* NFA::oneNone(NFA *m) {
	//clock_t t1 = clock();

	m->transTabOfNFA->add(0, EPSILON, m->numStateOfNFA-1);
	//clock_t t2 = clock();
	//cout<<"onenone"<<t2-t1<<endl;
	return m;
}
//��NFA״̬ת������״̬���ȫ���� s
void NFA::shift(int s) {
	//��������״̬
	for ( int i = 0; i < numStateOfNFA; i++ ) {
		//�������з���
		for ( int symb = 0; symb < NUM_OF_COLUMNS; symb++ ) {
			//��ȡ״̬ i ���շ���symb��ת�Ƶ���״̬����
			hash_set<int>* items = transTabOfNFA->get( i,symb );
			if ( items == NULL) {
				continue;
			}
			//��ԭ״̬������״̬���ȫ���� s 
			hash_set<int> *newItems = new hash_set<int>();
			hash_set<int>::iterator iter = items->begin();
			while(iter != items->end()){
				newItems->insert( *iter + s );
				iter++;
			}
			delete items;
			//����״̬ת����� i+1 �е� symb+1��
			transTabOfNFA->set(i, symb, newItems);
		}
	}
}
//����NFA״̬ת����
Table* NFA::getTrsTbl() 
{
	return transTabOfNFA;
}

//��ȡ״̬state ���շ���symbol��ת�Ƶ���״̬����
hash_set<int>* NFA::move( int state, char symbol ) 
{
	return transTabOfNFA->get(state, symbol);
}