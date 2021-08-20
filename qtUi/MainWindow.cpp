//
// Created by lining on 8/14/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <unistd.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow) {
    mainControlBoard = new MainControlBoard();
    clientUdp_test = new ClientUdp(serverConfig_test.ip, serverConfig_test.port,
                                   nullptr, mainControlBoard);
    clientUdp_test->Open();
    clientUdp_test->Run();
    thread_processRecv_ClientUdp_test = std::thread(ThreadProcessRecv_ClientUdp_test, this);
    thread_processRecv_ClientUdp_test.detach();

    ui->setupUi(this);

    QObject::connect(this, SIGNAL(UpdateResult(QString)), this, SLOT(on_UpdateResult(QString)));

//    this->setFocusPolicy(Qt::StrongFocus);

    //限定输入范围

    //freq 0-999999999
    ui->lineEdit_testBoard_setFreq_value->setValidator(new QDoubleValidator(0, 9999999999, 9, this));
    //atten 0-99999
    ui->lineEdit_testBoard_setAtten_value->setValidator(new QIntValidator(0, 99999, this));
    //ref 1-2
    ui->lineEdit_testBoard_setRef_value->setValidator(new QIntValidator(1, 2, this));
    //vcc 0-3300
    ui->lineEdit_testBoard_setVcc_value->setValidator(new QIntValidator(0, 3300, this));
    //startfreq 0-999999999
    ui->lineEdit_testBoard_setStartFreq_value->setValidator(new QDoubleValidator(0, 9999999999, 9, this));
    //endfreq 0-999999999
    ui->lineEdit_testBoard_setEndFreq_value->setValidator(new QDoubleValidator(0, 999999999, 9, this));
    //level 0-255
    ui->lineEdit_testBoard_setLevel_value->setValidator(new QIntValidator(0, 255, this));
    //mode 0-255
    ui->lineEdit_testBoard_setMode_value->setValidator(new QIntValidator(0, 255, this));
    //workMode 0-255
    ui->lineEdit_testBoard_setWorkMode_value->setValidator(new QIntValidator(0, 255, this));
    //antMode 0-255
    ui->lineEdit_testBoard_setAntMode_value->setValidator(new QIntValidator(0, 255, this));
}

MainWindow::~MainWindow() {
    delete clientUdp_test;
    delete mainControlBoard;

    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
//    QWidget::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_Up: {
            cout << "key up" << endl;

//            upDownCount--;
//            if (upDownCount <= 0) {
//                upDownCount = 0;
//            }
//            cout << "upDownCount:" << to_string(upDownCount) << endl;
            this->focusPreviousChild();
        }
            break;
        case Qt::Key_Down: {
            cout << "key down" << endl;

//            upDownCount++;
//            if (upDownCount >= 9) {
//                upDownCount = 9;
//            }
//            cout << "upDownCount:" << to_string(upDownCount) << endl;
            this->focusNextChild();
        }
            break;
        default: {

        }
            break;
    }
}


void MainWindow::on_UpdateResult(QString info) {
    //最多100行
    if (ui->plainTextEdit_result->blockCount() >=100){
        ui->plainTextEdit_result->clear();
    }
    ui->plainTextEdit_result->appendPlainText(info);
}

