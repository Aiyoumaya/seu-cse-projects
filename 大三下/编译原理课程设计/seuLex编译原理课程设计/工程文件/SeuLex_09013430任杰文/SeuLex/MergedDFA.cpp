#include "MergedNFA.h"

MergedNFA::MergedNFA ( const vector<NFA*> &nfas ) {
	//ʶ��ģʽToken i ������״̬�ļ���
	finalStates.resize( nfas.size() );
	//��ǰ�Ѻϲ�״̬�������һ����ʼ״̬��
	numStateOfNFA = 1;

	for ( int i=0; i<nfas.size(); i++ ) {
		//NFA���±��
		nfas[i]->shift( numStateOfNFA );
		//����ϲ�NFA״̬��
		numStateOfNFA += nfas[i]->numStateOfNFA;
		//����ʶ��TOKEN�Ľ���̬����
		finalStates[i] = numStateOfNFA-1;
	}
	//�����µ�״̬ת����
	transTabOfNFA = new Table(numStateOfNFA);
	int curNumOfState = 1;
	for ( int i=0; i<nfas.size(); i++ ) {
		//�������NFA��̬����NFA��̬��EPSILON��
		transTabOfNFA->add(0, EPSILON, curNumOfState);
		//��������ת������
		for ( int j = 0; j < nfas[i]->numStateOfNFA;  j++ ) {
			transTabOfNFA->copyLineByRef ( nfas[i]->transTabOfNFA, j, curNumOfState);
			curNumOfState++;
		}
	}
}
//���ص�ǰNFA״̬ʶ���Token��ţ��ͱ�ž��и����ȼ�
int MergedNFA::statePatten(int s) 
{
	for ( int i = 0; i < finalStates.size(); i++ ) {
		if ( finalStates[i] == s)
			return i;
	}
	return -1;
}

