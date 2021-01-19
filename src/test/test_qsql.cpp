#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

BOOST_AUTO_TEST_SUITE(s_qsql)

BOOST_AUTO_TEST_CASE(c_qsql)
{
	//注意MYSQL数据库中表CHAR类型字段对应的QVariant中存放的不是QChar，应该是int

    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

	QSqlDatabase sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
	sqlDatabase.setHostName("localhost");
	sqlDatabase.setDatabaseName("ry-vue");
	sqlDatabase.setUserName("root");
	sqlDatabase.setPassword("");

	bool isOpen = sqlDatabase.open();
	if (isOpen)
	{
		QSqlQuery sqlQuery(sqlDatabase);
		bool isExec = sqlQuery.exec("SELECT sex FROM sys_user");
		if (isExec)
		{
			while (sqlQuery.next())
			{
				//
				//QChar类型
				QVariant v = sqlQuery.value(0);

				QChar c1 = v.toChar();

				QChar c;
				if (v.canConvert<QChar>())
				{
					c = v.value<QChar>();
				}
			}

		}
		else
		{
			QString error = sqlQuery.lastError().text();
			qDebug() << error;
		}
	}
	else
	{
		QString error = sqlDatabase.lastError().text();
		qDebug() << error;
	}

}

BOOST_AUTO_TEST_SUITE_END()