void MainWindow::on_lineEdit_testBoard_setFreq_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setFreq_value->text();
    uint64_t value = valueStr.toLong();
    cout << "set freq:" << to_string(value) << " kHz" << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardFreq(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setAtten_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setAtten_value->text();
    uint64_t value = valueStr.toLong();
    cout << "set atten:" << to_string(value / 10) << " dB" << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardAtten(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setRef_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setRef_value->text();
    uint8_t value = valueStr.toLong();

    uint8_t ref = 0x80;//内参考
    if (value == 1) {
        ref = 0x80;//内参考
    } else if (value == 2) {
        ref = 0x81;//外参考
    }

    cout << "set ref:" << to_string(ref) << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardRef(msg, ref);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setVcc_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setVcc_value->text();
    uint64_t value = valueStr.toLong();
    cout << "set vcc:" << to_string(value) << " mv" << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardVcc(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setStartFreq_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setStartFreq_value->text();
    uint32_t value = valueStr.toLong();
    cout << "set startFreq:" << to_string(value) << " kHz" << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardStartFreq(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setEndFreq_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setEndFreq_value->text();
    uint32_t value = valueStr.toLong();
    cout << "set endFreq:" << to_string(value) << " kHz" << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardEndFreq(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setLevel_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setLevel_value->text();
    uint8_t value = valueStr.toLong();
    cout << "set level:" << to_string(value) << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardLevel(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setMode_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setMode_value->text();
    uint8_t value = valueStr.toLong();
    cout << "set mode:" << to_string(value) << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardMode(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setWorkMode_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setWorkMode_value->text();
    uint8_t value = valueStr.toLong();
    cout << "set workMode:" << to_string(value) << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardWorkMode(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::on_lineEdit_testBoard_setAntMode_value_returnPressed() {
    QString valueStr = ui->lineEdit_testBoard_setAntMode_value->text();
    uint8_t value = valueStr.toLong();
    cout << "set antMode:" << to_string(value) << endl;

    //set testBoard freq
    if (clientUdp_test->isRun) {
        ClientUdp::Msg msg;
        mainControlBoard->GetInfo_SetTestBoardAntMode(msg, value);
        clientUdp_test->Send(msg);
    }
}

void MainWindow::ThreadProcessRecv_ClientUdp_test(void *p) {
    if (p == nullptr) {
        return;
    }

    auto local = (MainWindow *) p;
    cout << "MainWindow " << __FUNCTION__ << " run" << endl;
    while (local->clientUdp_test->isRun) {
        usleep(10 * 1000);
        if (!local->clientUdp_test->isRun) {
            continue;
        }
        if (local->clientUdp_test->queue_recv.empty()) {
            continue;
        }

        pthread_mutex_lock(&local->clientUdp_test->lock_recv);
        while (local->clientUdp_test->queue_recv.empty()) {
            pthread_cond_wait(&local->clientUdp_test->cond_recv,
                              &local->clientUdp_test->lock_recv);
        }
        //recv task
        while (!local->clientUdp_test->queue_recv.empty()) {
            ClientUdp::Msg msg;
            msg = local->clientUdp_test->queue_recv.front();
            local->clientUdp_test->queue_recv.pop();
            //parse
            InfoBase infoBase;
            GetInfo(msg.buf, msg.len, infoBase);

            if (infoBase.dev != DT_Simple_Process) {
                continue;
            }
            switch (infoBase.cmd) {
                case Rsp_Set: {
                    uint16_t paramKey = 0;
                    uint8_t result;
                    int index = 0;
                    memcpy(&paramKey, &infoBase.data[index], sizeof(paramKey));
                    index += sizeof(paramKey);
                    memcpy(&result, &infoBase.data[index], sizeof(result));
                    index += sizeof(result);

                    //按照参数体内关键字和结果，打印输出到结果ui
                    QString show;
                    switch (paramKey) {
                        case ParamKey_Test_Freq: {
                            if (result == Result_Success) {
                                show = "设置频率成功";
                            } else if (result == Result_Fail) {
                                show = "设置频率失败";
                            }
                        }
                            break;
                        case ParamKey_Test_Atten: {
                            if (result == Result_Success) {
                                show = "设置衰减成功";
                            } else if (result == Result_Fail) {
                                show = "设置衰减失败";
                            }
                        }
                            break;
                        case ParamKey_Test_Ref: {
                            if (result == Result_Success) {
                                show = "设置参考切换成功";
                            } else if (result == Result_Fail) {
                                show = "设置参考切换失败";
                            }
                        }
                            break;
                        case ParamKey_Test_Vcc: {
                            if (result == Result_Success) {
                                show = "设置偏置电压成功";
                            } else if (result == Result_Fail) {
                                show = "设置偏置电压失败";
                            }
                        }
                            break;
                        case ParamKey_Test_StartFreq: {
                            if (result == Result_Success) {
                                show = "设置开始频率成功";
                            } else if (result == Result_Fail) {
                                show = "设置开始频率失败";
                            }
                        }
                            break;
                        case ParamKey_Test_EndFreq: {
                            if (result == Result_Success) {
                                show = "设置结束频率成功";
                            } else if (result == Result_Fail) {
                                show = "设置结束频率失败";
                            }
                        }
                            break;
                        case ParamKey_Test_Level: {
                            if (result == Result_Success) {
                                show = "设置等级成功";
                            } else if (result == Result_Fail) {
                                show = "设置等级失败";
                            }
                        }
                            break;
                        case ParamKey_Test_Mode: {
                            if (result == Result_Success) {
                                show = "设置模式成功";
                            } else if (result == Result_Fail) {
                                show = "设置模式失败";
                            }
                        }
                            break;
                        case ParamKey_Test_WorkMode: {
                            if (result == Result_Success) {
                                show = "设置工作模式成功";
                            } else if (result == Result_Fail) {
                                show = "设置工作模式失败";
                            }
                        }
                            break;
                        case ParamKey_Test_AntMode: {
                            if (result == Result_Success) {
                                show = "设置天线模式成功";
                            } else if (result == Result_Fail) {
                                show = "设置天线模式失败";
                            }
                        }
                            break;
                        default: {
                            //未知
                            show = "未知关键字";
                        }
                            break;
                    }
                    emit local->UpdateResult(show);
                }
                    break;
                case Rsp_GetAll: {

                }
                    break;
                case Rsp_Get: {

                }
                    break;
                case Rsp_CheckControl: {

                }
                    break;
                case Rsp_GetHealth: {

                }
                    break;
                case CheckResult_Report: {

                }
                    break;
                default: {
                    cout << "unknown cmd:" << to_string(infoBase.cmd) << endl;
                }
                    break;
            }

        }

        pthread_mutex_unlock(&local->clientUdp_test->lock_recv);
    }

    cout << "MainWindow " << __FUNCTION__ << " exit" << endl;

}

