//
// Created by lining on 8/20/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Dialog1.h" resolved

#include "dialog1.h"
#include "resource_dialog1.h"
#include <QHBoxLayout>
#include <QWidget>

Dialog1::Dialog1(QWidget *parent) :
        QDialog(parent) {

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    setObjectName(QString(dialog1_objname));

    setWindowTitle(QString(dialog1_objname));
    resize(dialog1_width, dialog1_height);

    //freq
    label_setFreq_title = new QLabel(this);
    lineEdit_setFreq_value = new QLineEdit(this);
    label_setFreq_unit = new QLabel(this);

    label_setFreq_title->setText(QString("设置频率"));
//    label_setFreq_title->move(10,10);
    lineEdit_setFreq_value->setMaxLength(9);
    lineEdit_setFreq_value->setFixedWidth(15);
//    lineEdit_setFreq_value->move(30,10);
    label_setFreq_unit->setText(QString("kHz"));
//    label_setFreq_unit->move(50,10);
    QHBoxLayout hBoxLayout;
    hBoxLayout.setAlignment(Qt::AlignLeft);
    hBoxLayout.addWidget(label_setFreq_title);
    hBoxLayout.addWidget(lineEdit_setFreq_value);
    hBoxLayout.addWidget(label_setFreq_unit);
    this->setLayout(&hBoxLayout);
}

Dialog1::~Dialog1() {

}

void Dialog1::keyPressEvent(QKeyEvent *event) {
//    QDialog::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_Backspace: {
            //隐藏自己
            this->hide();
            //显示父界面
            parentWidget()->show();
        }
            break;
        default: {
            QDialog::keyPressEvent(event);
        }
            break;
    }

}

