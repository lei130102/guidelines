#ifndef SQLQUERYCONTAINER_H
#define SQLQUERYCONTAINER_H

#include <sqlquerycontainerelement.h>

#include <integer_sequence.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

#include <iterator>
#include <memory>

template<typename ElementT, SqlField... sfs>
class SqlQueryContainerIterator_;

template<typename ElementT, SqlField... sfs>
class SqlQueryContainer_
{
    friend class SqlQueryContainerIterator_<ElementT, sfs...>;

public:
    SqlQueryContainer_()
    {}

    SqlQueryContainer_(
            QString const& sqlSelect,
            QString const& sqlInsert,
            QString const& sqlDelete,
            QString const& type,
            QString const& hostName,
            QString const& databaseName,
            QString const& userName,
            QString const& passWord)
        :mSqlSelect(sqlSelect),
          mSqlInsert(sqlInsert),
          mSqlDelete(sqlDelete)
    {
        mSqlDatabase = QSqlDatabase::addDatabase(type);
        mSqlDatabase.setHostName(hostName);
        mSqlDatabase.setDatabaseName(databaseName);
        mSqlDatabase.setUserName(userName);
        mSqlDatabase.setPassword(passWord);

        initSqlQuery();
    }

    ~SqlQueryContainer_()
    {
        mSqlDatabase.close();
    }

    void setSqlInsert(QString const& sqlInsert)
    {
        mSqlInsert = sqlInsert;
    }

    void setSqlDelete(QString const& sqlDelete)
    {
        mSqlDelete = sqlDelete;
    }

    //begin
    SqlQueryContainerIterator_<ElementT, sfs...> begin()
    {
        return SqlQueryContainerIterator_<ElementT, sfs...>(this);
    }

    //end
    SqlQueryContainerIterator_<ElementT, sfs...> end()
    {
        SqlQueryContainerIterator_<ElementT, sfs...> iter(this);
        iter.mRowIndex = mSqlQuery.size();
        return iter;
    }

    //insert
    void insert(SqlQueryContainerElement_<sfs...>&& element)
    {
        dbHelper<typename SqlFieldInfo<sfs>::value_type...>()(this, mSqlInsert, (*(element.get<sfs>().pointer_to_value<SqlFieldInfo<sfs>::value_type>()))..., make_index_sequence<sizeof...(sfs)>{});

        initSqlQuery();
    }

    //针对主键是自产生的情况
    template<SqlField... sfs_subset>
    typename std::enable_if<sizeof...(sfs_subset) != sizeof...(sfs)>::type insert(SqlQueryContainerElement_<sfs_subset...>&& element)
    {
        dbHelper<typename SqlFieldInfo<sfs_subset>::value_type...>()(this, mSqlInsert, (*(element.get<sfs_subset>().pointer_to_value<SqlFieldInfo<sfs_subset>::value_type>()))..., make_index_sequence<sizeof...(sfs_subset)>{});

        initSqlQuery();
    }

    //erase
    void erase(SqlQueryContainerIterator_<ElementT, sfs...> iter)
    {
        dbHelper<typename SqlFieldInfo<sfs>::value_type...>()(this, mSqlDelete, (*(iter->get<sfs>().pointer_to_value<SqlFieldInfo<sfs>::value_type>()))..., make_index_sequence<sizeof...(sfs)>{});

        initSqlQuery();
    }

protected:
    void initSqlQuery()
    {
        bool isOpen = mSqlDatabase.open();
        if(isOpen)
        {
            QSqlQuery sqlQuery(mSqlDatabase);
            sqlQuery.exec(mSqlSelect);
            mSqlQuery = sqlQuery;
        }
        else
        {
            QString qstrError = mSqlDatabase.lastError().text();
            qDebug() << qstrError;
        }
    }

private:
    template<typename... Ts>
    struct dbHelper
    {
        template<std::size_t... is>
        void operator()(SqlQueryContainer_* pContainer, QString const& sql, Ts... ts, index_sequence<is...>)
        {
            pContainer->mSqlQuery.prepare(sql);
            std::initializer_list<int>{(pContainer->mSqlQuery.bindValue(is, QVariant(ts)), 0)...};
            bool isExec = pContainer->mSqlQuery.exec();
            if(!isExec)
            {
                QString qstrError = pContainer->mSqlQuery.lastError().text();
                qDebug() << qstrError;
            }
        }
    };

protected:
    QString mSqlSelect;
    QString mSqlInsert;
    QString mSqlDelete;
    QSqlDatabase mSqlDatabase;

private:
    QSqlQuery mSqlQuery;
};

