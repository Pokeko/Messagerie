#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtAlgorithms>

#define IS_TYPING 0x1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Remove unecessary default widget
    ui->statusBar->hide();
    ui->mainToolBar->hide();

    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit_2->setFocusPolicy(Qt::NoFocus);
    // Update all style sheet
    ui->centralWidget->setStyleSheet("background-color: black;");
    ui->textToSend->setStyleSheet("background-color: white");
    ui->send->setStyleSheet("background-color: white");
    ui->typing->setStyleSheet("color: white;");
    ui->textEdit->setStyleSheet("color: #00FF41;");
    ui->textEdit_2->setStyleSheet("color: #00FF41;");

    connect(this->parametersDialog, &socketParameters::socketData, this, &MainWindow::getSocket);
    connect(ui->textToSend, &QLineEdit::textChanged, this, &MainWindow::setTyping);
    connect(ui->textToSend, &QLineEdit::returnPressed, this, &MainWindow::pushButton_send);
    connect(ui->textToSend, &QLineEdit::editingFinished, [this](){
        if (!isfinish && !this->name.isEmpty())
        {
            socket->write("{{<<FINISH_TYPING>>}}\n");
            socket->waitForBytesWritten();
            isfinish = true;
            istyping = false;
        }
    });
    connect(this->socket, &QTcpSocket::readyRead, this, &MainWindow::manageSocketData);
    QTimer::singleShot(100, this, &MainWindow::init);

}

void MainWindow::getSocket(QTcpSocket *sock, QString pseudo)
{
    socket = sock;
    this->name = pseudo;
//    parametersDialog->close();
//    parametersDialog->deleteLater();
//    parametersDialog = nullptr;
    parametersDialog->hide();
    this->show();
    qDebug() << "Socket Initialized";
}

void MainWindow::init()
{
//    parametersDialog->show();
    socket->connectToHost("localhost", 7777);
    socket->waitForConnected();
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        bool ok;
           QString text = QInputDialog::getText(this, tr("Entrer votre pseudo"),
                                                tr("Pseudo"), QLineEdit::Normal,
                                                QDir::home().dirName(), &ok);
           if (ok && !text.isEmpty())
               this->name = text;
           else
           {
               qsrand(0);
               int number = qrand() % 50 + 0;
               this->name = "Visitor" + QString::number(number) + "\n";
           }
        socket->write(QString("{{<<NAME>>}}\n" + this->name + "\n").toStdString().c_str());
        socket->waitForBytesWritten();
    }
//    else
//        QApplication::exit(0);
    //this->name = pseudo;
//    parametersDialog->close();
//    parametersDialog->deleteLater();
//    parametersDialog = nullptr;
    this->show();
    qDebug() << "Socket Initialized";
}

void MainWindow::manageSocketData()
{
    while (socket->canReadLine())
    {
        QByteArray data = socket->readLine();
        QString dataToSend = data.toStdString().c_str();
         dataToSend.remove(QChar('\n'), Qt::CaseInsensitive);

        if (dataToSend == "{{<<IS_TYPING>>}}")
                ui->typing->setText("Someone writing..");
        else if (dataToSend == "{{<<FINISH_TYPING>>}}")
                ui->typing->clear();
        else if (dataToSend == "{{<<NAME>>}}")
        {
            QString name_ = socket->readLine();
            name_.remove(QChar('\n'), Qt::CaseInsensitive);
            ui->textEdit->append(name_.remove(QChar('\n'), Qt::CaseInsensitive) + " is connected !");
            clientsName.push_back(name_);
            ui->textEdit_2->clear();
            for (auto client: clientsName)
                ui->textEdit_2->append(client);
        }
        else if (dataToSend == "{{<<ONLINE>>}}")
        {
            qDebug() << "DEBUG";
            QString clientName = socket->readLine();
            qDebug() << clientName;
            clientsName.push_back(clientName);
            ui->textEdit_2->clear();
            clientsName.sort();
            for (auto client: clientsName)
            {   qDebug() << client;
                client.remove('\n');
                    ui->textEdit_2->append(client);
            }
        }
        else if (dataToSend == "{{<<DISCONNECT>>}}")
        {
            QString clientName = socket->readLine();
            ui->textEdit->append(clientName.remove('\n') + " is disconnected.");
            clientsName.removeOne(clientName);
            ui->textEdit_2->clear();
            for (auto client: clientsName)
            {
                client.remove('\n');
                ui->textEdit_2->append(client);
            }
        }
        else
         ui->textEdit->append(dataToSend);
     }
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}

void MainWindow::setTyping()
{
    if (ui->textToSend->text().isEmpty())
    {
        if (!isfinish)
        {
            socket->write("{{<<FINISH_TYPING>>}}\n");
            socket->waitForBytesWritten();
            istyping = false;
            isfinish = true;
        }
    }
    else
    {
        if (!istyping)
        {
            socket->write("{{<<IS_TYPING>>}}\n");
            socket->waitForBytesWritten();
            isfinish = false;
            istyping = true;
        }
    }
}

void MainWindow::pushButton_send()
{
    if (!ui->textToSend->text().isEmpty())
    {
        QString messageToSend = ui->textToSend->text();
        this->socket->write((this->name + ": " + messageToSend + "\n").toStdString().c_str());
        this->socket->waitForBytesWritten();
        ui->textToSend->clear();
    }
}
