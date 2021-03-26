#ifndef SQLQUERYCONTAINERELEMENT_H
#define SQLQUERYCONTAINERELEMENT_H

#include <sqlfield.h>
#include <sqlvalue.h>

template<SqlField... sfs>
class SqlQueryContainerElement_
{};

template<SqlField sf1, SqlField... sf2s>
class SqlQueryContainerElement_<sf1, sf2s...> : public SqlQueryContainerElement_<sf2s...>
{
    typedef typename SqlFieldInfo<sf1>::value_type T1;

public:
    template<typename... SqlValueT>
    SqlQueryContainerElement_(SqlValue sqlValue1, SqlValueT... sqlValue2s)
        :SqlQueryContainerElement_<sf2s...>(sqlValue2s...),mSqlValue(sqlValue1)
    {}

    template<SqlField sf>
    SqlValue get()
    {
        return SqlQueryContainerElement_<sf2s...>::get<sf>();
    }

    template<>
    SqlValue get<sf1>()
    {
        return mSqlValue;
    }

private:
    SqlValue mSqlValue;
};

template<SqlField... sfs>
using SqlQueryContainerElement = SqlQueryContainerElement_<sfs...>;

#endif // SQLQUERYCONTAINERELEMENT_H
