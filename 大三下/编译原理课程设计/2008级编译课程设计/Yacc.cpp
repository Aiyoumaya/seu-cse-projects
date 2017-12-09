/////////////////////////////////////////////////////////
//����ԭ��ʵ�顪���ڶ�����:YACC                        //
//����:�                                            //
//ʱ��:2006��4~5��                                     //
/////////////////////////////////////////////////////////
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<set>
#include<vector>
#include<stack>
#include<queue>
#include<hash_set>
#include<hash_map>
#include<map>
#include<set>
#include<conio.h>
#define ACCEPT 0
#define ERROR 40000
#define EPSLONG 39999
#define SEGMENT_DELIM 39998
#define DEF_SEG_BEGIN 39997
#define DEF_SEG_END 39996
#define TOKEN 39995
#define TYPE 39994
#define UNION 39993
#define LEFT 39992
#define TERMINATE 39991
#define NOMATCH 39000
#define RIGHT 38999
#define SEGMENT_REG 38998
#define SELF_DEF 38997
#define NONTERMINS 35000
#define TERMINS 30000
using namespace std;
struct Producer
{
	int left;
	vector<int> right;
};
ifstream fin;
ofstream fout;
ofstream sout;
ofstream hout;
char NO=1;
//һЩ�����Ķ���
vector<Producer> producerSet;//�洢���в���ʽ���������ڶ������ʽʱ���г�ʼ����
hash_map<int,vector<int> > hpSet;//�Բ���ʽ����Ϊ�ؼ��֣��Զ�Ӧ����ʽ�ı��Ϊ���ݵĹ�ϣ��
hash_map<int,int> terminSet;//�洢�ս��,***����Ҫ�����м����������#
hash_map<int,int> nonterminSet;//�洢���ս��,��ͬʱ�洢һ����action��ͷ�Ķ�Ӧ��ϵ
vector<vector<int> > actionTable;//�洢���������еڶ�ά��С������֪��
//Ŀǰ������Ϊ�涨�������ķ���̬���ķ��ŵ���ֵ��Χ��
hash_map<string,int> tsymTable;//��������ս���Ĵ�����Ӧ���ֱ����ӳ��
hash_map<string,int> ntsymTable;
hash_map<int,int> precedenceTable;//���ȼ���
vector<int> producerPreTable;//����ʽ�����ȼ�������������ȼ�������ʹ��
hash_set<int> leftTable;//���ϱ�
hash_set<int> rightTable;//�ҽ�ϱ�
hash_map<int,string> symclaTable;//�洢����ֵ���͵ı�
vector<string> produceActionTable;//�洢���嶯���ı�
//class ItemLess;//��������Item��С������
//������Item�Ķ���
class Item
{
public:
	//friend class ItemLess;
	friend bool operator <(const Item &it1,const Item &it2);
	Item(int producerNum,int currentPos);
	int getCurrSym();//������һ����Ҫ�ƽ��ķ��ţ�����Ѿ���������򷵻�-1����ʾ�ɹ�Լ��
	int getNextSym() const;//������һ��Ҫ�ƽ��ķ��ţ�����հ������м���������ʱʹ�á�
	int getProdN();//���ز���ʽ�ı�š�
	bool isEnd();//�Ƿ��Ҳ��Ѿ�ȫ���ƽ���
	bool nextIsEnd() const;//�Ҳ��Ƿ񵽴����һ�����ţ���հ�����ʱ�á�
	void setSearchSym(const hash_set<int> &searchSymbol);
	const hash_set<int> &getSearchSym() const;
	void move();
private:
	
	int pn;//�洢����ʽ�ı�š�
	int pos;//�洢����Ŀ���ƽ�λ�á�
	hash_set<int> searchSym;//�洢����Ŀ������������Ϊ�����������ⲿ����firstֱ��ʹ��
};



//������Item�ľ��庯����������
//........
Item::Item(int producerNum,int currentPos)
{
	this->pn=producerNum;
	this->pos=currentPos;
}
int Item::getCurrSym()
{
	return producerSet[pn].right[pos];//���ز���ʽ�Ҳ���һ�ƽ�����
}
int Item::getNextSym() const
{
	return producerSet[pn].right[pos+1];
}
int Item::getProdN()
{
	return this->pn;
}
bool Item::isEnd()
{
	if(producerSet[pn].right[0]==EPSLONG)
		return true;
	if(this->pos==producerSet[pn].right.size())
		return true;
	return false;
}
bool Item::nextIsEnd() const
{
	if(this->pos==producerSet[pn].right.size()-1)
		return true;
	return false;
}
void Item::setSearchSym(const hash_set<int> &searchSymbol)
{
	searchSym=searchSymbol;
}

const hash_set<int> &Item::getSearchSym() const
{
	return searchSym;
}

void Item::move()
{
	pos++;
	return;
}


bool operator <(const Item &it1,const Item &it2)
{//�˴���<���Ӷ���ܹؼ���һ��Ҫ����ƫ����
	if(it1.pn==it2.pn)
	{
		if(it1.pos==it2.pos)
			return it1.getSearchSym()<it2.getSearchSym();
		else return it1.pos<it2.pos;
	}
	else return it1.pn<it2.pn;
}
//

