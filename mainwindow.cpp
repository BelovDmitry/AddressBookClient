#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dialog(new CInputDialog())
{
    ui->setupUi(this);
    tableModel = new CTableModel(ui->tableView);

    ui->tableView->setModel(tableModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(dialog, SIGNAL(sendData(QList<QVariant>,int)),
            tableModel, SLOT(appendRow(QList<QVariant>,int)));
    connect(dialog, SIGNAL(complete()), this, SLOT(onComplete()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addRecord_clicked()
{
    dialog->setIndex(tableModel->rowCount(QModelIndex()));
    dialog->show();
}

void MainWindow::on_updateRecord_clicked()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    if (index.isValid())
    {
        QList<QVariant> data = tableModel->getData(index);
        dialog->setData(data, &index);
        dialog->show();
        ui->tableView->selectionModel()->clearCurrentIndex();
        onComplete();
    }
}

void MainWindow::on_deleteRecord_clicked()
{
    auto index = ui->tableView->selectionModel()->currentIndex();
    if (ui->tableView->selectionModel()->currentIndex().isValid())
    {
        if (!QMessageBox::warning(this,
                                  trUtf8("Удаление записи"),
                                  trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                  trUtf8("Да"),
                                  trUtf8("Нет"),
                                  QString(),
                                  0,
                                  1)){
            tableModel->removeRow(index.row());
            ui->tableView->selectionModel()->clearCurrentIndex();
            onComplete();
        }
    }
}

void MainWindow::setData(QByteArray data)
{
    QList<QList<QVariant>> book;
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject root = document.object();
    QJsonArray ja_data = root["data"].toArray();
    if(!ja_data.isEmpty()){
        for(int i = 0; i < ja_data.count(); i++){
            QJsonObject jo_data = ja_data.at(i).toObject();
            QList<QVariant> node;
            node.append(jo_data.value("firstName").toString());
            node.append(jo_data.value("lastName").toString());
            node.append(jo_data.value("patronymic").toString());
            QJsonValue gender = jo_data.value("gender");
            gender.toString() == trUtf8("М") ?
                        node.append(trUtf8("М")) : node.append(trUtf8("Ж"));
            node.append(jo_data.value("phone").toString());
            book.append(node);
        }

        tableModel->insert(book);

        int columnCount = 5;
        auto pad = ui->tableView->width()- ui->tableView->verticalHeader()->width();
        for (int i = 0; i < columnCount; ++i)
            ui->tableView->setColumnWidth(i, pad/columnCount);
    }
}

void MainWindow::setData(QList<QVariant> data)
{
    QList<QList<QVariant>> node;
    if (data.at(3).toString() == trUtf8("М"))
        data[3] = trUtf8("М");
    node.append(data);
    tableModel->insert(node);
}

void MainWindow::onComplete()
{
    QList<QList<QVariant>> data = tableModel->getAllData();
    // setd all data to tcp client
    QJsonDocument document;
    QJsonArray book;

    for(int i = 0; i < data.size(); ++i) {
        QJsonObject node;
        node["firstName"] = data.at(i).at(0).toString();
        node["lastName"] = data.at(i).at(1).toString();
        node["patronymic"] = data.at(i).at(2).toString();
        node["gender"] = data.at(i).at(3).toString();
        node["phone"] = data.at(i).at(4).toString();
        book.append(node);
    }
    QJsonObject obj; obj["data"] = book;
    document.setObject(obj);

    QByteArray payload;
    payload.append(0x02);
    payload.append(document.toJson());
    emit sendData(payload);
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);

    int rowCount = 5;
    auto pad = ui->tableView->width() - ui->tableView->verticalHeader()->width();
    for (int i = 0; i < rowCount; ++i)
        ui->tableView->setColumnWidth(i, pad/rowCount);
}
