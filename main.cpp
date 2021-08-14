#include <QApplication>
#include <QDebug>
#include "MainWindow.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug() << "Hello World";
    cout<<"123"<<endl;
    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
