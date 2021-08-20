//
// Created by lining on 8/14/21.
//

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "dialog1.h"

#include <QWidget>
#include <QPushButton>

#include "MainControlBoard.h"
#include "ClientUdp.h"
#include <thread>

class MainWindow : public QWidget {
Q_OBJECT

public:
    struct ServerConfig {
        string ip;
        uint16_t port;
    };


public:
    ServerConfig serverConfig_test = {"127.0.0.1", 8313};
    MainControlBoard *mainControlBoard = nullptr;
    ClientUdp *clientUdp_test = nullptr;
    std::thread thread_processRecv_ClientUdp_test;

    static void ThreadProcessRecv_ClientUdp_test(void *p);


public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QPushButton *pushButton_dialog1;//按键按下跳转界面1
    Dialog1 *dialog1;
    QPushButton *pushButton_dialog2;//按键按下跳转界面2
    QPushButton *pushButton_dialog3;//按键按下跳转界面2
    QPushButton *pushButton_dialog4;//按键按下跳转界面2

public:

    int upDownCount = 0;
protected:
    /**
     * 键盘事件
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);

signals:

    void UpdateResult(QString info);

public slots:

    void on_UpdateResult(QString info);

public slots:

    /**
     * 按键1按下
     */
    void on_Btn_dialog1_returnPressed();
    /**
     * 按键2按下
     */
    void on_Btn_dialog2_returnPressed();
    /**
     * 按键3按下
     */
    void on_Btn_dialog3_returnPressed();
    /**
     * 按键4按下
     */
    void on_Btn_dialog4_returnPressed();


    /**
     * 发送频率设置
     */
    void on_lineEdit_testBoard_setFreq_value_returnPressed();

    /**
     * 发送衰减设置
     */
    void on_lineEdit_testBoard_setAtten_value_returnPressed();

    /**
     * 发送参考设置
     */
    void on_lineEdit_testBoard_setRef_value_returnPressed();

    /**
     * 发送偏置电压设置
     */
    void on_lineEdit_testBoard_setVcc_value_returnPressed();

    /**
     * 发送开始频率设置
     */
    void on_lineEdit_testBoard_setStartFreq_value_returnPressed();

    /**
     * 发送结束频率设置
     */
    void on_lineEdit_testBoard_setEndFreq_value_returnPressed();

    /**
     * 发送等级设置
     */
    void on_lineEdit_testBoard_setLevel_value_returnPressed();

    /**
     * 发送模式设置
     */
    void on_lineEdit_testBoard_setMode_value_returnPressed();

    /**
     * 发送工作模式设置
     */
    void on_lineEdit_testBoard_setWorkMode_value_returnPressed();

    /**
     * 发送天线模式设置
     */
    void on_lineEdit_testBoard_setAntMode_value_returnPressed();

};


#endif //_MAINWINDOW_H
