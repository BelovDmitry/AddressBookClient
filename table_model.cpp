#include "table_model.h"
#include <QVariant>
#include <QString>
#include <QList>
#include <QDebug>

#define DEFAULT_COLUMN_COUNT 5

CTableModel::CTableModel(QObject *parent): QAbstractTableModel(parent)
{
    columnNames << trUtf8("Фамилия")
                << trUtf8("Имя")
                << trUtf8("Отчество")
                << trUtf8("Пол")
                << trUtf8("Телефон");
}

void CTableModel::appendRow(QList<QVariant> data, int index)
{
    int row;
    if (index != -1)
        row = index;
    else
        row = rowCount(QModelIndex());

    for (int i = 0; i < data.size(); ++i)
        setData(createIndex(row,i), data.at(i));
}

void CTableModel::insert(QList<QList<QVariant>> data)
{
    for (int i = 0; i < data.size(); ++i)
        for (int j = 0; j < columnCount(QModelIndex()); ++j) {
            setData(createIndex(i,j), data.at(i).at(j));
        }
}

int CTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return addressData.size();
}

int CTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columnNames.size();
}

QList<QList<QVariant>> CTableModel::getAllData()
{
    QList<QList<QVariant>> book;

    for(int i = 0; i < rowCount(QModelIndex()); ++i)
        book.append(getData(createIndex(i,0)));

    return book;
}

QVariant CTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole || index.row() > addressData.size())
        return QVariant::Invalid;

    CDataUnit p = addressData.at(index.row());
    QVariant data = p.getUnitItem(index.column());
    return data;
}

QList<QVariant> CTableModel::getData(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    QList<QVariant> node;
    for (int i = 0; i < columnCount(index); ++i)
        node.append(data(createIndex(index.row(), i)));

    return node;
}

QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section > columnNames.size() || role != Qt::DisplayRole)
        return QVariant::Invalid;

    if(orientation == Qt::Horizontal) {
        return QVariant(columnNames.at(section));
    } else if(orientation == Qt::Vertical)
        return QVariant(section+1);

    return QVariant::Invalid;
}

bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        if (index.row() >= addressData.size())
        {
            insertRows(0,1,QModelIndex());
            CDataUnit p(columnCount(QModelIndex()));
            p.setUnitItem(index.column(), value);
            addressData.replace(index.row(), p);
        }else{

            CDataUnit p = addressData.at(index.row());
            p.setUnitItem(index.column(), value);
            addressData.replace(index.row(), p);
        }
        emit dataChanged(index, index);

        return true;
    }

    return false;
}

bool CTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    Q_UNUSED(rows);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    CDataUnit data(5);
    addressData.append(data);

    endInsertRows();

    return true;
}

bool CTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        addressData.removeAt(position);
    }

    endRemoveRows();

    return true;
}

Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}


