#pragma once
#include<mysql.h>
#include<string>
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
private:
	MYSQL * m_conn;
};