//����������
void closure(set<Item> &itemSet);
//��������first�ĺ���,����Ϊ����ʽ��ţ�first���ż����ѳ��ַ��ս���ż�
//��Ϊ�ݹ麯����Ϊ�ⲿ���ɵ���
void first(const Item &item,hash_set<int> &searchSymbol);
//�������ⲿ�ɵ���first����
bool getfirstSet(int producerNum, hash_set<int> &firstSet, hash_set<int> &isUsed);
bool produce();//������Ŀ��֮���ת����ϵ
void generateTableCode();
void generateParseCode();
void generateSemanticActionCode();//�������嶯������
void generateMainCode();
void generateHead();
void generateSVCode();//�������ս����Ĭ��ֵ �Ĵ���:void getvalue();
int readInputFile();//ǰ�����벿�ֵ���Ҫ���������ڶ����ļ�������
int specSymParse();
bool readReg();//����ε�ɨ�����
bool readAproducer();//��ȡһ������ʽ
void genhpSet();
void outSignalTable();
void displayI(Item & item);
////////////////////////////////////////////////////////////////////
//����������
///////////////////////////////////////////////////////////////////
void main() 
{
	string inputname;
	cout<<"Please input the file name:\n";
	cin>>inputname;
	fin.open(inputname.c_str());

	//fin.open("test2.txt");
	fout.open("yyparse.cpp");
	sout.open("signal_table.txt");//���ɷ��ű��ͷ�ļ�
	hout.open("yytab.h");//����ȫ�ַ�������ֵ����������
	if(fout.fail()||fin.fail()||sout.fail()||hout.fail())
	{
		cout<<"Cannot open output file!"<<endl;
		return;
	}
	//��ʼ��
	Producer ap;
	producerSet.push_back(ap);//���Ȱ���չ������ʼ���ŷ���
	readInputFile();
	generateMainCode();
	outSignalTable();
	cout<<"Finish!\n";
}
void outSignalTable()
{
	hout<<"\n#ifndef YYTAB_H\n";
	hout<<"#define YYTAB_H\n";
	sout<<setw(20)<<"SIGNAL"<<setw(20)<<"VALUE"<<endl;
	sout<<setfill('=')<<setw(41)<<" "<<setfill(' ')<<endl;
	int num=0;
	for(hash_map<string,int>::iterator pi=tsymTable.begin();pi!=tsymTable.end();pi++)
	{
		string delim("~!@#$%^&*()-=+`{}[]\|'\":;/?.,<>");
		int x=pi->first.find_first_of(delim);
		if(x<0||x>=pi->first.size())
		{
			hout<<"#define"<<setw(20)<<pi->first<<setw(20)<<pi->second<<endl;
		}
		sout<<setw(20)<<pi->first<<setw(20)<<pi->second<<endl;
		num++;
	}
	sout<<setfill('=')<<setw(41)<<" "<<setfill(' ')<<endl;
	sout<<setw(20)<<"TOTAL:"<<num<<endl;
	hout<<"#endif\n";
}


void closure(set<Item> &itemSet)
{
	queue<Item> Q;
	//����Ҫ�ѵ�ǰ��Ŀ��������״̬���뵽�����С�(��ȥ��������������)
	for(set<Item>::iterator ps=itemSet.begin();ps!=itemSet.end();ps++)
	{
		//cout<<"in closure "<<ps->getCurrSym()<<endl;
		if(!ps->isEnd()&&nonterminSet.count(ps->getCurrSym()))
			Q.push(*ps);
	}
	//����ѭ���жϡ�
	hash_set<int> isU;
	hash_set<int> isE;
	while(!Q.empty())
	{
		Item citem=Q.front();
		Q.pop();
		vector<int> vpi=hpSet[citem.getCurrSym()];
		hash_set<int> searchSym;
		first(citem,searchSym);//�˲�����������ļ���	
		for(int i=0;i<vpi.size();i++)
		{
			Item tp(vpi[i],0);
			tp.setSearchSym(searchSym);
			if(!itemSet.count(tp))//�������Ŀ������Ŀ���У����轫�����뵽��Ŀ����ȥ��
			{                //ͬʱ��Ҫ������Ӧ���������ļ��㡣
				itemSet.insert(tp); 
				if(nonterminSet.count(tp.getCurrSym()))
				{
					Q.push(tp);
				}
			}
		}
	}
}

void first(const Item &item,hash_set<int> &searchSymbol)
{
	if(item.nextIsEnd())
	{
		searchSymbol=item.getSearchSym();
	}
	else 
	{
		int sym=item.getNextSym();
		if(terminSet.count(sym))
			searchSymbol.insert(sym);
		else
		{
			vector<int> tv=hpSet[sym];
			hash_set<int> isused;
			for(int i=0;i<tv.size();i++)
			{
				getfirstSet(tv[i],searchSymbol,isused);
			}
		}
	}
}
bool getfirstSet(int producerNum, hash_set<int> &firstSet, hash_set<int> &isUsed)
{
	if ( producerSet.at(producerNum).right[0] == EPSLONG )
		return 0;
	else {
		int i = 0;
		if ( terminSet.count ( producerSet.at(producerNum).right[i] ) )
			firstSet.insert( producerSet.at(producerNum).right[i] );
		else if ( nonterminSet.count ( producerSet.at(producerNum).right[i] ) )
			{
			if ( !isUsed.count( producerSet.at(producerNum).right[i]) )
				{
				isUsed.insert ( producerSet.at(producerNum).right[i] );
				vector < int > tv;
				tv = hpSet[ producerSet.at(producerNum).right[i] ] ;
				vector < int >::const_iterator p1;
				int c = 1;  // 
				for ( p1 = tv.begin(); p1 != tv.end(); p1++ )
				{  
					if(!getfirstSet ( *p1, firstSet, isUsed ))
						{
						c=0;
						}
				}
				while( c == 0 ) 
					{
					i++;
					if ( producerSet.at(producerNum).right[i]!=0 &&
						 nonterminSet.count ( producerSet.at(producerNum).right[i]) &&
						 isUsed.count ( producerSet.at(producerNum).right[i] ) == 0)
						 {
						vector < int > tv2;
						tv2 = hpSet [ producerSet.at(producerNum).right[i] ] ;
						vector < int >::const_iterator p2;
						for (p2 = tv2.begin(); p2 != tv2.end(); p2++ )
							if(!getfirstSet ( *p2, firstSet,isUsed ))
								c=0;
					}
					else if ( terminSet.count ( producerSet.at(producerNum).right[i] ) )
						firstSet.insert ( producerSet.at(producerNum).right[i] );
				}
			}
		}
		return 1;
	}
}

