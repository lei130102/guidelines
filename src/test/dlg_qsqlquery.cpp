#include "dlg_qsqlquery.h"
#include "ui_dlg_qsqlquery.h"

#include <QSqlDatabase>
#include <QSqlQuery>

#include <sqlqueryresult.h>
#include <sqlquerycontainer.h>

dlg_qsqlquery::dlg_qsqlquery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qsqlquery)
{
    ui->setupUi(this);
}

dlg_qsqlquery::~dlg_qsqlquery()
{
    delete ui;
}

QString dlg_qsqlquery::get_qstrType() const
{
    return ui->cbType->currentText();
}

QString dlg_qsqlquery::get_qstrHostName() const
{
    return ui->cbHostName->currentText();
}

QString dlg_qsqlquery::get_qstrDatabaseName() const
{
    return ui->cbDatabaseName->currentText();
}

QString dlg_qsqlquery::get_qstrUserName() const
{
    return ui->cbUserName->currentText();
}

QString dlg_qsqlquery::get_qstrPassword() const
{
    return ui->cbPassword->currentText();
}

void dlg_qsqlquery::on_pbSelect_clicked()
{
    //1.打开
    QSqlDatabase sqlDatabase = QSqlDatabase::addDatabase(get_qstrType());
    sqlDatabase.setHostName(get_qstrHostName());
    sqlDatabase.setDatabaseName(get_qstrDatabaseName());
    sqlDatabase.setUserName(get_qstrUserName());
    sqlDatabase.setPassword(get_qstrPassword());

    bool isOpen = sqlDatabase.open();
    if(!isOpen)
    {
        return;
    }

    //2.查询
    QSqlQuery sqlQuery(sqlDatabase);
    QString qstrSql = ui->leSelect->text();
    bool isExec = sqlQuery.exec(qstrSql);

    if(isExec)
    {
        //创建并初始化SqlQueryResult对象
        SqlQueryResult sqlQueryResult;
        sqlQueryResult.setResultName("t_forecast_user");
        while(sqlQuery.next())
        {
            sqlQueryResult.insertRow(
                        SqlValue::create<SqlField::t_forecast_user_id>(sqlQuery.value(0).toInt()),
                        SqlValue::create<SqlField::t_forecast_user_name>(sqlQuery.value(1).toString()),
                        SqlValue::create<SqlField::t_forecast_user_info>(sqlQuery.value(2).toString()),
                        SqlValue::create<SqlField::t_forecast_user_info2>(sqlQuery.value(3).toString()),
                        SqlValue::create<SqlField::t_forecast_user_info3>(sqlQuery.value(4).toString())
                        );
        }

        //使用SqlQueryResult对象
        ui->lwSelectResult->clear();
        for(int rowIndex = 0; rowIndex < sqlQueryResult.rowSize(); ++rowIndex)
        {
            for(int columnIndex = 0; columnIndex < sqlQueryResult.columnSize(); ++columnIndex)
            {
                SqlValue sqlValue = sqlQueryResult.at(rowIndex, columnIndex);
                if(sqlValue.title() == SqlFieldInfo<SqlField::t_forecast_user_id>::name())
                {
                    //SqlFieldInfo<SqlField::t_forecast_user_id>::value_type value = *sqlValue.pointer_to_value<SqlFieldInfo<SqlField::t_forecast_user_id>::value_type>();

                    //sqlValue.toString();

                    ui->lwSelectResult->addItem(sqlValue.toString());
                }
                else if(sqlValue.title() == SqlFieldInfo<SqlField::t_forecast_user_name>::name())
                {
                    ui->lwSelectResult->addItem(sqlValue.toString());
                }
                else if(sqlValue.title() == SqlFieldInfo<SqlField::t_forecast_user_info>::name())
                {
                    ui->lwSelectResult->addItem(sqlValue.toString());
                }
                else if(sqlValue.title() == SqlFieldInfo<SqlField::t_forecast_user_info2>::name())
                {
                    ui->lwSelectResult->addItem(sqlValue.toString());
                }
                else if(sqlValue.title() == SqlFieldInfo<SqlField::t_forecast_user_info3>::name())
                {
                    ui->lwSelectResult->addItem(sqlValue.toString());
                }
            }
        }
    }

    //3.关闭
    sqlDatabase.close();
}

void dlg_qsqlquery::on_pbSelectIntoContainer_clicked()
{
    QString qstrSqlSelect = ui->leSelect->text();
    QString qstrSqlInsert = ui->leInsert->text();
    QString qstrSqlDelete = ui->leDelete->text();
    SqlQueryContainer<
            SqlField::t_forecast_user_id,
            SqlField::t_forecast_user_name,
            SqlField::t_forecast_user_info,
            SqlField::t_forecast_user_info2,
            SqlField::t_forecast_user_info3
            > container(
                qstrSqlSelect,
                qstrSqlInsert,
                qstrSqlDelete,
                get_qstrType(),
                get_qstrHostName(),
                get_qstrDatabaseName(),
                get_qstrUserName(),
                get_qstrPassword()
                );

    ui->lwSelectResult->clear();
    for(auto element : container)
    {
        //element代表一行
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_id>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_name>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info2>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info3>().toString());
    }
}

