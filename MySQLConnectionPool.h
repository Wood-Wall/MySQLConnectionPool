#pragma once
/*****************************************************************//**
 * \file   MySQLConnectionPool.h
 * \brief  ʵ�����ӳع���ģ��
 * 
 * \author 53208
 * \date   March 2024
 *********************************************************************/
/**
 * .���ݿ�ֻ��һ�����ӳ�ʵ�����õ���ģʽʵ��
 */
#include<string>
#include<queue>
#include<mutex>
#include<atomic>
#include<thread>
#include<condition_variable>
#include<memory>
#include<functional>
#include"Connection.h"
using namespace std;

class ConnectionPool
{
public:
	//��ȡʵ���ӿ�
	static ConnectionPool* getConnectionPool();
	//���ⲿ�ṩһ���ӿڣ������ӳ��л�ȡһ�����õĿ�������,��������ָ��
	shared_ptr<Connection> getConnection();
private:
	//����ģʽ�����캯��˽�л�
	ConnectionPool();
	//���������ļ�
	bool loadConfigFile();
	//�����ڶ������߳��У�ר�Ÿ�������������
	void produceConnectionTask();

	string m_IP;				//mysql IP��ַ
	int m_port;					//mysql �˿ں�
	string m_userName;			//mysql �û���
	string m_passWord;			//mysql ��½����
	string m_dbName;			//mysql ���ݿ�����
	int m_initSize;				//���ӳس�ʼ������
	int m_maxSize;				//���ӳ����������
	int m_maxIdleTime;			//���ӳ�������ʱ��
	int m_connectionTimeOut;	//���ӳػ�ȡ���ӳ�ʱʱ��

	queue<Connection*> m_connectionQue;	//�洢mysql���ӵĶ���
	mutex m_queueMutex;					//ά�����Ӷ��е��̰߳�ȫ������
	atomic_int m_connectionCnt;			//��¼����������������atomic_int���̰߳�ȫ�ģ����Զ��߳��ۼ�
	condition_variable cv;				//���������������������������̺߳����������߳�ͨ��
};

