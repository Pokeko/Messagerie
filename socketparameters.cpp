#include "socketparameters.h"
#include "ui_socketparameters.h"

socketParameters::socketParameters(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::socketParameters)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(400, 400));
    this->setStyleSheet("background-color: black;");

    ui->host->setStyleSheet("background-color: #eee3e0;");
    ui->port->setStyleSheet("background-color: #eee3e0;");
    ui->pseudo->setStyleSheet("background-color: #eee3e0;");
    ui->log->setStyleSheet("background-color: #eee3e0;");
    ui->pushButton->setStyleSheet("background-color: #eee3e0;");
    ui->log->setFocusPolicy(Qt::NoFocus);
}

socketParameters::~socketParameters()
{
    delete ui;
}

void socketParameters::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
        ui->pushButton->click();
}

void socketParameters::on_pushButton_clicked()
{
    ui->log->clear();
    if (!ui->host->text().isEmpty() && !ui->port->text().isEmpty() && !ui->pseudo->text().isEmpty())
    {
        ui->log->append("Host: " + ui->host->text());
        ui->log->append("Port: " + ui->port->text());
        ui->log->append("Pseudo: " + ui->pseudo->text());
        ui->log->append("Try to connect to the host..");
        socket->connectToHost(ui->host->text(), ui->port->text().toInt());
        socket->waitForConnected();
        if (socket->state() != QAbstractSocket::ConnectedState)
        {
            ui->log->append("Connection failed. Check your host/port and retry again.");
        }
        else
        {
            ui->log->append("Successfully connected.");
            QThread::msleep(1000);
            emit socketData(socket, ui->pseudo->text());
        }
    }
    else
    {
        ui->log->append("You need to specify an host and a port !");
    }
}
