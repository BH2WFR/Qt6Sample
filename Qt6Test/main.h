#ifndef MAIN_H__
#define MAIN_H__

#ifdef __cplusplus

#include <qglobal.h>
#include <QObject>
#include <QApplication>
#include <TerminalBeauty.h>

#include <QDebug>


class A : public QObject{
    Q_OBJECT
public:
    QString shabi {"shabi"};

signals:
    void MySignal();
    
public slots:
    virtual void MySlot(){
        qDebug() << "A";
    }
public:
    A(){
        //QObject::connect(pushButtonCntUp, SIGNAL(clicked()), MainWindow, SLOT(btnCntUp_Click()));
        QObject::connect(this, SIGNAL(MySignal()), this, SLOT(MySlot()));
    }
};

struct B1 : public A{
    Q_OBJECT
public:
    B1() : A() {
        shabi = "B1";
    }
public slots:
	virtual void MySlot() override{
		qDebug() << "B1";
	}
};

struct B2 : public A{
    Q_OBJECT
public:
    B2() : A() {
        shabi = "B2";
    }
public slots:
	virtual void MySlot() override{
		qDebug() << "B2";
	}
};

#endif // __cplusplus

#endif //MAIN_H__
