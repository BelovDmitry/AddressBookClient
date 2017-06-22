#include "data_unit.h"

CDataUnit::CDataUnit(int size)
{
    for (int i = 0; i < size; ++i)
        datas.append(QVariant());
}

QVariant CDataUnit::getUnitItem(int index)
{
    return datas.at(index);
}

void CDataUnit::setUnitItem(int index, QVariant data)
{
    if (index < datas.size())
        datas[index] = data;
}
