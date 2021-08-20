//
// Created by lining on 8/20/21.
//

#ifndef _DIALOG1_H
#define _DIALOG1_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>


class Dialog1 : public QDialog {
Q_OBJECT

public:
    explicit Dialog1(QWidget *parent = nullptr);

    ~Dialog1() override;

private:

    //频率
    QLabel *label_setFreq_title;
    QLineEdit *lineEdit_setFreq_value;
    QLabel *label_setFreq_unit;
    //衰减
    QLabel *label_setAtten_title;
    QLineEdit *lineEdit_setAtten_value;
    QLabel *label_setAtten_unit;
    //参考切换
    QLabel *label_setRef_title;
    QLineEdit *lineEdit_setRef_value;
    QLabel *label_setRef_unit;
    //偏置电压
    QLabel *label_setVcc_title;
    QLineEdit *lineEdit_setVcc_value;
    QLabel *label_setVcc_unit;
    //开始频率
    QLabel *label_setStartFreq_title;
    QLineEdit *lineEdit_setStartFreq_value;
    QLabel *label_setStartFreq_unit;
    //结束频率
    QLabel *label_setEndFreq_title;
    QLineEdit *lineEdit_setEndFreq_value;
    QLabel *label_setEndFreq_unit;
    //等级
    QLabel *label_setLevel_title;
    QLineEdit *lineEdit_setLevel_value;
    QLabel *label_setLevel_unit;
    //模式
    QLabel *label_setMode_title;
    QLineEdit *lineEdit_setMode_value;
    QLabel *label_setMode_unit;
    //工作模式
    QLabel *label_setWorkMode_title;
    QLineEdit *lineEdit_setWorkMode_value;
    QLabel *label_setWorkMode_unit;
    //天线模式
    QLabel *label_setAntMode_title;
    QLineEdit *lineEdit_setAntMode_value;
    QLabel *label_setAntMode_unit;

public:
    /**
     * 键盘事件
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);

};


#endif //_DIALOG1_H
