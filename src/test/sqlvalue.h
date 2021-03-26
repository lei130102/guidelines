#ifndef SQLVALUE_H
#define SQLVALUE_H

#include <QVariant>

#include <sqlfield.h>
#include <sqlvaluewrapper.h>

class SqlValue
{
public:

    template<SqlField sf, typename T>
    static typename std::enable_if<!std::is_same<T, QVariant>::value, SqlValue>::type create(T v)
    {
        SqlValue sqlValue;
        sqlValue.mWrapper = new SqlValueImp<sf>(v);
        return sqlValue;
    }

    template<SqlField sf, typename T>
    static typename std::enable_if<std::is_same<T, QVariant>::value, SqlValue>::type create(T v)
    {
        SqlValue sqlValue;
        sqlValue.mWrapper = new SqlValueImp<sf>(v.value<SqlFieldInfo<sf>::value_type>());
        return sqlValue;
    }

    SqlValue():mWrapper(nullptr)
    {}

    ~SqlValue()
    {
        delete mWrapper;
    }

    SqlValue(SqlValue const& rhs)
    {
        if(rhs.isValid() == true)
        {
            mWrapper = rhs.mWrapper->copy();
        }
    }

    SqlValue& operator=(SqlValue const& rhs)
    {
        SqlValue(rhs).swap(*this);
        return *this;
    }

    std::type_info const& type() const
    {
        if(isValid())
        {
            return mWrapper->type();
        }

        return typeid(void);
    }

    bool isValid() const
    {
        return (mWrapper != nullptr);
    }

    template<typename T>
    bool value(T& t) const
    {
        T const* tmp = pointer_to_value<T>();
        if(tmp != nullptr)
        {
            t = *tmp;
            return true;
        }
        return false;
    }

    template<typename T>
    T const* pointer_to_value() const
    {
        if(type() == typeid(T))
        {
            return reinterpret_cast<T*>(pointer_to_value_as_void());
        }
        return nullptr;
    }

    template<typename T>
    T* pointer_to_value()
    {
        return const_cast<T*>(static_cast<SqlValue const*>(this)->pointer_to_value<T>());
    }

    void* pointer_to_value_as_void() const
    {
        if(isValid())
        {
            return mWrapper->value();
        }
        else
        {
            return nullptr;
        }
    }

    QString toString(bool withUnit = false) const
    {
        return mWrapper->toString(withUnit);
    }

    QString title() const
    {
        return mWrapper->title();
    }

    QString unit() const
    {
        return mWrapper->unit();
    }

    SqlValue& swap(SqlValue& rhs)
    {
        std::swap(mWrapper, rhs.mWrapper);
        return *this;
    }

private:
    SqlValueWrapper* mWrapper;
};

#endif // SQLVALUE_H
