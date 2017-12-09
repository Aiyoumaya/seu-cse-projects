#include "windows.h"
#include <iostream>
#include <string>
using namespace std;
int main()
{
	//�������
	HANDLE hFileRead;
	HANDLE hFileWrite;
	//���ݳ���
	DWORD dwDataLen = 100;  
	DWORD written = 0;   
	//���ݻ���
	char buffer[101];   

	//��ʾ������Դ�ļ���Ŀ���ļ���
	string inputFile;
	string outputFile;
	cout<<"Please input the file name of source file:" <<endl;
	cin>>inputFile;
	cout<<"Please input the file name of destination file:" <<endl;
	cin>>outputFile;

	//���ļ�   
	if ((hFileRead=CreateFile(inputFile.c_str(),   
		GENERIC_READ,   
		FILE_SHARE_READ,   
		NULL,   
		OPEN_EXISTING,   
		FILE_ATTRIBUTE_NORMAL,   
		NULL)) == INVALID_HANDLE_VALUE )
	{
		cout<<"The temp to open file makes error! "<<endl;
		return 0;
	}

	if (( hFileWrite=CreateFile(outputFile.c_str(),   
		GENERIC_WRITE,   
		FILE_SHARE_WRITE,   
		NULL,   
		OPEN_ALWAYS,   
		FILE_ATTRIBUTE_NORMAL,   
		NULL) )== INVALID_HANDLE_VALUE)
	{
		cout<<"The temp to open file makes error! "<<endl;
		exit(1);
	}

	while( dwDataLen != 0 )
	{
		//��ȡ����   
		ReadFile(hFileRead,buffer,99,&dwDataLen,NULL);
		//��ǽ���λ��   
		buffer[dwDataLen]='\0';
		SetFilePointer(hFileWrite, 0, NULL, FILE_END);
		WriteFile(hFileWrite,buffer,dwDataLen,&written,NULL);//д���ļ�   
	}
	
	//�رվ��   
	CloseHandle(hFileRead);
	CloseHandle(hFileWrite);

	//����ɹ���ʾ
	cout<<"The operation of copying has terminated normally!"<<endl;
	return 0;
}