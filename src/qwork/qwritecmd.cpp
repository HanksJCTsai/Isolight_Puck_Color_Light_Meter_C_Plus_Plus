#include "qwritecmd.h"

QWriteCMD::QWriteCMD(QObject *parent) :QObject(parent)
{
  qDebug() << __FUNCTION__ << "Initial QWriteCMD Class Thread ID:" << QThread::currentThreadId();
  serialPort = new QSerialPort(this);
  connect(serialPort, SIGNAL(readyRead()), this, SLOT(receiveSerialPortData()));
}

QWriteCMD::~QWriteCMD(){
  qDebug() << __FUNCTION__ << "Initial QWriteCMD Class Thread ID:" << QThread::currentThreadId();
}

void QWriteCMD:: openSerialPort(QString serialPortName, qint32 serialPortBaudrate, qint32 serialPortDataBits, QString serialPortParity, double serialPortStopBits){
    qDebug() << __FUNCTION__ << "Open Serial Port Thread ID:" << QThread::currentThreadId();
    serialPort->setPortName(serialPortName);
    switch (serialPortBaudrate){
       case 1200:
           serialPort->setBaudRate(QSerialPort::Baud1200, QSerialPort::AllDirections);
           break;
       case 2400:
           serialPort->setBaudRate(QSerialPort::Baud2400, QSerialPort::AllDirections);
           break;
       case 4800:
           serialPort->setBaudRate(QSerialPort::Baud4800, QSerialPort::AllDirections);
           break;
       case 9600:
           serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
           break;
       case 19200:
           serialPort->setBaudRate(QSerialPort::Baud19200, QSerialPort::AllDirections);
           break;
       case 38400:
           serialPort->setBaudRate(QSerialPort::Baud38400, QSerialPort::AllDirections);
           break;
       case 57600:
           serialPort->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
           break;
       case 115200:
           serialPort->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
           break;
    }
    switch (serialPortDataBits) {
       case 8:
           serialPort->setDataBits(QSerialPort::Data8);
           break;
       case 7:
           serialPort->setDataBits(QSerialPort::Data7);
           break;
       case 6:
           serialPort->setDataBits(QSerialPort::Data6);
           break;
       case 5:
           serialPort->setDataBits(QSerialPort::Data5);
           break;
    }
    if(serialPortParity=="None") {
        serialPort->setParity(QSerialPort::NoParity);
      } else if(serialPortParity =="Odd"){
        serialPort->setParity(QSerialPort::OddParity);
      } else if(serialPortParity =="Even"){
        serialPort->setParity(QSerialPort::EvenParity);
      } else if(serialPortParity=="Mark"){
        serialPort->setParity(QSerialPort::MarkParity);
      } else if(serialPortParity =="Space"){
        serialPort->setParity(QSerialPort::SpaceParity);
      }
    if(serialPortStopBits == 1){
        serialPort->setStopBits(QSerialPort::OneStop);
      } else if(serialPortStopBits == 1.5) {
        serialPort->setStopBits(QSerialPort::OneAndHalfStop);
      } else {
        serialPort->setStopBits(QSerialPort::TwoStop);
      }
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setReadBufferSize(1000);

    if(serialPort->open(QSerialPort::ReadWrite)){
        emit threadToMainMessage(QWRITECMD_MESSAGE_OPEN_SUCCESS);
      } else {
        emit threadToMainMessage(QWRITECMD_MESSAGE_OPEN_FAILED);
      }
}

void QWriteCMD:: closeSerialPort(void){
    qDebug() << __FUNCTION__ << "Close Serial Port Thread ID:" << QThread::currentThreadId();
    if(serialPort->isOpen()){
        serialPort->clear();
        serialPort->close();
      }
      emit threadToMainMessage(QWRITECMD_MESSAGE_CLOSE);
}

void QWriteCMD::receiveSerialPortData(){
    QByteArray receiveDataList;
    if(serialPort->bytesAvailable() > 0){
        receiveDataList = serialPort->readAll();
        while(serialPort->waitForReadyRead(1000)){
            receiveDataList.append(serialPort->readAll());
          }
      }
    qDebug() << receiveDataList;
    emit threadReceiveData(receiveDataList);
}

void QWriteCMD::writeSerialPortCMD(const QByteArray data){
    qDebug() << __FUNCTION__ << "Write Serial Port Thread ID:" << QThread::currentThreadId();
    serialPort->clear();
    serialPort->write(data, data.length());
    serialPort->flush();
    serialPort->waitForBytesWritten(10);
    emit sendSerialPortCMD(QString(data));
}


void QWriteCMD::writeSerialPortCMDByAuto(const QByteArray data, int intervalTime){
    qDebug() << __FUNCTION__ << "Auto Write Serial Port Thread ID:" << QThread::currentThreadId();
    int time = intervalTime;

    while(not m_is_pause){
        emit countDownIntervalTime(time);
        if(time == 0){
            if(serialPort!=NULL && serialPort->isOpen()){
                serialPort->clear();
                serialPort->write(data, data.length());
                serialPort->flush();
                serialPort->waitForBytesWritten(10);
              }
            time = intervalTime;
            emit sendSerialPortCMD(QString(data));
            m_is_pause = true;
          }
        time--;
        QSleep(1 * 1000);
      }
}

void QWriteCMD::QSleep(int msec){
    QDateTime last = QDateTime::currentDateTime();
    QDateTime now;
    while (1){
        now = QDateTime::currentDateTime();
        if (last.msecsTo(now) >= msec){
              break;
          }
      }
}
