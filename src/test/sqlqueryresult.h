#ifndef SQLQUERYRESULT_H
#define SQLQUERYRESULT_H

#include <sqlvalue.h>

#include <vector>

class SqlQueryResult
{
public:
    SqlQueryResult()
        :mRowSize(0)
    {}

    void setResultName(QString const& name)
    {
        mName= name;
    }

    QString resultName() const
    {
        return mName;
    }

    std::vector<QString> const& titles() const
    {
        return mTitles;
    }

    template<typename... SqlValueT>
    void insertRow(SqlValue&& value0, SqlValueT&&... values)
    {
        mValues.push_back(std::forward<SqlValue>(value0));
        insertRow(std::forward<SqlValueT>(values)...);
    }

    void insertRow()
    {
        ++mRowSize;
        if(mTitles.empty())
        {
            for(std::size_t columnIndex = 0; columnIndex < columnSize(); ++columnIndex)
            {
                mTitles.push_back(at(0, columnIndex).title());
            }
        }
    }

    std::size_t rowSize() const
    {
        return mRowSize;
    }

    std::size_t columnSize() const
    {
        if(mRowSize == 0)
        {
            return 0;
        }
        return mValues.size()/mRowSize;
    }

    SqlValue at(std::size_t rowIndex, std::size_t columnIndex) const
    {
        return mValues.at(rowIndex * columnSize() + columnIndex);
    }

    template<SqlField sf>
    SqlValue at(std::size_t rowIndex) const
    {
        for(int i = 0; i < rowSize(); ++i)
        {
            if(i == rowIndex)
            {
                for(int j = 0; j < columnSize(); ++j)
                {
                    if(titles().at(j) == SqlFieldInfo<sf>::name())
                    {
                        return at(i, j);
                    }
                }
            }
        }
        return SqlValue();
    }

private:
    QString mName;
    std::vector<SqlValue> mValues;
    std::size_t mRowSize;

    std::vector<QString> mTitles;
};

#endif // SQLQUERYRESULT_H
