#include <stdio.h> 
#include <windows.h>
#include <iostream>
#include <time.h>
using namespace std;

//���建������������
typedef int buffer_item;
//���建������С
#define BUFFER_SIZE 8 
//����������
buffer_item myBuffer[BUFFER_SIZE];
//�����ź���
HANDLE empty;
HANDLE full;
HANDLE mutex;
//���������ߡ�����֮�������±�
int in = 0, out = 0;

//��ʼ������
void init()
{	
	//��ʼ���ź���
	empty = CreateSemaphore( NULL, 5, 5, NULL );
	full = CreateSemaphore( NULL, 0, 5, NULL );
	mutex = CreateSemaphore( NULL, 1, 1, NULL );
}

//����һ�����ݵ�������
bool insert(buffer_item item)
{
	//�����
	WaitForSingleObject( empty, INFINITE );
	WaitForSingleObject( mutex, INFINITE );
	//�������� 
	myBuffer[ in ] = item;
	in = (in + 1) % 5;
	//�ͷ���
	ReleaseSemaphore( mutex, 1, NULL );
	ReleaseSemaphore( full, 1, NULL );
	return true;
}
//�ӻ������Ƴ�һ������
bool remove(buffer_item *item)
{	
	//�����
	WaitForSingleObject( full, INFINITE );
	WaitForSingleObject( mutex, INFINITE );
	//��������
	*item = myBuffer[out];
	out = (out + 1) % 5;
	//�ͷ���
	ReleaseSemaphore( mutex, 1, NULL );
	ReleaseSemaphore( empty, 1, NULL );
	return true;
}

//������
DWORD WINAPI producer(LPVOID  lpParam)
{
	int num = *(int*)lpParam;
	srand((unsigned)time(0));
	while (true)
	{
		//˯�����ʱ��
		_sleep( rand() % 100 );
		//���������
		int data = rand() % 100;
		//��ӡ��Ϣ
		if ( insert(data) )
			//��num��������
			printf("Producer%d produced %d\n", num, data);
		else
			printf("Warning:produce error\n");
	} 	
}

//������
DWORD WINAPI consumer(LPVOID  lpParam)
{	
	int num = *(int*)lpParam;
	while (true)
	{
		//˯�����ʱ��
		_sleep( rand() % 100 );
		//���������
		int data = 0;
		//��ӡ��Ϣ
		if ( remove(&data) )
			//��num��������
			printf("Consumer%d consumed %d\n", num, data);
		else
			printf("Warning:consumer error\n");
	}	
}

int main(void)
{
	freopen("windowOut.txt","w",stdout);
	//��ʼ���ź���
	init();
	//���� 5 �������ߺ�5���������߳�
	HANDLE producerThread[5] , consumerThread[5];
	int num[5] = {0 ,1, 2, 3, 4};
	for( int i = 0; i < 5; i++)
	{
		producerThread[i] = CreateThread(NULL,0,producer,&num[i],0,NULL); 
		consumerThread[i] = CreateThread(NULL,0,consumer,&num[i],0,NULL); 
	}
	//˯��һ��ʱ��
	_sleep(5000);
	//�˳��߳�
	for( int i = 0; i < 1; i++)
	{
		CloseHandle( producerThread[i] ); 
		CloseHandle( consumerThread[i] ); 
	}
	return 0;
}
