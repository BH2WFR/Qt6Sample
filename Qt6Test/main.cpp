#include "main.h"
#include "mainwindow.h"

#include <qglobal.h>
#include <QObject>
#include <QApplication>
#include <TerminalBeauty.h>

#include <QDebug>







int main(int argc, char *argv[])
{
QT_HIGH_DPI();
    //QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    MainWindow w;
    
    //* 调试命令行
	QDEBUGNN << "=================== Project Debugging  中文 ================";
    
    A aa;
    aa.MySignal();
    
    std::unique_ptr<A> b1 {std::make_unique<B1>()};
    b1->MySignal();
    
    std::shared_ptr<A> b2 {std::make_shared<B2>()};
    emit b2->MySignal();
    
    w.show();
    return a.exec();
}