void display(set<Item> & i0)
{
	cout<<"\n----------------ItemSet ---------------------"<<endl;
	for(set<Item>::iterator pi=i0.begin();pi!=i0.end();pi++)
	{
		cout<<pi->getProdN()<<"--"<<pi->getCurrSym()<<"  SEARCH:";
		for(hash_set<int>::const_iterator i=pi->getSearchSym().begin();i!=pi->getSearchSym().end();i++)
			cout<<*i<<" ";
		cout<<"||"<<endl;
	}
	cout<<"\n------------------------\n";
}

void displayI(Item & item)
{
	cout<<"\nitem -----------------------------\n";
	cout<<"PID:"<<item.getProdN()<<endl;
	cout<<"LEFT:"<<producerSet[item.getProdN()].left<<endl;
	cout<<"RIGHT:";
	for(int i=0;i<producerSet[item.getProdN()].right.size();i++)
	{
		cout<<producerSet[item.getProdN()].right[i]<<"  ";
	}
	cout<<"\nSEARCH:";
	for(hash_set<int>::const_iterator i=item.getSearchSym().begin();i!=item.getSearchSym().end();i++)
	{
		cout<<*i<<"  ";
	}
	cout<<"\n-----------------------------------\n";
}
void genhpSet()
{
	for(int i=1;i<producerSet.size();i++)//0�Ų���ʽ���ù�
	{
		if(!hpSet.count(producerSet[i].left))
		{
			pair<int,vector<int> > tp;
			tp.first=producerSet[i].left;
			vector<int> vt;
			tp.second=vt;
			hpSet.insert(tp);
		}
		hpSet[producerSet[i].left].push_back(i);
	}
}
bool produce()
{
	/**
	 *  �˴�����ͼ�ı�ʾ����Ϊ��ά���飬��Ϊ�����ʱҲ���Զ�ά�������ʽ��
	 *  �ڴ������У���������ʾҪ��ת��״̬�ţ��ø�����ʾĳһ����Լ�Ĳ���ʽ�š�
	 *  DONE!
	 */
	//������һ��Ҫ�õ��ĳ�������
	int curState=0;//�洢��Ŀ����״̬�š�����Ŀ���Ĳ�����̬����
	int widgeN=nonterminSet.size()+terminSet.size();//��ʾ���п��ܵ���Ŀ���ı�
	queue<set<Item> > Q;
	map<set<Item>,int> itemsetTable;//����Ŀ����ĳһ���ֱ�ʶ��ӳ��
	hash_map<int, set<Item> > moveItemset;//�Ա�Ϊ�ؼ��֣��Խ��з������Ŀ����Ŀ��Ϊ���ݡ�
	set<Item> I0;

	Item firstitem(0,0);//�����ʼ��Ŀ���е���Ŀ��
	hash_set<int> searchs;
	searchs.insert(NONTERMINS-1);//��ʾ����״̬
	firstitem.setSearchSym(searchs);

	I0.insert(firstitem);

	firstitem.move();//�޸����ƽ�λ����Ϊ�ں��������ȷ״̬ʱҪ��

	closure(I0);
	Q.push(I0);

	display(I0);
	pair<set<Item>,int> te;//����Ŀ��0���뵽��Ŀ���ı���
	te.first=I0;
	te.second=curState;
	itemsetTable.insert(te);

	while(!Q.empty())
	{
		set<Item> itemset=Q.front();
		Q.pop();//�Ӷ�����ȡ��һ����Ŀ����
		moveItemset.clear();//��գ��Խ�����һ��ѭ��

		vector<int> f(widgeN);//����ڶ�ά�Ĵ�С����Ԥ�ȶ����
		for(int i=0;i<f.size();i++)
		{
			f[i]=ERROR;//��ERRORֵ��vector���г�ʼ����
		}

		actionTable.push_back(f);

		int column,row;
		row=itemsetTable[itemset];
		cout<<"\n===================================================================\n";
		//display(itemset);
		cout<<"state = "<<row<<endl;
		cout<<"\n===================================================================\n";

		//��ʼ������������Ŀ��move�����Ŀ����te
		for(set<Item>::iterator ps=itemset.begin();ps!=itemset.end();ps++)
		{
			if(ps->isEnd())//���ֿɹ�Լ��Ŀ������д���
			{
				for(hash_set<int>::const_iterator pi=ps->getSearchSym().begin();pi!=ps->getSearchSym().end();pi++)
				{
					//�Կɹ�Լ��Ŀ��������ÿ�����������������
					column=terminSet[*pi];
					if(actionTable[row][column]<=0&&ps->getProdN()!=actionTable[row][column]*(-1))
					{
						cout<<"Reduction & Reduction Confliction"<<endl;
						cout<<"row="<<row<<" column="<<column<<" old="<<actionTable[row][column]
					       <<" new="<<ps->getProdN()<<endl;
						return false;
					}
					actionTable[row][column]=ps->getProdN()*(-1);//��ɸ����Ա�ʾ��Ϊ��Լ��,���Ϊ0���ڱ�ʾaccept
					//cout<<"actionTable row="<<row<<" column="<<column<<endl;
				}
			}
			else
			{
				Item tp=*ps;
				tp.move();
				moveItemset[ps->getCurrSym()].insert(tp);
			}
		}
		//�������²�������Ŀ����հ����ж��Ƿ���Ҫ���뵽������ȥ��
		for(hash_map<int,set<Item> >::iterator ph=moveItemset.begin();ph!=moveItemset.end();ph++)
		{
			if(curState==75)
			{
				cout<<"wa";
			}
			closure(ph->second);

			if(!itemsetTable.count(ph->second))
			{
				pair<set<Item>,int> tp;
				tp.first=ph->second;			
				curState++;//ֻ�ڴ˴�������Ŀ������ֻ�ڴ˴��޸���ֵ
				tp.second=curState;

				itemsetTable.insert(tp);
				//�����ǰ����Ŀ��δ���ֹ��Ļ�����Ҫ�����������еȴ�����
				Q.push(ph->second);
			}	

			//���濪ʼ���ɶ�Ӧ��action��
			//������ñ�����Ӧ�ı��е����
			column=(nonterminSet.count(ph->first)?nonterminSet[ph->first]:terminSet[ph->first]);

			//�˴���һЩ�������ж��Ƿ��й�Լ���ƽ���ͻ
			if(actionTable[row][column]<=0)
			{	
				if(producerPreTable[actionTable[row][column]*(-1)]<precedenceTable[ph->first])
				{
					actionTable[row][column]=itemsetTable[ph->second];
					continue;
				}
				if(rightTable.count(ph->first))
				{
					actionTable[row][column]=itemsetTable[ph->second];
					continue;
				}
				if(leftTable.count(ph->first))//����˲���������������ϱ���
				{
					continue;
				}
	
				//������������������㣬�򱨴�
				cout<<"Reduction & Shift Confliction"<<endl;
				cout<<"row="<<row<<" column="<<column<<" old="<<actionTable[row][column]
					<<" new="<<itemsetTable[ph->second]
					<<" signal="<<ph->first<<endl;
				return false;
			}
			else
				actionTable[row][column]=itemsetTable[ph->second];
			//cout<<"actionTable row="<<row<<" column="<<column<<endl;
			//getch();
		}
	}
	return true;
	//�����������Ŀ����������Ĺ���
}
void generateTableCode()
{
	fout<<"int actionTable["<<actionTable.size()<<"]["<<actionTable.front().size()<<"]=\n\t\t{";
	for(int i=0;i<actionTable.size();i++)
	{
		fout<<"\t\t{";
		for(int j=0;j<actionTable.front().size();j++)
		{
			if(actionTable[i][j]!=40000)
				fout<<actionTable[i][j];
			else
				fout<<"E";
			if(j==actionTable.front().size()-1) continue;
			fout<<",";
		}
		fout<<"}";
		if(i==actionTable.size()-1) continue;
		fout<<",\n";
	}
	fout<<"};\n";
	fout<<"int searchTable(int cstate,char symbol)\n";
	fout<<"{\n";
	fout<<"\treturn actionTable[cstate][symbol];\n";
	fout<<"}\n";
	fout<<"\n";
	//�˲�����ɲ���������
	//������ɶ�Token��������
	fout<<"int readToken()\n";
	fout<<"{\n";
	fout<<"\tif(fin.eof())\n";
	fout<<"\t\treturn SOURCE_END;\n";
	fout<<"\tchar buf[256];//��֧�ֳ���256���ַ��ķ���\n";
	fout<<"\tint pos=0;\n";
	fout<<"\tbool isEnd=false;\n";
	fout<<"\tbool isToken=false;//��isOperator����\n";
	fout<<"\tbool isOperator=false;\n";
	fout<<"\tstring ops(\"!@#$%^&*()+-=|\[]{};':\\\",.<>/?\");\n";
	fout<<"\tstring wss(\"\\t\\n\\r \");\n";
	fout<<"\tstring letter(\"_0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ\");\n";

	fout<<"\tchar c=fin.get();\n";
	fout<<"\twhile(c==' '||c=='\\t'||c=='\\n'||c=='\\r') c=fin.get();//�˵��հ׷�\n";
	fout<<"\twhile(!isEnd)\n";
	fout<<"\t{\n";
	fout<<"\t\tif(c==-1)\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tbuf[pos]='\\0';\n";
	fout<<"\t\t\tisEnd=true;\n";
	fout<<"\t\t\tcontinue;\n";
	fout<<"\t\t}";
	fout<<"\t\tif(ops.find(c)>=0&&ops.find(c)<ops.size())//��ʾ��c���������ַ�ʱ��\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\t//����������ַ��Ĵ������\n";
	fout<<"\t\t\tif(isToken)\n";
	fout<<"\t\t\t{\n";
	fout<<"\t\t\t\tfin.unget();\n";
	fout<<"\t\t\t\tbuf[pos]='\\0';\n";
	fout<<"\t\t\t\tisEnd=true;\n";
	fout<<"\t\t\t}\n";
	fout<<"\t\t\telse\n";
	fout<<"\t\t\t{\n";
	fout<<"\t\t\t\tisOperator=true;\n";
	fout<<"\t\t\t\tbuf[pos]=c;\n";
	fout<<"\t\t\t\tpos++;\n";
	fout<<"\t\t\t\tc=fin.get();\n";
	fout<<"\t\t\t}\n";
	fout<<"\t\t\tcontinue;\n";
	fout<<"\t\t}\n";
	fout<<"\t\tif(letter.find(c)>=0&&letter.find(c)<letter.size())\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tif(isOperator)\n";
	fout<<"\t\t\t{\n";
	fout<<"\t\t\t\tfin.unget();\n";
	fout<<"\t\t\t\tbuf[pos]='\\0';\n";
	fout<<"\t\t\t\tisEnd=true;\n";
	fout<<"\t\t\t}\n";
	fout<<"\t\t\telse\n";
	fout<<"\t\t\t{\n";
	fout<<"\t\t\t\tisToken=true;\n";
	fout<<"\t\t\t\tbuf[pos]=c;\n";
	fout<<"\t\t\t\tpos++;\n";
	fout<<"\t\t\t\tc=fin.get();\n";
	fout<<"\t\t\t}\n";
	fout<<"\t\t\tcontinue;\n";
	fout<<"\t\t}\n";
	fout<<"\t\tif(wss.find(c)>=0&&wss.find(c)<wss.size())\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tbuf[pos]='\\0';\n";
	fout<<"\t\t\tisEnd=true;\n";
	fout<<"\t\t}\n";
	fout<<"\t\t\n";
	fout<<"\t\t\n";

	fout<<"\t\telse return -1;\n";
	fout<<"\t}\n";
	fout<<"\treturn analysis(buf,strlen(buf));\n";
	fout<<"}\n";
}

