#ifndef TABLE_MODEL_H
#define TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>
#include <QObject>

#include "data_unit.h"

class CTableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    CTableModel(QObject*);
    ~CTableModel() = default;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QList<QVariant> getData(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QList<QList<QVariant>> getAllData();

public slots:
    void insert(QList<QList<QVariant>>);
    void appendRow(QList<QVariant> data, int index = -1);

private:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList columnNames;
    QVector<CDataUnit> addressData;
};

#endif // TABLE_MODEL_H
