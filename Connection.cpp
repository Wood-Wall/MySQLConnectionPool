#include"Public.h"
#include"Connection.h"

//��ʼ�����ݿ�����
Connection::Connection()
{
	m_conn = mysql_init(nullptr);
}

//�ͷ����ݿ�����
Connection::~Connection()
{
	if (!m_conn)
		mysql_close(m_conn);
}

//���Ӳ���
bool Connection::connect(const string &IP, unsigned short port, const string &user, 
	const string &passWord, const string &dbName)
{
	MYSQL *p = mysql_real_connect(m_conn, IP.c_str(), user.c_str(), passWord.c_str(), dbName.c_str(), port, nullptr, 0);
	return p != nullptr;
}

//���²���
bool Connection::update(const string &sql)
{
	//insert\delete\update
	if (mysql_query(m_conn, sql.c_str()))
	{
		LOG("����ʧ��:" + sql);
		return false;
	}
	return true;
}

//��ѯ����
MYSQL_RES* Connection::query(const string &sql)
{
	if (mysql_query(m_conn, sql.c_str()))
	{
		LOG("��ѯʧ��:" + sql);
		return nullptr;
	}
	return mysql_use_result(m_conn);
}
