#pragma once
#include<mysql.h>
#include<string>
#include<ctime>
using namespace std;
/*****************************************************************//**
 * \file   Connection.h
 * \brief  ʵ�����ݿ����
 * 
 * \author 53208
 * \date   March 2024
 *********************************************************************/

class Connection
{
public:
	Connection();
	~Connection();

	//�������ݿ�
	bool connect(const string &IP, unsigned short port,const string &user, const string &passWord,const string &dbName);
	//���²���
	bool update(const string &sql);
	//��ѯ����
	MYSQL_RES *query(const string &sql);
	//ˢ��һ�� ���� ����ʱ�ε���ʼʱ��(������е�ʱ���)
	void refreshAliveTime()
	{
		m_aliveTime = clock();
	}
	//���ؿ���ʱ��
	clock_t getAliveTime()
	{
		return clock() - m_aliveTime;
	}

private:
	MYSQL * m_conn;
	clock_t m_aliveTime; //��¼���ڿ���״̬��ʱ��
};
