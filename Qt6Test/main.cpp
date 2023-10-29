#include "main.h"
#include "mainwindow.h"


#include <QApplication>
#include <TerminalBeauty.h>

int main(int argc, char *argv[])
{
    //QT_HIGH_DPI();
    //QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    MainWindow w;
    
    //* 调试命令行
	QDEBUGNN << "=================== Project Debugging  中文 ================";
    
    w.show();
    return a.exec();
}
