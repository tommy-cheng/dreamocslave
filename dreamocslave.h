#ifndef DREAMOCSLAVE_H
#define DREAMOCSLAVE_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE

namespace Ui {
class dreamocslave;
}

class dreamocslave : public QDialog
{
    Q_OBJECT

public:
    explicit dreamocslave(QWidget *parent = 0);
    ~dreamocslave();

private slots:
    void on_pbQuit_clicked();
    void readyRead();
    void InsertNewLine(QString, bool, QString, quint16);

private:
    Ui::dreamocslave *ui;
    QUdpSocket *udpSocket;
    QColor tcSlave;
};

enum FusionCmd {
    CMD_UNKNOWN,
    CMD_JOINT_REQ,
    CMD_PLAY_START,
    CMD_CHK_STATUS
};

#endif // DREAMOCSLAVE_H
