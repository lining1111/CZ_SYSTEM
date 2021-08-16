//
// Created by lining on 8/14/21.
//

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QWidget>

#include "MainControlBoard.h"
#include "ClientUdp.h"
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    Ui::MainWindow *ui;


public slots:
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
