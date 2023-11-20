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
    
    int b1pri1 {666};
    int b1pri2 {777};
    
    Q_PROPERTY(QMap<QString,QVariant> propertyGroup READ propertyGroup CONSTANT)
    
    Q_PROPERTY(int b1pri1 READ getB1pri1 WRITE setB1pri1 MEMBER b1pri1);
    Q_PROPERTY(int b1pri2 READ getB1pri2 WRITE setB1pri2 MEMBER b1pri2);

    
    int getB1pri1() const {return b1pri1;}
    int getB1pri2() const {return b1pri2;}
    
    void setB1pri1(int num) {b1pri1 = num;}
    void setB1pri2(int num) {b1pri2 = num;}
    
    QMap<QString, QVariant> propertyGroup();
    
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


class MyClass{
public:
    int num{666};
    
    MyClass(int num) : num{num}{}
    ~MyClass() {}
    
};

#endif // __cplusplus

#endif //MAIN_H__
