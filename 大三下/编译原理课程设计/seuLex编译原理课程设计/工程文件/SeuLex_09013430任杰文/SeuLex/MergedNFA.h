#ifndef MERGEDNFA_H
#define MERGEDNFA_H
#include "headers.h"
#include "NFA.h"

//������NFA
class MergedNFA :public NFA 
{
public:
	//�ϲ�NFA
	MergedNFA ( const vector<NFA*> &nfas );
	//���ص�ǰNFA״̬ʶ���Token��ţ��ͱ�ž��и����ȼ�
	int statePatten(int s);
private:
	//ʶ��ģʽToken i ������״̬�ļ���
	vector<int> finalStates;
};
#endif