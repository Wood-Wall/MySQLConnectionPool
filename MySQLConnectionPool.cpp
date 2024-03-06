#include <unordered_map>
#include"MySQLConnectionPool.h" 
#include"Public.h"

//懒汉单例模式出口
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool;
	return &pool;
}

bool ConnectionPool::loadConfigFile()
{
	FILE *pf;
	if (fopen_s(&pf, "mysql.ini", "r"))
	{
		LOG("mysql.ini file is not exist!");
		return false;
	}
	static std::unordered_map<std::string, int*> configMap
	{
			{"port", &m_port},
			{"initSize", &m_initSize},
			{"maxSize", &m_maxSize},
			{"maxIdleTime", &m_maxIdleTime},
			{"connectionTimeOut", &m_connectionTimeOut}
	};
	while (!feof(pf))
	{
		char line[1024] = {0};
		fgets(line, 1024, pf);
		string strLine = line;
		int idx = strLine.find("=", 0);
		if (idx == -1)
			continue;
		int endIdx = strLine.find('\n', idx);
		string key = strLine.substr(0, idx);
		string value = strLine.substr(idx + 1, endIdx - idx - 1);

		auto it = configMap.find(key);
		if (it != configMap.end())
		{
			*(it->second) = atoi(value.c_str());
		}
		else if (key == "IP")
			m_IP = value;
		else if (key == "userName")
			m_userName = value;
		else if (key == "passWord")
			m_passWord = value;
		else if (key == "dbName")
			m_dbName = value;
	}
	return true;
}

//线程池的构造
ConnectionPool::ConnectionPool()
{
	//加载配置项
	if (!loadConfigFile())
	{
		return;
	}

	//创建初始数量的连接
	for (int i = 0; i < m_initSize; i++)
	{
		Connection *p = new Connection();
		p->connect(m_IP, m_port,m_userName, m_passWord, m_dbName);
		m_connectionQue.push(p);
		m_connectionCnt++;
	}

	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
}

void ConnectionPool::produceConnectionTask()
{
	while(true)
	{
		unique_lock<mutex> lock(m_queueMutex);
		while (!m_connectionQue.empty())
		{
			cv.wait(lock);		//队列不空，生产线程进入等待状态
		}

		//连接数量没有到上限，继续创建
		if (m_connectionCnt < m_maxSize)
		{
			Connection *p = new Connection();
			p->connect(m_IP,m_port,m_userName,m_passWord,m_dbName);
			m_connectionQue.push(p);
			m_connectionCnt++;
		}

		//通知消费者线程，可以消费连接了
		cv.notify_all();
	}
}

//给外部提供接口，从连接池中获取一个而可用的空闲连接
shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex> lock(m_queueMutex);
	while (m_connectionQue.empty())
	{
		//别用sleep，wait只要有资源就可以唤醒，sleep强制等待不合理
		if(cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(m_connectionTimeOut)))
		if (m_connectionQue.empty())
		{
			LOG("获取连接失败！");
			return nullptr;
		}
	}

	//为了防止智能指针自动回收掉此连接，需要自定义shared_ptr释放资源的方式，把connection归还
	shared_ptr<Connection> sp(m_connectionQue.front(), 
		[&](Connection *pcon) 
	{	
		//queue作为服务器应用线程调用的对象，要保证线程安全
		unique_lock<mutex> lock(m_queueMutex);
		m_connectionQue.push(pcon); 
	});
	m_connectionQue.pop();
	//通知生产者线程，由生产者线程判断队列是否为空，是否需要生产
	cv.notify_all();

	return sp;
}
