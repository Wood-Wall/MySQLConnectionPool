#include"Public.h"
#include"Connection.h"

//初始化数据库连接
Connection::Connection()
{
	m_conn = mysql_init(nullptr);
}

//释放数据库连接
Connection::~Connection()
{
	if (!m_conn)
		mysql_close(m_conn);
}

//连接操作
bool Connection::connect(const string &IP, unsigned short port, const string &user, 
	const string &passWord, const string &dbName)
{
	MYSQL *p = mysql_real_connect(m_conn, IP.c_str(), user.c_str(), passWord.c_str(), dbName.c_str(), port, nullptr, 0);
	return p != nullptr;
}

//更新操作
bool Connection::update(const string &sql)
{
	//insert\delete\update
	if (mysql_query(m_conn, sql.c_str()))
	{
		LOG("更新失败:" + sql);
		return false;
	}
	return true;
}

//查询操作
MYSQL_RES* Connection::query(const string &sql)
{
	if (mysql_query(m_conn, sql.c_str()))
	{
		LOG("查询失败:" + sql);
		return nullptr;
	}
	return mysql_use_result(m_conn);
}