void generateSemanticActionCode()
{
	fout<<"void runaction(int num)\n";
	fout<<"{\n";
	fout<<"\tswitch(num)\n";
	fout<<"\t{\n";
	for(int i=0;i<produceActionTable.size();i++)
	{
		if(produceActionTable[i].size()!=0)
		{
			fout<<"\tcase "<<i+1<<":\n";
			fout<<"\t\t{\n";
			fout<<"\t\t\t"<<produceActionTable[i]<<"\n";
			fout<<"\t\t\tbreak;\n";
			fout<<"\t\t}\n";
		}
	}
	fout<<"\t}\n";
	fout<<"}\n";
}

void generateParseCode()
{
	fout<<"int parse()\n";
	fout<<"{\n";
	fout<<"\tint inputsymbol=0;\n";
	fout<<"\tint cstate=0;\n";
	fout<<"\tstack<Sym> symStack;//����ջ\n";
	fout<<"\tstack<SV> valStack;//����ֵջ\n";
	fout<<"\tSym st;//��������ʱ����ʱջ��Ԫ�ش洢����\n";
	fout<<"\tst.symbol=0;\n";
	fout<<"\tst.state=0;\n";
	fout<<"\tSV val;\n";
	fout<<"\tsymStack.push(st);//����ֵջ����Ҫ�ͷ���ջͬ��\n";
	fout<<"\tvalStack.push(val);\n";
	fout<<"\tinputsymbol=readToken();\n";
	fout<<"\twhile(1)\n";
	fout<<"\t{\n";
	fout<<"\t\tst=symStack.top();\n";
	fout<<"\t\tint col=signalTable[inputsymbol];\n";
	fout<<"\t\tint result=searchTable(st.state,col);\n";
	fout<<"\t\tif(result==E)//����\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tcout<<\"Compile Error!\"<<endl;\n";
	fout<<"\t\t\treturn 0;\n";
	fout<<"\t\t}\n";
	fout<<"\t\tif(result==ACCEPT)\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tcout<<\"Compile sucessfully!\"<<endl;\n";
	fout<<"\t\t\treturn 1;\n";
	fout<<"\t\t}\n";
	fout<<"\t\tif(result<0)//������ʾΪ��Լ��Ŀ\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tresult*=-1;\n";
	fout<<"\t\t\tint n=producerN[result];//ȡ�øúŲ���ʽ�Ҳ�����������������ջ��\n";
	fout<<"\t\t\tfor(int i=0;i<n;i++)\n";
	fout<<"\t\t\t{\n";
	fout<<"\t\t\t\tsymStack.pop();\n";
	fout<<"\t\t\t\tu[n-i]=valStack.top();\n";
	fout<<"\t\t\t\tvalStack.pop();\n";
	fout<<"\t\t\t}\n";
	fout<<"\t\t\trunaction(result);//ִ�����嶯��\n";
	fout<<"\t\t\t//�ٽ�����ʽ�󲿵ķ���ѹջ������ֵһͬѹջ\n";
	fout<<"\t\t\tst.symbol=pLeftSection[result];\n";
	fout<<"\t\t\tst.state=searchTable(symStack.top().state,signalTable[st.symbol]);\n";
	fout<<"\t\t\tsymStack.push(st);\n";
	fout<<"\t\t\t//������ʽ�󲿷��ŵ�����ֵ��ջ�����ֵ��runaction()���Ѿ��޸�\n";
	fout<<"\t\t\tvalStack.push(u[0]);\n";
	fout<<"\t\t}\n";
	fout<<"\t\telse\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tst.symbol=inputsymbol;\n";
	fout<<"\t\t\tst.state=result;\n";
	fout<<"\t\t\tsymStack.push(st);\n";
	fout<<"\t\t\tSV tu;\n";
	fout<<"\t\t\tif(yyval.ival==0)\n";
	fout<<"\t\t\t\tgetvalue(inputsymbol,tu);\n";
	fout<<"\t\t\telse tu=yyval;\n";
	fout<<"\t\t\tvalStack.push(tu);\n";
	fout<<"\t\t\tinputsymbol=readToken();\n";
	fout<<"\t\t}\n";
	fout<<"\t}\n";
	fout<<"}\n";
}

