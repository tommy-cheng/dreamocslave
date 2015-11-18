#include <QtNetwork>
#include <QString>
#include <QMessageBox>
#include "dreamocslave.h"
#include "ui_dreamocslave.h"

QString GetCurrentTimeStr()
{
    QDateTime dateTime = dateTime.currentDateTime();
    QString dateTimeString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString strRet;

    strRet = "[" + dateTimeString + "]:";
    return strRet;
}

FusionCmd GetFusionCmd(QByteArray byaCommand)
{
    QString sCommand = byaCommand;
    FusionCmd eRetCmd;

    if (sCommand.compare("Joint Request", Qt::CaseInsensitive) == 0) {
        eRetCmd = CMD_JOINT_REQ;
    }
    else if (sCommand.compare("Play Start", Qt::CaseInsensitive) == 0) {
        eRetCmd = CMD_PLAY_START;
    }
    else if (sCommand.compare("Staus Check", Qt::CaseInsensitive) == 0) {
         eRetCmd = CMD_CHK_STATUS;
    }
    else {
        eRetCmd = CMD_UNKNOWN;
    }
    return eRetCmd;
}

void dreamocslave::InsertNewLine(QString sBuf, bool bLocal, QString sIP, quint16 Port)
{
    QString strBuf;

    strBuf = GetCurrentTimeStr() + "\n";
    if (bLocal) {
        ui->textStatus->setTextColor(QColor("red"));
        strBuf = strBuf + "\t [Local IP]:" + sIP + "\n";
        strBuf = strBuf + "\t [Local Port]:" + QString::number(Port) + "\n";
    }
    else {
        ui->textStatus->setTextColor(tcSlave);
        strBuf = strBuf + "\t [Remote IP]:" + sIP + "\n";
        strBuf = strBuf + "\t [Remote Port]:" + QString::number(Port) + "\n";
    }
    strBuf = strBuf + "\t [Command]:" + sBuf;
    ui->textStatus->append(strBuf);
}

dreamocslave::dreamocslave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dreamocslave)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::LocalHost, 45454);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    ui->textStatus->append(GetCurrentTimeStr());
    ui->textStatus->append("Slave is ready...\n");
    tcSlave = ui->textStatus->textColor();
}

dreamocslave::~dreamocslave()
{
    delete ui;
}

void dreamocslave::readyRead()
{
    QByteArray buffer, datagram;
    QString strBuf;

    buffer.resize(udpSocket->pendingDatagramSize());

    QHostAddress senderIP;
    quint16 senderPort;
    FusionCmd eCmd;

    udpSocket->readDatagram(buffer.data(), buffer.size(), &senderIP, &senderPort);
    InsertNewLine(buffer, false, senderIP.toString(), senderPort);

    eCmd = GetFusionCmd(buffer);
    switch (eCmd) {
    case CMD_JOINT_REQ:
        datagram = "OK";
        udpSocket->writeDatagram(datagram.data(), datagram.size(),senderIP, senderPort);
        InsertNewLine(datagram, true, udpSocket->localAddress().toString(), udpSocket->localPort());
        break;

    case CMD_PLAY_START:
    case CMD_CHK_STATUS:
    default:
        break;
    }
}

void dreamocslave::on_pbQuit_clicked()
{
    close();
}


