#ifndef SQLFIELD_H
#define SQLFIELD_H

#include <QString>
#include <QVariant>

#include <boost/locale.hpp>

#include <unit.h>

enum class SqlField
{
    t_forecast_user_id,
    t_forecast_user_name,
    t_forecast_user_info,
    t_forecast_user_info2,
    t_forecast_user_info3
};

//
template<SqlField sf>
struct SqlFieldToQString
{
    QString invalidValueString()
    {
        return "-";
    }

    template<typename T>
    QString operator()(T value, QString const& valueQStr, bool withUnit = false)
    {
        if(!SqlFieldInfo<sf>::isValidValue(value))
        {
            return invalidValueString();
        }
        else
        {
            return withUnit ? (valueQStr + SqlFieldInfo<sf>::unit()) : valueQStr;
        }
    }
};

//
template<SqlField sf>
class SqlFieldInfo;

template<>
class SqlFieldInfo<SqlField::t_forecast_user_id>
{
public:
    typedef int value_type;

    static QString name()
    {
        return QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("id", "gbk"));
    }

    static QString unit()
    {
        return UnitEmpty::toString();
    }

    static bool isValidValue(value_type value)
    {
        QString valueQStr = QVariant(value).toString();
        if(valueQStr.isEmpty())
        {
            return false;
        }
        return true;
    }

    static QString toString(value_type value, bool withUnit = false)
    {
        QString valueQStr = QVariant(value).toString();
        return SqlFieldToQString<SqlField::t_forecast_user_id>()(value, valueQStr, withUnit);
    }
};

template<>
class SqlFieldInfo<SqlField::t_forecast_user_name>
{
public:
    typedef QString value_type;

    static QString name()
    {
        return QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("名称", "gbk"));
    }

    static QString unit()
    {
        return UnitEmpty::toString();
    }

    static bool isValidValue(value_type value)
    {
        QString valueQStr = QVariant(value).toString();
        if(valueQStr.isEmpty())
        {
            return false;
        }
        return true;
    }

    static QString toString(value_type value, bool withUnit = false)
    {
        QString valueQStr = QVariant(value).toString();
        return SqlFieldToQString<SqlField::t_forecast_user_name>()(value, valueQStr, withUnit);
    }
};

template<>
class SqlFieldInfo<SqlField::t_forecast_user_info>
{
public:
    typedef QString value_type;

    static QString name()
    {
        return QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("信息", "gbk"));
    }

    static QString unit()
    {
        return UnitEmpty::toString();
    }

    static bool isValidValue(value_type value)
    {
        QString valueQStr = QVariant(value).toString();
        if(valueQStr.isEmpty())
        {
            return false;
        }
        return true;
    }

    static QString toString(value_type value, bool withUnit = false)
    {
        QString valueQStr = QVariant(value).toString();
        return SqlFieldToQString<SqlField::t_forecast_user_info>()(value, valueQStr, withUnit);
    }
};

template<>
class SqlFieldInfo<SqlField::t_forecast_user_info2>
{
public:
    typedef QString value_type;

    static QString name()
    {
        return QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("信息2", "gbk"));
    }

    static QString unit()
    {
        return UnitEmpty::toString();
    }

    static bool isValidValue(value_type value)
    {
        QString valueQStr = QVariant(value).toString();
        if(valueQStr.isEmpty())
        {
            return false;
        }
        return true;
    }

    static QString toString(value_type value, bool withUnit = false)
    {
        QString valueQStr = QVariant(value).toString();
        return SqlFieldToQString<SqlField::t_forecast_user_info2>()(value, valueQStr, withUnit);
    }
};

template<>
class SqlFieldInfo<SqlField::t_forecast_user_info3>
{
public:
    typedef QString value_type;

    static QString name()
    {
        return QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("信息3", "gbk"));
    }

    static QString unit()
    {
        return UnitEmpty::toString();
    }

    static bool isValidValue(value_type value)
    {
        QString valueQStr = QVariant(value).toString();
        if(valueQStr.isEmpty())
        {
            return false;
        }
        return true;
    }

    static QString toString(value_type value, bool withUnit = false)
    {
        QString valueQStr = QVariant(value).toString();
        return SqlFieldToQString<SqlField::t_forecast_user_info3>()(value, valueQStr, withUnit);
    }
};

#endif // SQLFIELD_H
