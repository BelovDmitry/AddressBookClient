#ifndef DATA_UNIT_H
#define DATA_UNIT_H

#include <QVector>
#include <QVariant>

class CDataUnit
{
public:
    CDataUnit() = default;
    CDataUnit(int size);
    ~CDataUnit() = default;

    QVariant getUnitItem(int);
    void setUnitItem(int, QVariant);

private:

    QVector<QVariant> datas;
};

#endif // DATA_UNIT_H