void generateMainCode()
{
	fout<<"void main()\n";
	fout<<"{\n";
	/*
	for(hash_map<string,int>::iterator pti=tsymTable.begin();pti!=tsymTable.end();pti++)
	{
		if(pti->first!="#")
		fout<<"\tconst int "<<pti->first<<"="<<pti->second<<";\n";//�������������������
	}*/
	fout<<"\tpair<int,int> tp;\n";
	for(hash_map<string,int>::iterator pti=tsymTable.begin();pti!=tsymTable.end();pti++)
	{
		if(pti->first!="#")
		{
			fout<<"\ttp.first="<<pti->second<<";\n";
			fout<<"\ttp.second="<<terminSet[pti->second]<<";\n";
			fout<<"\tsignalTable.insert(tp);\n";
		}
		else
		{
			fout<<"\ttp.first=40001;\n";
			fout<<"\ttp.second="<<terminSet[pti->second]<<";\n";
			fout<<"\tsignalTable.insert(tp);\n";
		}
	}
	for(hash_map<int,int>::iterator pti=nonterminSet.begin();pti!=nonterminSet.end();pti++)
	{
		fout<<"\ttp.first="<<pti->first<<";\n";
		fout<<"\ttp.second="<<pti->second<<";\n";
		fout<<"\tsignalTable.insert(tp);\n";
	}
	//�������signalTable�ĳ�ʼ���δ���.
	fout<<"\tstring filename;\n";
	fout<<"\tcout<<\"Please input the file name:\"<<endl;\n";
	fout<<"\tcin>>filename;\n";
	fout<<"\tfin.open(filename.c_str());\n";
	fout<<"\tif(fin.fail())\n";
	fout<<"\t{\n";
	fout<<"\t\tcout<<\"Cannot open the file \"<<filename<<endl;\n";
	fout<<"\t\treturn;\n";
	fout<<"\t}\n";
	fout<<"\tparse();\n";
	fout<<"}\n";
}
void generateSVCode()
{
	fout<<"void getvalue(int symbol,SV & val)\n";
	fout<<"{\n";
	fout<<"\tswitch(symbol)\n";
	fout<<"\t{\n";
	for(hash_map<int,int>::iterator pi=terminSet.begin();pi!=terminSet.end();pi++)
	{
		if(symclaTable.count(pi->first))
		fout<<"\tcase "<<pi->first<<":val."<<symclaTable[pi->first]<<"="<<pi->first
			<<";break;\n";
	}
	fout<<"\t}\n";
	fout<<"}\n";


}