template<typename ElementT, SqlField... sfs>
class SqlQueryContainerIterator_ : public std::iterator<std::bidirectional_iterator_tag, ElementT>
{
    friend class SqlQueryContainer_<ElementT, sfs...>;

public:
    SqlQueryContainerIterator_()
        :mpSqlQueryContainer(nullptr), mRowIndex(0)
    {}

    explicit SqlQueryContainerIterator_(SqlQueryContainer_<ElementT, sfs...>* pSqlQueryContainer)
        :mpSqlQueryContainer(pSqlQueryContainer), mRowIndex(0)
    {}

    SqlQueryContainerIterator_(SqlQueryContainerIterator_ const& rhs) = default;
    SqlQueryContainerIterator_& operator=(SqlQueryContainerIterator_ const& rhs) = default;

    SqlQueryContainerElement_<sfs...>& operator*()
    {
        return helper(make_index_sequence<sizeof...(sfs)>{});
    }

    SqlQueryContainerElement_<sfs...>* operator->()
    {
        return &operator*();
    }

    SqlQueryContainerIterator_& operator++()
    {
        if(mpSqlQueryContainer != nullptr && mpSqlQueryContainer->mSqlQuery.seek(mRowIndex))
        {
            ++mRowIndex;
            return *this;
        }
        else
        {
            throw std::logic_error("SqlQueryContainerElement& operator++() invalid");
        }
    }

    SqlQueryContainerIterator_ operator++(int)
    {
        if(mpSqlQueryContainer != nullptr && mpSqlQueryContainer->mSqlQuery.seek(mRowIndex))
        {
            auto tmp = *this;
            ++mRowIndex;
            return tmp;
        }
        else
        {
            throw std::logic_error("SqlQueryContainerIterator operator++(int) invalid");
        }
    }

    SqlQueryContainerIterator_& operator--()
    {
        if(mRowIndex > 0)
        {
            --mRowIndex;
            return *this;
        }
        else
        {
            throw std::logic_error("SqlQueryContainerIterator& operator--() invalid");
        }
    }

    SqlQueryContainerIterator_ operator--(int)
    {
        if(mRowIndex > 0)
        {
            auto tmp = *this;
            --mRowIndex;
            return tmp;
        }
        else
        {
            throw std::logic_error("SqlQueryContainerIterator operator--(int) invalid");
        }
    }

    bool operator==(SqlQueryContainerIterator_ const& iter) const
    {
        return ((mpSqlQueryContainer == iter.mpSqlQueryContainer) && (mRowIndex == iter.mRowIndex));
    }

    bool operator!=(SqlQueryContainerIterator_ const& iter) const
    {
        return !(operator==(iter));
    }

private:
    template<std::size_t... is>
    SqlQueryContainerElement_<sfs...>& helper(index_sequence<is...>)
    {
        if(mpSqlQueryContainer != nullptr && mpSqlQueryContainer->mSqlQuery.seek(mRowIndex))
        {
            mpSqlQueryContainerElement = std::make_shared<SqlQueryContainerElement_<sfs...>>(
                                             SqlValue::create<sfs>(mpSqlQueryContainer->mSqlQuery.value(is))...
                                                                                               );
            return *mpSqlQueryContainerElement.get();
        }
        else
        {
            throw std::logic_error("SqlQueryContainerElement_<sfs...>& operator*() invalid");
        }
    }

private:
    SqlQueryContainer_<ElementT, sfs...>* mpSqlQueryContainer;

    std::shared_ptr<SqlQueryContainerElement_<sfs...>> mpSqlQueryContainerElement;
    int mRowIndex;
};

template<SqlField... sfs>
using SqlQueryContainer = SqlQueryContainer_<SqlQueryContainerElement<sfs...>, sfs...>;

template<SqlField... sfs>
using SqlQueryContainerIterator = SqlQueryContainerIterator_<SqlQueryContainerElement<sfs...>, sfs...>;

#endif // SQLQUERYCONTAINER_H
