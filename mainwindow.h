#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QDir>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>
#include <QStringList>

#include "socketparameters.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();
    void manageSocketData();

public slots:
    void pushButton_send();
    void setTyping();
    void getSocket(QTcpSocket*, QString);

private:
    Ui::MainWindow *ui;
    socketParameters *parametersDialog = new socketParameters();
    QString name = "";
    QTcpSocket *socket = new QTcpSocket();
    QStringList clientsName;
    bool istyping = false;
    bool isfinish = false;
};

#endif // MAINWINDOW_H
