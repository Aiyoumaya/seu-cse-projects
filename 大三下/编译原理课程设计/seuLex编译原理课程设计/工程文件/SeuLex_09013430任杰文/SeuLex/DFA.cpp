#include "DFA.h"

//��NFA����DFA
DFA::DFA( MergedNFA *pnfa) {
	//��������NFA
	pMergedNFA = pnfa;
	//��ʼ״̬Ĭ��Ϊ0
	enterState = 0;

	//���壺DFA�հ���DFA�հ���š��հ�֮���ת��ӳ���ϵ������vectorΪ128ά����Ӧ128��ASCLL��
	list<hash_set<int>* > UnmarkedDstates;
	vector<hash_set<int> > Dstates;
	hash_map<hash_set<int>*, vector<hash_set<int>* >* > Dtran;

	//��ʼ������EPSILON�հ���ţ���ΪDFA��״̬�����DFA״̬����ӱհ���ӳ���ϵ
	Dstates.push_back( *epsilonClosure(0) );
	UnmarkedDstates.push_front( epsilonClosure(0) );
	Dtran[ epsilonClosure(0) ] = new vector<hash_set<int>* >(NUM_OF_COLUMNS,NULL);

	//�������µ�DFA״̬ʱ��������������DFA״̬��Ϊÿһ��DFA״̬���ת�����������ѳ��ֵ���״̬����DFA�����У�
	hash_set<int>* curDFAState;
	while ( !UnmarkedDstates.empty() ) {
		curDFAState = UnmarkedDstates.front();
		UnmarkedDstates.erase( UnmarkedDstates.begin() );

		//�ѳ��ֵ���״̬����DFA�����У�
		hash_set<int> *U;
		for ( int symbol = 1; symbol < NUM_OF_COLUMNS; symbol++ ) 
		{
			//��ǰת����EPSILON�հ�
			U = epsilonClosure( move( curDFAState, symbol ) );
			if (  U->empty() )
				continue;
			//�����µ�EPSILON�հ���ΪDFA״̬
			if ( find( Dstates.begin(), Dstates.end(), *U) == Dstates.end() ) {
				//��š������µ�DFA״̬����ӱհ���ӳ���ϵ
				Dstates.push_back( *U );
				UnmarkedDstates.push_front(U);
				vector<hash_set<int>* >* v = new vector<hash_set<int>* >(NUM_OF_COLUMNS,NULL);
				Dtran[U] = v;
			}
			//��ӱհ���ӳ���ϵ
			(*(Dtran[curDFAState]))[symbol] = U;
		}		
	}

	//DFA״̬������ʼ��״̬ת������ʼ��ʶ��TOKEN��
	numStatesOfDFA = Dstates.size();
	transTabOfDFA = new vector<vector<int> >( numStatesOfDFA, vector<int>(NUM_OF_COLUMNS,-1) );
	finalStatePatten = new vector<int>(numStatesOfDFA,-1);

	//��������DFAState
	hash_map<hash_set<int>*, vector<hash_set<int>* >* >::iterator it = Dtran.begin();
	hash_set<int>* curState;
	while( it != Dtran.end() )
	{
		curState = (it++)->first;
		int tagOfCurState =  find(Dstates.begin(), Dstates.end(), *curState)-Dstates.begin();
		//��ǵ�ǰDFA״̬��ʶ���token
		(*finalStatePatten)[tagOfCurState] = statePatten( curState );
		(*finalStatePatten)[tagOfCurState] = statePatten( curState );

		//����״̬ת����
		vector<hash_set<int>* >* TransOfCurState = Dtran[curState];
		for ( int symbol = 0; symbol < NUM_OF_COLUMNS; symbol++ )
		{
			if ( (*TransOfCurState)[symbol] != NULL &&  !(*TransOfCurState)[symbol]->empty() )
				(*transTabOfDFA)[tagOfCurState][symbol] = find(Dstates.begin(), Dstates.end(), *(*TransOfCurState)[symbol])-Dstates.begin();
		}
	}
}
//���ص�ǰDFA״̬ʶ��Token��ţ��ͱ�ž��и����ȼ�
int DFA::statePatten( hash_set<int>* curStateOfDFA )
{ 
	int minState = pMergedNFA->numStateOfNFA;
	int pid = -1;
	//������DFA״̬�����е�NFA״̬
	hash_set<int>::iterator it = curStateOfDFA->begin();
	while ( it != curStateOfDFA->end() )
	{
		int currentState = *(it++);
		//��ǰ״̬ʶ��token���
		int curpid = pMergedNFA->statePatten(currentState);
		if ( curpid > -1 && currentState < minState ) {
			pid = curpid;
			minState = currentState;
		}
	}
	return pid;
}
//���ؼ���T,����a��ת��״̬��
hash_set<int>* DFA::move(hash_set<int>* T, char a) 
{
	hash_set<int>* res = new hash_set<int>();
	hash_set<int>::iterator it = T->begin();
	while ( it != T->end() ) 
	{
		hash_set<int>* t = pMergedNFA->move(*it, a);
		it++;
		if( t == NULL)
			continue;
		res->insert(t->begin(), t->end());
	}
	return res;
}
//����״̬ i ��EPSILON�հ�
hash_set<int>* DFA::epsilonClosure ( int i ) 
{
	//����� i ״̬��EPSILON�հ���ֱ�ӷ���
	if ( espClsTbl.count(i) ) 
		return (espClsTbl)[i];

	//������ȱ�������EPSILON�հ�
	hash_set<int>* epsCls = new hash_set<int>();
	epsCls->insert(i);
	stack<int> s;
	s.push(i);
	while ( !s.empty() ) 
	{
		int t = s.top();
		s.pop();

		//��ǰ״̬��EPSILONת��װ̬
		hash_set<int> *epsTrs = pMergedNFA->move(t, EPSILON);
		if(epsTrs == NULL)
			continue;
		//����ǰ״̬��EPSILONת��״̬��������EPSILON�հ�
		hash_set<int>::iterator itEspTrs = epsTrs->begin();
		while ( itEspTrs != epsTrs->end() ) 
		{
			int u = *itEspTrs;
			itEspTrs++;
			//�жϵ�ǰ״̬�Ƿ��ѱ�����
			if ( !epsCls->count(u) )
				s.push(u);
			epsCls->insert(u);
		}
	}
	espClsTbl[i] = epsCls;

	return epsCls;
}
//����һ��NFA״̬���ϵ�EPSILON�հ�
hash_set<int>* DFA::epsilonClosure( hash_set<int>* curSetOfNFAStates ) 
{
	hash_set<int>* res = curSetOfNFAStates;
	//������ǰ״̬������ÿ��״̬��EPSILON�հ����뷵�ؽ����
	hash_set<int>::iterator i = curSetOfNFAStates->begin();
	while ( i != curSetOfNFAStates->end() ) 
	{
		hash_set<int>* t = epsilonClosure( *i );
		res->insert(t->begin(), t->end());
		i++;
	}
	return res;
}
//����DFA״̬��
int DFA::getNumStates() {
	return numStatesOfDFA;
}
//����״̬ת����
vector<vector<int > >* DFA::getTable() {
	return transTabOfDFA;
}
//����DFAʶ��ģʽ��
vector<int >* DFA::getStatePattern() {
	return finalStatePatten;
}
//DFA��С��
void DFA::minimize() {
	//����DFA��ʶ��Token��
	int numOfToken = -1;
	for ( int i = 0; i<numStatesOfDFA; i++ ) {
		if ( (*finalStatePatten)[i] > numOfToken ) {
			numOfToken =  (*finalStatePatten)[i];
		}
	}
	numOfToken++;

	//���壺��С��DFA״̬���ϼ����š�ԭDFA״̬ӳ�䵽��С��DFA��ĳ��״̬��
	vector<set<int>* >* statesOfMinimizedDFA = new vector<set<int>* >();
	map< int , int > oriStateToMinimizedState;

	//��ʼ������ʼ������̬����̬����
	set<int>* nonfinalStates = new set<int>();
	vector<set<int>* >* finalStates = new vector<set<int>* >(numOfToken, NULL);
	for( int i = 0; i < finalStates->size(); i++)
		(*finalStates)[i] = new set<int>();
	for ( int curState = 0; curState < numStatesOfDFA; curState++ ){
		if ( (*finalStatePatten)[curState] < 0 ) 
		{
			//����̬
			nonfinalStates->insert( curState );
		} else{
			//��ÿһ����̬����ʶ��Token��ͬ���ֵ�һ��state����
			(*finalStates)[ (*finalStatePatten)[curState] ]->insert(curState);
		}
	}

	//������С��DFA��ʼ״̬��������ʼ���¾�״̬ӳ���
	if ( !nonfinalStates->empty()) {
		statesOfMinimizedDFA->push_back(nonfinalStates);
		for(set<int>::iterator it = nonfinalStates->begin(); it != nonfinalStates->end();  it++)
			oriStateToMinimizedState[ *it ] = statesOfMinimizedDFA->size() - 1;
	}
	for ( int i = 0; i < numOfToken; i++ ) {
		if ( !(*finalStates)[i]->empty() ) {
			statesOfMinimizedDFA->push_back( (*finalStates)[i] );
			for(set<int>::iterator it = (*finalStates)[i]->begin();  it != (*finalStates)[i]->end(); it++)
				oriStateToMinimizedState[ *it ] = statesOfMinimizedDFA->size() - 1;
		}
	}
	//�Զ����»���
	while ( partition(statesOfMinimizedDFA,oriStateToMinimizedState) );

	//������״̬ת������״̬��ʶ��Tokenӳ���
	vector<vector<int> >* newTrsTbl = new vector<vector<int> >(statesOfMinimizedDFA->size(), vector<int>(NUM_OF_COLUMNS,-1));
	vector<int>* newStatePattern = new vector<int>(statesOfMinimizedDFA->size(),-1);
	for( vector<set<int>* >::iterator itState = statesOfMinimizedDFA->begin();   itState != statesOfMinimizedDFA->end() ;itState++)
	{
		set<int>::iterator it = (*itState)->begin();
		int oldNum  = *it;
		int newNum = oriStateToMinimizedState[oldNum];
		(*newStatePattern)[newNum] = (*finalStatePatten)[oldNum];
		//�����µ�״̬ת����
		for ( int symb = 0; symb < NUM_OF_COLUMNS; symb++ ) {
			if(  (*transTabOfDFA)[ oldNum ][ symb ]  == -1)
				(*newTrsTbl)[newNum][symb] = -1;
			else
				(*newTrsTbl)[newNum][symb] = oriStateToMinimizedState [ (*transTabOfDFA)[ oldNum ][ symb ] ];
		}
	}
	//����DFA�����ڴ�й©��������
	enterState = oriStateToMinimizedState[0];
	numStatesOfDFA = statesOfMinimizedDFA->size();
	transTabOfDFA = newTrsTbl;
	finalStatePatten = newStatePattern;
}
//���ݳ��߻����Ӽ�
bool DFA::partition( vector<set<int>* >* statesOfMinimizedDFA, map<int,int>& oriStateToMinimizedState) 
{
	//��ǵ�ǰ�����Ƿ����µĻ���
	bool partitioned = false;

	//��������״̬��
	for( int curS = 0; curS < statesOfMinimizedDFA->size(); curS++)
	{
		set<int>* curState = (*statesOfMinimizedDFA)[curS];
		//��ǰ״̬�������ٻ���
		if ( curState->size() <= 1 )
			continue;
		//������ǰ״̬��������״̬�����п��ܳ���
		for ( int c = 1; c < NUM_OF_COLUMNS; c++ ) 
		{
			if( c == 127)
				int t = 0;
			//ӳ�䵽ĳ����С��DFA״̬�� �� DFA״̬����
			map< int ,set<int> > partByc;
			for( set<int>::iterator it = curState->begin(); it != curState->end() ; it++)
			{
				//�Ƿ����c
				if ( (*transTabOfDFA)[*it][c] == -1 ){
					partByc[-1].insert(*it);
				}else{
					partByc[  oriStateToMinimizedState[ (*transTabOfDFA)[*it][c] ] ] .insert(*it);
				}
			}
			//����ӳ���ϵ�����µ�DFA����
			if( partByc.size() > 1)
			{
				partitioned = true;
				//�ӵڶ������Ͽ�ʼ�����µ�DFA����
				map< int ,set<int> >::iterator it  = partByc.begin();
				for( it++; it != partByc.end(); it++)
				{
					set<int> *newState = new set<int>();
					for( set<int>::iterator i = (*it).second.begin(); i != (*it).second.end(); i++)
					{
						newState->insert( *i );
						curState->erase( *i );
					}
					//�����µ�DFA����
					statesOfMinimizedDFA->push_back(newState);
					for( set<int>::iterator j = newState->begin();  j != newState->end(); j++)
						oriStateToMinimizedState[ *j ] = statesOfMinimizedDFA->size() - 1;
				}
			}
		}
	}
	return partitioned;
}
//��ȡDFA��ʼ״̬
int DFA::getEnterState()
{
	return enterState;
}