#ifndef SOCKETPARAMETERS_H
#define SOCKETPARAMETERS_H

#include <QWidget>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QThread>

namespace Ui {
class socketParameters;
}

class socketParameters : public QWidget
{
    Q_OBJECT

public:
    explicit socketParameters(QWidget *parent = 0);
    ~socketParameters();

private slots:
    void on_pushButton_clicked();
    void keyPressEvent(QKeyEvent *event);
signals:
    void socketData(QTcpSocket*, QString);
private:
    Ui::socketParameters *ui;
    QTcpSocket *socket = new QTcpSocket();
};

#endif // SOCKETPARAMETERS_H
