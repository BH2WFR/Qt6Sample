#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "qdebug.h"
#include <qtwidgets/qtextedit.h>

#include "TerminalBeauty.h"

#include <qevent.h>
#include <fmt/core.h>
#include <fmt/format.h>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	
	this->setFixedSize(this->width(), this->height());
	this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
	
	//ui->lineEditCnt->setEnabled(false);
	ui->lineEditCnt->setReadOnly(true);
	ui->lineEditCnt->setFocusPolicy(Qt::FocusPolicy::NoFocus); //让它无法获取焦点
	
	// 按钮无法获取焦点时, 仍然可以正常用鼠标点击, 只是不能用键盘导航
	ui->pushButtonCntUp-> setFocusPolicy((Qt::FocusPolicy::NoFocus)); 
	ui->pushButtonCntDn-> setFocusPolicy((Qt::FocusPolicy::NoFocus));
	ui->pushButtonCntRst->setFocusPolicy((Qt::FocusPolicy::NoFocus));
	
	flushCntNumber();
}

MainWindow::~MainWindow()
{
	//delete textEdit;
	delete ui;
}

void MainWindow::setCntNumber(int number, bool isFlush)
{
	m_number = number;
	if(isFlush){
		flushCntNumber();
	}
}

void MainWindow::flushCntNumber()
{
	ui->lineEditCnt->setText(QString::number(m_number));
	ui->lineEditCnt->setText(QString::fromStdString(fmt::format("{}", m_number)));
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
	
	
	switch (int key = ev->key(); key){ //(ev->key() & 0xFFFF)
		case Qt::Key::Key_Up:
		case Qt::Key_U:
		case Qt::Key_Space:
		case Qt::Key_Return:
		//case 0x1000004:
			btnCntUp_Click();
			
			break;
		case Qt::Key::Key_Down:
		case Qt::Key::Key_D:
		case Qt::Key::Key_Backspace:
			btnCntDn_Click();
			
			break;
		case Qt::Key::Key_R:
		case Qt::Key::Key_Delete:
			btnCntRst_Click();
			
			break;
		default:
			fmt::println("keyPressEvent, key=0x{:x}", key);
	}
	
}

// void MainWindow::keyReleaseEvent(QKeyEvent *ev)
// {
// 	QDEBUGN << "keyReleaseEvent";
	
// }

void MainWindow::btnCntDn_Click()
{
	
	m_number--;
	flushCntNumber();
	QDEBUGN << "cntDn, num=" << m_number;
}

void MainWindow::btnCntRst_Click()
{
	
	m_number = 0;
	flushCntNumber();
	ui->lineEditCnt->text();
	QDEBUGN << "rst, num=" << m_number;
}

void MainWindow::btnCntUp_Click()
{
	
	m_number++;
	flushCntNumber();
	QDEBUGN << "cntUp, num=" << m_number;
}
