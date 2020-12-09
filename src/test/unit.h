#ifndef UNIT_H
#define UNIT_H

#include <QString>

class UnitEmpty
{
public:
    static QString toString()
    {
        return QString();
    }
};

class UnitCentigrade
{
public:
    static QString toString()
    {
        return "℃";
    }
};

class UnitDegree
{
public:
    static QString toString()
    {
        return "°";
    }
};

class UnitMeterPerSecond
{
public:
    static QString toString()
    {
        return "m/s";
    }
};

class UnitHPA
{
public:
    static QString toString()
    {
        return "hPa";
    }
};

class UnitPercent
{
public:
    static QString toString()
    {
        return "%";
    }
};

class UnitMeter
{
public:
    static QString toString()
    {
        return "m";
    }
};

class UnitCentimeter
{
public:
    static QString toString()
    {
        return "cm";
    }
};

class UnitMillimeter
{
public:
    static QString toString()
    {
        return "mm";
    }
};

#endif // UNIT_H