void dlg_qsqlquery::on_pbInsert_clicked()
{
    QString qstrSqlSelect = ui->leSelect->text();
    QString qstrSqlInsert = ui->leInsert->text();
    QString qstrSqlDelete = ui->leDelete->text();
    SqlQueryContainer<
            SqlField::t_forecast_user_id,
            SqlField::t_forecast_user_name,
            SqlField::t_forecast_user_info,
            SqlField::t_forecast_user_info2,
            SqlField::t_forecast_user_info3
            > container(
                qstrSqlSelect,
                qstrSqlInsert,
                qstrSqlDelete,
                get_qstrType(),
                get_qstrHostName(),
                get_qstrDatabaseName(),
                get_qstrUserName(),
                get_qstrPassword()
                );

    //只会成功执行一次，因为id是主键
    container.insert(SqlQueryContainerElement<
                     SqlField::t_forecast_user_id,
                     SqlField::t_forecast_user_name,
                     SqlField::t_forecast_user_info,
                     SqlField::t_forecast_user_info2,
                     SqlField::t_forecast_user_info3
                     >(
                         SqlValue::create<SqlField::t_forecast_user_id>(10),
                         SqlValue::create<SqlField::t_forecast_user_name>(QString("name")),
                         SqlValue::create<SqlField::t_forecast_user_info>(QString("info")),
                         SqlValue::create<SqlField::t_forecast_user_info2>(QString("info2")),
                         SqlValue::create<SqlField::t_forecast_user_info3>(QString("info3"))
                         ));

    ui->lwSelectResult->clear();
    for(auto element : container)
    {
        //element代表一行
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_id>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_name>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info2>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info3>().toString());
    }
}

void dlg_qsqlquery::on_pbInsertPart_clicked()
{
    QString qstrSqlSelect = ui->leSelect->text();
    QString qstrSqlInsertPart = ui->leInsertPart->text();
    QString qstrSqlDelete = ui->leDelete->text();
    SqlQueryContainer<
            SqlField::t_forecast_user_id,
            SqlField::t_forecast_user_name,
            SqlField::t_forecast_user_info,
            SqlField::t_forecast_user_info2,
            SqlField::t_forecast_user_info3
            > container(
                qstrSqlSelect,
                qstrSqlInsertPart,
                qstrSqlDelete,
                get_qstrType(),
                get_qstrHostName(),
                get_qstrDatabaseName(),
                get_qstrUserName(),
                get_qstrPassword()
                );

    //成功执行多次，因为id是主键，id自增
    container.insert(SqlQueryContainerElement<
                     SqlField::t_forecast_user_name,
                     SqlField::t_forecast_user_info,
                     SqlField::t_forecast_user_info2,
                     SqlField::t_forecast_user_info3
                     >(
                         SqlValue::create<SqlField::t_forecast_user_name>(QString("name")),
                         SqlValue::create<SqlField::t_forecast_user_info>(QString("info")),
                         SqlValue::create<SqlField::t_forecast_user_info2>(QString("info2")),
                         SqlValue::create<SqlField::t_forecast_user_info3>(QString("info3"))
                         ));

    ui->lwSelectResult->clear();
    for(auto element : container)
    {
        //element代表一行
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_id>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_name>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info2>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info3>().toString());
    }
}

void dlg_qsqlquery::on_pbDelete_clicked()
{
    QString qstrSqlSelect = ui->leSelect->text();
    QString qstrSqlInsert = ui->leInsert->text();
    QString qstrSqlDelete = ui->leDelete->text();
    SqlQueryContainer<
            SqlField::t_forecast_user_id,
            SqlField::t_forecast_user_name,
            SqlField::t_forecast_user_info,
            SqlField::t_forecast_user_info2,
            SqlField::t_forecast_user_info3
            > container(
                qstrSqlSelect,
                qstrSqlInsert,
                qstrSqlDelete,
                get_qstrType(),
                get_qstrHostName(),
                get_qstrDatabaseName(),
                get_qstrUserName(),
                get_qstrPassword()
                );

    container.erase(container.begin());

    ui->lwSelectResult->clear();
    for(auto element : container)
    {
        //element代表一行
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_id>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_name>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info2>().toString());
        ui->lwSelectResult->addItem(element.get<SqlField::t_forecast_user_info3>().toString());
    }
}
