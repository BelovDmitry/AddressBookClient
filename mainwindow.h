#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QMainWindow>
#include <QResizeEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

#include "table_model.h"
#include "input_dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendData(QByteArray);

private slots:
    void on_addRecord_clicked();
    void on_updateRecord_clicked();
    void on_deleteRecord_clicked();
    void setData(QByteArray);
    void setData(QList<QVariant>);
    void onComplete();

protected:
    void resizeEvent(QResizeEvent*);
    void resizeHeaders();

private:
    Ui::MainWindow *ui;
    CTableModel *tableModel;
    CInputDialog *dialog;
};

#endif // MAINWINDOW_H
