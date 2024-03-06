#pragma once
/*****************************************************************//**
 * \file   MySQLConnectionPool.h
 * \brief  实现连接池功能模块
 * 
 * \author 53208
 * \date   March 2024
 *********************************************************************/
/**
 * .数据库只有一个连接池实例，用单例模式实现
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
	//获取实例接口
	static ConnectionPool* getConnectionPool();
	//给外部提供一个接口，从连接池中获取一个可用的空闲连接,返回智能指针
	shared_ptr<Connection> getConnection();
private:
	//单例模式，构造函数私有化
	ConnectionPool();
	//加载配置文件
	bool loadConfigFile();
	//运行在独立的线程中，专门负责生产新连接
	void produceConnectionTask();

	string m_IP;				//mysql IP地址
	int m_port;					//mysql 端口号
	string m_userName;			//mysql 用户名
	string m_passWord;			//mysql 登陆密码
	string m_dbName;			//mysql 数据库名称
	int m_initSize;				//连接池初始连接量
	int m_maxSize;				//连接池最大连接量
	int m_maxIdleTime;			//连接池最大空闲时间
	int m_connectionTimeOut;	//连接池获取连接超时时间

	queue<Connection*> m_connectionQue;	//存储mysql连接的队列
	mutex m_queueMutex;					//维护连接队列的线程安全互斥锁
	atomic_int m_connectionCnt;			//记录创建的连接数量，atomic_int是线程安全的，可以多线程累加
	condition_variable cv;				//设置条件变量，用于连接生产线程和连接消费线程通信
};

