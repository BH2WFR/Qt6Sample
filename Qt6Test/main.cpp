#include "main.h"
#include "mainwindow.h"

#include <qglobal.h>
#include <QObject>
#include <QApplication>
#include <TerminalBeauty.h>




#include <fmt/core.h>
#include <fmt/format.h>




int main(int argc, char *argv[])
{
	QT5_HIGH_DPI();
	CONSOLE_TITLE("Qt6-Test");
	PRINT_ARGV(argc, argv);
	
	QApplication a(argc, argv);
	MainWindow w;
	
	//* 调试命令行
	QDEBUGNN << CQFlash CLYellow "=================== Project Debugging  中文 ================" CDefault;
	
	
	
	w.show();
	return a.exec();
}