void generateConstCode()
{
	fout<<"using namespace std;\n";
	fout<<"extern int analysis(char *yytext,int n);\n";
	fout<<"struct Sym\n";
	fout<<"{\n";
	fout<<"\tint symbol;\n";
	fout<<"\tint state;\n";
	fout<<"};\n";
	fout<<"hash_map<int,int> signalTable;\n";
	fout<<"int producerN["<<producerSet.size()<<"]={";
	for(int i=0;i<producerSet.size();i++)
	{
		fout<<producerSet[i].right.size();
		if(i!=producerSet.size()-1)
			fout<<",";
	}
	fout<<"};\n";
	fout<<"int pLeftSection["<<producerSet.size()<<"]={";
	for(i=0;i<producerSet.size();i++)
	{
		fout<<producerSet[i].left;
		if(i!=producerSet.size()-1)
			fout<<",";
	}
	fout<<"};\n";
	fout<<"ifstream fin;\n";
}
void generateHead()
{
	fout<<"#include<iostream>\n";
	fout<<"#include<hash_map>\n";
	fout<<"#include<stack>\n";
	fout<<"#include<fstream>\n";
	fout<<"#include<string>\n";
	fout<<"#define E 40000\n";
	fout<<"#define ACCEPT 0\n";
	fout<<"#define SOURCE_END 40001\n";	
}

