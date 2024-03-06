#pragma once
#include<mysql.h>
#include<string>
#include<ctime>
using namespace std;
/*****************************************************************//**
 * \file   Connection.h
 * \brief  实现数据库操作
 * 
 * \author 53208
 * \date   March 2024
 *********************************************************************/

class Connection
{
public:
	Connection();
	~Connection();

	//连接数据库
	bool connect(const string &IP, unsigned short port,const string &user, const string &passWord,const string &dbName);
	//更新操作
	bool update(const string &sql);
	//查询操作
	MYSQL_RES *query(const string &sql);
	//刷新一下 连接 空闲时段的起始时间(进入队列的时间点)
	void refreshAliveTime()
	{
		m_aliveTime = clock();
	}
	//返回空闲时间
	clock_t getAliveTime()
	{
		return clock() - m_aliveTime;
	}

private:
	MYSQL * m_conn;
	clock_t m_aliveTime; //记录处于空闲状态的时间
};
