#ifndef SQLVALUEWRAPPER_H
#define SQLVALUEWRAPPER_H

#include <QString>

#include <sqlfield.h>

class SqlValueWrapper
{
public:
    virtual ~SqlValueWrapper(){}
    virtual SqlValueWrapper* copy() const = 0;
    virtual void* value() = 0;
    virtual std::type_info const& type() const = 0;
    virtual QString title() const = 0;
    virtual QString toString(bool withUnit = false) const = 0;
    virtual QString unit() const = 0;
};

template<SqlField sf>
class SqlValueImp : public SqlValueWrapper
{
public:
    typedef SqlFieldInfo<sf> info_type;

public:
    SqlValueImp(typename info_type::value_type value):
        mValue(value)
    {}

    SqlValueWrapper* copy() const
    {
        return new SqlValueImp(mValue);
    }

    void* value() override
    {
        return &valueReference();
    }

    std::type_info const& type() const
    {
        return typeid(typename info_type::value_type);
    }

    QString toString(bool withUnit = false) const override
    {
        return info_type::toString(mValue, withUnit);
    }

    QString title() const override
    {
        return info_type::name();
    }

    QString unit() const override
    {
        return info_type::unit();
    }

protected:
    typename info_type::value_type const& valueReference() const
    {
        return mValue;
    }

    typename info_type::value_type& valueReference()
    {
        return mValue;
    }

private:
    typename info_type::value_type mValue;
};

#endif // SQLVALUEWRAPPER_H
