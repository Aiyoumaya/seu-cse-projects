#ifndef DFA_H
#define DFA_H
#include "headers.h"
#include "MergedNFA.h"

class DFA {
public:
	//��NFA����DFA
	DFA( MergedNFA *pnfa);
	//���ص�ǰDFA״̬ʶ��Token��ţ��ͱ�ž��и����ȼ�
	int statePatten( hash_set<int>* curStateOfDFA );
	//���ؼ���T,����a��ת��״̬��
	hash_set<int>* move(hash_set<int>* T, char a);
	//����״̬ i ��EPSILON�հ�
	hash_set<int>* epsilonClosure ( int i );
	//����һ��NFA״̬���ϵ�EPSILON�հ�
	hash_set<int>* epsilonClosure( hash_set<int>* curSetOfNFAStates );
	//����DFA״̬��
	int getNumStates();
	//����״̬ת����
	vector<vector<int > >* getTable();
	//����DFAʶ��ģʽ��
	vector<int >* getStatePattern();

	//DFA��С��
	void minimize();
	//���ݳ��߻����Ӽ�
	bool partition( vector<set<int>* >* statesOfMinimizedDFA, map<int,int>& oriStateToMinimizedState);
	//��ȡDFA��ʼ״̬
	int getEnterState();
private:
	//��ת��pMergedNFA
	MergedNFA *pMergedNFA;
	//pMergedNFA��״̬����հ���ӳ���
	hash_map<int, hash_set<int>* > espClsTbl;

	
	//DFA״̬��
	int numStatesOfDFA;
	//DFA��ʼ״̬
	int enterState;
	//DFA״̬ת����
	vector<vector<int> >* transTabOfDFA;
	//��ǰ״̬������token���
	vector<int>* finalStatePatten;
};
#endif
