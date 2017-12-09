#include "FileReader.h"

FileReader::FileReader(string fileName)
{
	fcin.open(fileName, ios::in);

	// ����regularExpressionNum
	regularExpressionNum = 0;
	// ����definePartNum
	definePartNum = 0;
	// ����lastPartNum
	lastPartNum = 0;
}
FileReader::~FileReader()
{
	fcin.close();
}
vector<string> FileReader::getRegularExpression() 
{
	// ����regularExpressionNum
	fcin.clear();
	fcin.seekg(0, ios::beg); //��ָ���Ƶ��ļ���λ��
	string s;
	for(int i=0; i<2; i++)
	{
		while(fcin)
		{
			getline(fcin, s);
			if(s.size()==0 || s[0]==' ' || s[0]=='\t')
			{
				continue;
			}
			if(s == "%%")
			{
				regularExpressionNum++;
				break;
			}
			if(regularExpressionNum != 0)
			{
				regularExpressionNum++;
			}
		}
	}
	regularExpressionNum -= 2; //��ȥ��β����

	//��ָ���Ƶ��ļ���λ��
	fcin.clear();
	fcin.seekg(0, ios::beg); 
	while(fcin)
	{
		getline(fcin, s);
		//��ʼλ��
		if(s=="%%")
		{
			break;
		}
	}

	vector<string> ret(regularExpressionNum);
	for( int i = 0;  ; i++)
	{
		//��������
		getline( fcin, s);
		if(s == "%%")
			break;
		int len = s.size();
		if(len==0 || s[0]==' ' || s[0]=='\t')
		{
			i--; //��ret������ΪCminus.l�еĿ��г��ֿ�ֵ
			continue;
		}
		int temp = 0;
		for( int j = 0; j < len;  j++ )
		{
			//ƥ�䡰��
			if (s[j]=='"')
				temp++;
			if((s[j]==' ' || s[j]=='\t') && temp %2 ==0) 
			{
				ret[i] = s.substr(0, j);				
				break;
			}
		}
	}
	return ret;
}
vector<string> FileReader::getCode() 
{
	fcin.clear();
	fcin.seekg(0, ios::beg); //��ָ���Ƶ��ļ���λ��

	string s;
	while(fcin)
	{
		getline(fcin, s);
		if(s=="%%")
		{
			break;
		}
	}
	vector<string> ret(regularExpressionNum);
	for(int i=0; ; i++)
	{
		getline(fcin, s);
		if(s == "%%")
		{
			break;
		}

		int len = s.size();
		if(len==0 || s[0]==' ' || s[0]=='\t')
		{
			i--; //��ret������ΪCminus.l�еĿ��г��ֿ�ֵ
			continue;
		}

		int temp = 0;
		for(int j=0; j<len; j++)
		{
			//��� ��
			if (s[j]=='"')
				temp++;

			// temp��Ϊ��ȥ��white space
			if( (s[j]==' ' || s[j]=='\t')
				&& ( j+1 ) < len
				&& s[j+1] != ' ' && s[j+1]!='\t' && temp %2==0)
			{
				ret[i] = s.substr(j+1, len-j-1);	
				break;
			}
		}
	}
	return ret;
}
vector<string> FileReader::getDefPart()  //cminus.l�ļ���ǰ���һ��c����
{
	fcin.clear();
	fcin.seekg(0, ios::beg); //��ָ���Ƶ��ļ���λ��

	string s;
	definePartNum = 0;
	while(fcin)
	{
		getline(fcin, s);
		if (s == "%{" || s.size()==0)
		{
			continue;
		}
		if (s == "%}")
		{
			break;
		}

		definePartNum++;
	}

	//string* ret = new string[definePartNum];
	vector<string> ret(definePartNum);
	fcin.clear();
	fcin.seekg(0, ios::beg); //��ָ���Ƶ��ļ���λ��
	int pos = 0;
	while(fcin)
	{
		getline(fcin, s);
		if (s == "%{" || s.size()==0)
		{
			continue;
		}
		if (s == "%}")
		{
			break;
		}
		ret[pos++] = s;
	}
	return ret;
}
vector<string> FileReader::getLastPart() 
{
	fcin.clear();
	fcin.seekg(0, ios::beg); //��ָ���Ƶ��ļ���λ��

	string s;
	bool isFirstTime = true;
	lastPartNum = 0;
	while(fcin)
	{
		getline(fcin, s);
		if (s.size()==0)
		{
			continue;
		}
		if (s == "%%" && isFirstTime)
		{
			isFirstTime = false;
			continue;
		}
		if (s == "%%" && !isFirstTime)
		{
			lastPartNum++;
		}
		if(lastPartNum)
		{
			lastPartNum++;
		}
	}
	lastPartNum -= 2; 

	vector<string> ret(lastPartNum);

	fcin.clear();
	fcin.seekg(0, ios::beg); //��ָ���Ƶ��ļ���λ��
	int pos = 0;
	isFirstTime = true;
	bool flag = true;
	while(getline(fcin, s))
	{
		//getline(fcin, s);
		if (s.size()==0)
		{
			continue;
		}
		if (s == "%%" && isFirstTime)
		{
			isFirstTime = false;
			continue;
		}
		if (s == "%%" && !isFirstTime)
		{
			flag = false;
			continue;
		}
		if(!flag)
		{
			ret[pos++] = s;
		}	
	}

	return ret;
}
map<string,string> FileReader:: getRegDefPart()
{
	fcin.clear();
	//��λ�ļ�ͷ��
	fcin.seekg(0, ios::beg);

	//�������涨�弰��Ӧ������ʽ
	map<string,string> res;
	//��ǰ��
	string str;

	//��ȡ���涨�弰������ʽ
	char RegDef[20];
	char RegExp[20];
	while ( getline(fcin, str) && str != "%}" );
	while(getline(fcin, str) &&  str!="%%")
	{
		if(str != "")
		{
			sscanf( str.c_str(), "%s%s",&RegDef,&RegExp);
			res[string(RegDef)] = string(RegExp);
		}
	}
	return res ;
}