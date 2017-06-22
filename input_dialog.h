#ifndef INPUT_DIALOG_H
#define INPUT_DIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QList>
#include <QVariant>

namespace Ui {
class CInputDialog;
}

class CInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CInputDialog(QDialog *parent = 0);
    ~CInputDialog();

    void setIndex(int);

signals:
    void sendData(QList<QVariant> data, int index = -1);
    void complete();

public slots:
    void setData(QList<QVariant> data, QModelIndex *index = Q_NULLPTR);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::CInputDialog *dialog;
    int up_index;
};

#endif // INPUT_DIALOG_H
