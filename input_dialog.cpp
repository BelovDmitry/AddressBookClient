#include "input_dialog.h"
#include "ui_input_dialog.h"

#include <QDebug>
#include <QString>
#include <QChar>
#include <QRegExp>
#include <QValidator>

CInputDialog::CInputDialog(QDialog *parent) :
    QDialog(parent),
    dialog(new Ui::CInputDialog)
{
    dialog->setupUi(this);
    setWindowFlags(Qt::Popup);
    dialog->radioButton_1->setChecked(true);

    QRegExp rx("[_а-яА-Я_a-zA-Z]+");
    QValidator *validator = new QRegExpValidator(rx, this);
    dialog->lineEdit_1->setValidator(validator);
    dialog->lineEdit_2->setValidator(validator);
    dialog->lineEdit_3->setValidator(validator);
    dialog->lineEdit_5->setValidator(new QIntValidator());

}

CInputDialog::~CInputDialog()
{
    delete dialog;
}

void CInputDialog::setIndex(int index)
{
    up_index = index;
}


void CInputDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(dialog->buttonBox->standardButton(button) == QDialogButtonBox::Ok)
    {
        QList<QVariant> node;
        node.append(dialog->lineEdit_1->text());
        node.append(dialog->lineEdit_2->text());
        node.append(dialog->lineEdit_3->text());
        dialog->radioButton_1->isChecked() ?
                    node.append(trUtf8("М")) :
                    node.append(trUtf8("Ж"));
        node.append(dialog->lineEdit_5->text());

        emit sendData(node, up_index);
        up_index = -1;
        emit complete();
    }
    dialog->lineEdit_1->clear();
    dialog->lineEdit_2->clear();
    dialog->lineEdit_3->clear();
    dialog->lineEdit_5->clear();
}

void CInputDialog::setData(QList<QVariant> data, QModelIndex *index)
{
    dialog->lineEdit_1->setText(data.at(0).toString());
    dialog->lineEdit_2->setText(data.at(1).toString());
    dialog->lineEdit_3->setText(data.at(2).toString());
    dialog->lineEdit_5->setText(data.at(4).toString());

    auto gender = data.at(3).toString();
    gender == trUtf8("М") ?
                dialog->radioButton_1->setChecked(true) :
                dialog->radioButton_2->setChecked(true);
    up_index = index->row();
}
