#ifndef HEADERS_H
#define HEADERS_H
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <hash_set>
#include <hash_map>
#include <iomanip>
using namespace std;

//��ASCLL�벻�õĳ���������ʾ���հ������հ�����մ����롢�򡢿մ�
const char STAR = 128;
const char ONE_OR_MORE = 129;
const char ONE_OR_NONE = 130;
const char AND = 131;
const char OR = 132;
const char LEFT_LITTLE_BRACKET = 133;
const char RIGHT_LITTLE_BRACKET = 134;
//����մ�ascll��Ϊ0
const char EPSILON = 0;
//����NFA״̬ת������������NFA�ɽ��շ�������ΪASCLL�� 0 - 127 ��
const int NUM_OF_COLUMNS = 128;
#endif