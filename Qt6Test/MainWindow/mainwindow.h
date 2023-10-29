#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    
    
    int m_number {0};
    void setCntNumber(int number, bool isFlush=true);
    void flushCntNumber();
    
protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    //virtual void keyReleaseEvent(QKeyEvent *ev);
    
    
private slots:
    void btnCntUp_Click();
    void btnCntDn_Click();
    void btnCntRst_Click();
    
};
#endif // MAINWINDOW_H
