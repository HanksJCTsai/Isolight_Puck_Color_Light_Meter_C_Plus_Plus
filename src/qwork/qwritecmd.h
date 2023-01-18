#ifndef QWRITECMD_H
#define QWRITECMD_H

#define QWRITECMD_MESSAGE_OK 0
#define QWRITECMD_MESSAGE_OPEN_SUCCESS 1
#define QWRITECMD_MESSAGE_OPEN_FAILED 2
#define QWRITECMD_MESSAGE_CLOSE 3

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QTime>
#include <QEventLoop>

class QWriteCMD : public QObject
{
  Q_OBJECT
public:
  explicit QWriteCMD(QObject *parent = nullptr);
  ~QWriteCMD();


  bool m_is_pause = false;
  void QSleep(int msec);

signals:
  void threadToMainMessage(int messageType);
  void threadReceiveData(QByteArray dataArr);
  void countDownIntervalTime(int currentIntervalTime);
  void sendSerialPortCMD(QString resultCMD);

public slots:
  void openSerialPort(QString serialPortName, qint32 serialPortBaudrate, qint32 serialPortDataBits, QString serialPortParity, double serialPortStopBits);
  void closeSerialPort(void);
  void writeSerialPortCMD(const QByteArray data);
  void writeSerialPortCMDByAuto(const QByteArray data, int intervalTime);
  void receiveSerialPortData();

private:
  QSerialPort *serialPort;
};

#endif // QWRITECMD_H