int readInputFile()
{
	generateHead();
	char c=fin.get();
	if(c!='%') return TERMINATE;//Ҫ�������ļ���һ���ַ�����Ϊ%
	int state=specSymParse();
	while(!fin.eof()&&c!=-1)
	{
		switch(state)
		{
		case SEGMENT_DELIM:
			{
				NO++;
				if(NO==2)
				{
					state=SEGMENT_REG;//��������
					continue;//��������Ĳ���%����䣬ֱ�ӻص�switch
				}
				if(NO==3)
				{
					state=SELF_DEF;
					continue;
				}
				break;
			}
		case DEF_SEG_BEGIN://output 1
			{
				//����%{������ȫ������ļ�
				while(1)
				{
					c=fin.get();
					if(c=='%')
					{
						state=specSymParse();
						if(state==DEF_SEG_END) break;//�������ײ�%}�����˳�ѭ����
						else if(state!=NOMATCH)
						{
							return TERMINATE;//����Ȳ�����DEF_SEG_ENDҲ������NOMATCH���򱨴�
						}
					}
					fout.put(c);///////********output to file
				}
				break;
			}
		case TOKEN:case TYPE:
			{
				string line;
				getline(fin,line);

				int lpos=0;
				int pos=0;
				while(line[pos]!='<'&&pos<line.size()) pos++;
				if(pos==line.size())
				{
					cout<<"%type formation error!"<<endl;
					return 0;
				}
				
				pos++;//������һ���ַ�
				lpos=pos;
				while(line[pos]!='>'&&pos<line.size()) pos++;
				if(pos==line.size())
				{
					cout<<"%type or token formation error!"<<endl;
					return 0;
				}
				string valstr=line.substr(lpos,pos-lpos);//ȡ��ֵ����

				char ntbuf[20]={0};
				int i=0;
				while(1)
				{
					pos++;
					if(pos>line.size()) break;
					if(line[pos]==' '||pos==line.size())
					{
						if(ntbuf[0]=='\0') continue;
						ntbuf[i]='\0';
						string ntstr(ntbuf);

						pair<string,int> tp;
						tp.first=ntstr;
						if(state==TYPE)
						{
							tp.second=NONTERMINS+ntsymTable.size();
						}
						else
						{
							tp.second=TERMINS+tsymTable.size();
						}
						pair<int,int> tp2;
						tp2.first=tp.second;
						tp2.second=terminSet.size()+nonterminSet.size();

						if(state==TYPE)
						{
							ntsymTable.insert(tp);
							nonterminSet.insert(tp2);
						}
						else
						{
							tsymTable.insert(tp);
							terminSet.insert(tp2);
						}

						pair<int,string> stp;
						stp.first=tp.second;
						stp.second=valstr;
						symclaTable.insert(stp);
						
						i=0;
						ntbuf[0]='\0';
					}
					else
					{
						ntbuf[i]=line[pos];	
						i++;
					}
				}
				break;
				//�˲��ִ�����֮������˱�symclaTable�Ͳ���ntsymTable
			}
		case UNION://output 2
			{
				c=fin.get();
				while(c!='{'&&c!=-1) c=fin.get();
				if(c==-1) return TERMINATE;	
				hout<<"#ifndef YYVAL_H\n";//�����ͷ�ļ�yyval_h.h
				hout<<"#define YYVAL_H\n";
				fout<<"union SV\n";
				hout<<"union SV\n";
				fout<<"{\n";
				hout<<"{\n";
				while((c=fin.get())!='}') {fout.put(c);hout.put(c);}
				fout<<"};\n";//�˶�û�����κα��棬����˵������ִ���Ҳ����У��
				hout<<"};\n";
				fout<<"SV u[10];\n";//�ڴ�������ĿΪ10���������飬�Ա�������嶯��ʱ���á�
				fout<<"SV yyval;\n";//�ڴ�����ȫ�ֱ���yyval�����ڷ���lex���ŵ�����ֵ
				hout<<"#endif";
				break;
			}
		case LEFT:case RIGHT:
			{
				string line;
				getline(fin,line);
				int lpos=0;
				int pos=0;
				static int priority=0;
				priority++;
				while(pos!=line.find_last_of('\''))
				{
					lpos=line.find_first_of('\'',lpos);
					pos=line.find_first_of('\'',lpos+1);
					string t=line.substr(lpos+1,pos-lpos-1);
					lpos=pos+1;	
					//�洢��������
					pair<string,int> tp;
					tp.first=t;
					tp.second=TERMINS+tsymTable.size();
					tsymTable.insert(tp);//�Ѳ�����Ҳ�ŵ��ս�����С�
				    
					pair<int,int> tpp;
					tpp.first=tp.second;
					tpp.second=terminSet.size()+nonterminSet.size();
					terminSet.insert(tpp);
					if(state==LEFT)//�������ҽ�ϱ�
					{
						leftTable.insert(tp.second);
					}
					else
					{
						rightTable.insert(tp.second);
					}

					pair<int,int> ptp;
					ptp.first=tp.second;
					ptp.second=priority;
					precedenceTable.insert(ptp);//�Ѳ������������ȼ����С�
				}
				break;
			}
		case SEGMENT_REG:
			{
				//�˴�Ϊ����ε�ɨ��ʹ��������
				//��һ��������ʽ�ļ��ϣ���ɸ��ֱ�����ݡ�			
				//����producerPreTable		
				producerPreTable.push_back(0);//��֤����ֵ���ʲ������
				if(!readReg())
				{
					cout<<"read reg segment error!\n";
					return 0;
				}
				//����ProducerSet;
				producerSet[0].left=NONTERMINS-1;//��ʾ����չ�ķ���ʼ����
				producerSet[0].right.push_back(producerSet[1].left);
				//����tsymTable
				pair<string,int> tp1;
				string s("#");
				tp1.first=s;
				tp1.second=NONTERMINS-1;
				tsymTable.insert(tp1);
				//����terminSet
				pair<int,int> tp2;
				tp2.first=tp1.second;
				tp2.second=terminSet.size()+nonterminSet.size();
				terminSet.insert(tp2);
				//�ڶ�����ɶ���������ɡ�
				genhpSet();//�������hpSet������
				if(!produce())
				{
					cout<<"produce action table error!"<<endl;
					return 0;
				}

				//����ļ������һЩ����
				generateConstCode();
				//������������������Token�ĺ���
				generateTableCode();
				//���Ĳ����ִ�����嶯���ĺ���
				generateSemanticActionCode();
				//���岽�����������,���Ȳ���void getvalue()���������ս����Ĭ�ϳ�ֵ
				generateSVCode();
				generateParseCode();
				//�����������������Ӧ��������������
				//generateMainCode();
				break;
			}
		case SELF_DEF://����û��Զ���������
			{
				c=fin.get();
				while(c!=-1)
				{
					fout.put(c);////*******output to file
					c=fin.get();
					
				}
				fout.put('\n');
				continue;//����while���жϽ�����
			}
		}
		c=fin.get();
		while(c!='%'&&c!=-1) c=fin.get();
		if(c=='%')
			state=specSymParse();
	}
}
int specSymParse()
{
	char nextc=fin.get();
	switch(nextc)
	{
	case '%'://����ηָ���
		{
			return SEGMENT_DELIM;
		}
	case '{':
		{
			return DEF_SEG_BEGIN;
		}
	case '}':
		{
			return DEF_SEG_END;
		}
	case 't':case 'u':case 'l':case 'r'://%token	//%union//%left
		{
			char buf[10];
			int pos=1;
			buf[0]=nextc;
			nextc=fin.get();
			while(nextc!=' '&&nextc!='\n'&&nextc!='\t')
			{
				buf[pos]=nextc;
				nextc=fin.get();
				pos++;
				if(pos==5) break;
			}
			buf[pos]='\0';
			string bbuf(buf);
			string t1("token");
			string t2("type");
			string t3("union");
			string t4("left");
			string t5("right");
			if(bbuf==t1) return TOKEN;
			if(bbuf==t2) return TYPE;
			if(bbuf==t3) return UNION;
			if(bbuf==t4) return LEFT;
			if(bbuf==t5) return RIGHT;
			else return TERMINATE;//���󣬳�����ֹ
		}

	default:
		fin.unget();
		return NOMATCH;
	}
}

bool readReg()
{
	//����ε�ɨ�����
	char c=fin.get();
	while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	int state=-1;
	char buf[256];
	int pos=0;
	while(1)
	{
		if(c==-1) break;
		if(c=='%')
		{
			state=specSymParse();
			if(state==SEGMENT_DELIM)
			{
				fin.unget();
				fin.unget();//���������ַ�����Ϊ��readInputFile()�����ж�%%���д���ĳ���
				break;
			}
			if(state==TERMINATE) 
			{
				cout<<"Regulation Segment ERROR!"<<endl;
				return false;
			}	
			if(state==NOMATCH) fin.unget();//��%Ҳ�˻أ��������������
			else
			{
				cout<<"Regulation Segment ERROR2"<<endl;
				return false;//����ֱ�ӷ���.
			}
		}


		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();//�׵��ո�
		fin.unget();//�˻أ���������readAproducer()����
		if(!readAproducer())
		{
			cout<<"producer error!see above!"<<endl;
			return false;
		}
		c=fin.get();
		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	}

}
bool readAproducer()//δ�׵�ǰ��Ŀո�
{
	char c=fin.get();
	char buf[256];
	int pos=0;
	while(c!=' '&&c!='\n'&&c!='\t'&&c!='\r'&&c!=-1)
	{
		buf[pos]=c;
		pos++;
		c=fin.get();
	}
	fin.unget();
	buf[pos]='\0';
	string temp(buf);
	buf[0]='\0';//���buf��
	pos=0;
	if(tsymTable.count(temp))
	{
		cout<<"NonTerminatedSym define error!"<<endl;
		return false;
	}
	if(!ntsymTable.count(temp))
	{
		pair<string,int> tp;
		tp.first=temp;
		tp.second=NONTERMINS+ntsymTable.size();
		ntsymTable.insert(tp);
	
		pair<int,int> tpp;
		tpp.first=tp.second;
		tpp.second=terminSet.size()+nonterminSet.size();
		nonterminSet.insert(tpp);//�����ս�����ڼ������ݡ��洢���Ƿ��ս����Ӧ��action��ı�ͷ
	}
	c=fin.get();
	while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	if(c!=':')
	{
		cout<<"Producer formation Error!"<<endl;
		return false;
	}
	while(1)
	{
		Producer ap;
		ap.left=ntsymTable[temp];//�½�һ������ʽ���ȱ����󲿵�ֵ
		int priority=0;//�洢����ʽ�����ȼ�����ʱ��ȡ�ķ����ǣ�����ʽ�����ȼ�ͬ�Ҳ��������ҵĲ���
	               //�����ȼ���ͬ
		while(1)
		{
		//�����׵��ո�
			c=fin.get();
			while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
			if(c==';'||c=='{'||c=='|') break;//����ʽ������ǻ����Ƕ�����ʼ���
			//������ʽ���Ҳ�
			if(c=='\'')//�������'�������������Ķ������
			{
				c=fin.get();
				while(c!='\'')
				{
					buf[pos]=c;
					pos++;
					c=fin.get();
				}
				buf[pos]='\0';
				string temp3(buf);
				pos=0;
				buf[0]='\0';//clear buf
				if(!tsymTable.count(temp3))
				{
					pair<string,int> tp;
					tp.first=temp3;
					tp.second=TERMINS+tsymTable.size();

					tsymTable.insert(tp);//�Ѳ�����Ҳ�ŵ��ս�����С�
				
					pair<int,int> tpp;
					tpp.first=tp.second;
					tpp.second=terminSet.size()+nonterminSet.size();
					terminSet.insert(tpp);
					//ap.right.push_back(tp.second);

					pair<int,int> ptp;
					ptp.first=tp.second;
					ptp.second=0;
					precedenceTable.insert(ptp);
				}
				ap.right.push_back(tsymTable[temp3]);
				priority=precedenceTable[tsymTable[temp3]];
				continue;
			}
			if(c=='\\')//��\e��ʾ��
			{
				c=fin.get();
				if(c=='e')
				{
					break;
				}
				else
				{
					cout<<"epslong error!"<<endl;
					return false;
				}
			}
			else
			{
				while(c!=' '&&c!=-1&&c!='\n'&&c!='\t'&&c!='\r')
				{
					buf[pos]=c;
					pos++;
					c=fin.get();
				}
				buf[pos]='\0';
				string temp1(buf);
				pos=0;
				buf[0]='\0';//clear buf
				if(tsymTable.count(temp1))
				{
					ap.right.push_back(tsymTable[temp1]);
				}
				else
				{
					if(!ntsymTable.count(temp1))
					{
						pair<string,int> tp;
						tp.first=temp1;
						tp.second=NONTERMINS+ntsymTable.size();
						ntsymTable.insert(tp);
	
						pair<int,int> tpp;
						tpp.first=tp.second;
						tpp.second=terminSet.size()+nonterminSet.size();
						nonterminSet.insert(tpp);

					}
					ap.right.push_back(ntsymTable[temp1]);
				}
			}
		}
		char action[1000];//�洢���嶯��
		action[0]='\0';
		if(c=='{')
		{
			char change[4]={'u','[','0',']'};
			int pi=0;
			c=fin.get();
			while(c!='}')
			{
				if(c=='$')
				{
					c=fin.get();
					if(c=='$')//��ʾ�ǲ���ʽ�󲿵�����ֵ����
					{
						//���봮"u[0]"
						for(int i=0;i<4;i++)
							action[pi+i]=change[i];
						pi+=4;
						action[pi]='.';//��������ֵ�ľ�������
						pi++;
						string s=symclaTable[ap.left];
						for(i=0;i<s.size();i++)
							action[pi+i]=s[i];
						pi+=i;
					}
					else 
					if(c>='1'&&c<='9')
					{
						int num=0;
						action[pi++]='u';
						action[pi++]='[';
						while(c>='0'&&c<='9')//֧�ָ�����Ҳ����ս��
						{
							action[pi++]=c;
							num=num*10+c-'0';
							c=fin.get();
						}
						fin.unget();
						action[pi++]=']';
						/*
						for(int i=0;i<4;i++)
						{
							action[pi+i]=change[i];
						}*/
						//action[pi+2]=c;//ֻ���޸Ĵ˴���ֵ���ɡ�
						//pi+=4;
						action[pi++]='.';//��������ֵ�ľ�������
						string s=symclaTable[ap.right[num-1]];
						for(int i=0;i<s.size();i++)
							action[pi+i]=s[i];
						pi+=i;
					}
					else
					{
						cout<<"Action definition error!"<<endl;
						return false;
					}
				}
				else
				{
					action[pi]=c;
					pi++;	
				}
				c=fin.get();
			}
			action[pi]='\0';
			c=fin.get();
			while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
		}
		if(c==';'||c=='|')
		{
			//�������嶯����
			string oneaction(action);
			produceActionTable.push_back(oneaction);
			producerSet.push_back(ap);
			producerPreTable.push_back(priority);
			if(c==';')
				break;
		}
		else
		{
			cout<<"Producer formation error,there should be a ';' after each producer!"<<endl;
			return false;
		}
	}
	return true;
}


