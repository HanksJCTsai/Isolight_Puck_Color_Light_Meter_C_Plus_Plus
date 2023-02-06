#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QPlainTextEdit>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

// About Serial Port
QSerialPort *serial;
QSerialPort *testConnect;
// About CheckBox from UI
QCheckBox *checkBoxOnTop;
// About ComBoBox from UI
QComboBox *comBoBoxSerialPortList;
QComboBox *comBoBoxBaudRateList;
QComboBox *comBoBoxDataBitList;
QComboBox *comBoBoxParityList;
QComboBox *comBoBoxStopBitList;
QComboBox *comBoBoxSingleCMDList;
QComboBox *comBoBoxIniSerialPortList;
QComboBox *comBoBoxIniBaudRateList;
QComboBox *comBoBoxIniDataBitList;
QComboBox *comBoBoxIniParityList;
QComboBox *comBoBoxIniStopBitList;
QComboBox *comBoBoxIniIntervalTimeList;
// About LCDNumberDisplay
QLCDNumber *LCDNumberIntervalTime;
// About Initi Data
QStringList baudList;
QStringList dataBitsList;
QStringList stopBitsList;
QStringList parityList;
QStringList singleCMDList;
QStringList intervalTimeList;
// Get application Dir Path
QString path;
// About Initi Data from json file
QString serialPortName = "";
QString baudRate;
QString dataBit;
QString stopBit;
QString parity = "";
int intervalTime;
// About QPlainTextEdit from UI
QPlainTextEdit *plainTextEditSendedCMD;
QPlainTextEdit *plainTextEditReceiveCMD;
QPlainTextEdit *plainTextEditThreadStatus;
QPlainTextEdit *plainTextEditConnectStatus;
QPlainTextEdit *plainTextEditIniSendCMD;
// About QLineEdit from UI
QLineEdit *textGRCCT;
QLineEdit *textGRL;
QLineEdit *textGRXYZ_X;
QLineEdit *textGRXYZ_Y;
QLineEdit *textGRXYZ_Z;
QLineEdit *textGRYXY_Y1;
QLineEdit *textGRYXY_Y2;
QLineEdit *textGRYXY_X;
// About QPushButton from UI
QPushButton *buttonTestConnect;
QPushButton *buttonPlay;
QPushButton *buttonPause;
QPushButton *buttonStop;
QPushButton *buttonSingleSendCMD;
QPushButton *buttonSave;
QPushButton *buttonCancel;
QPushButton *buttonClearLogs;
// tabWidgetMain
QTabWidget *tabWidgetMain;
// About system log path
QString configPath;
QString logPath;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  comBoBoxSerialPortList = ui->comBoBoxSerialPorts;
  comBoBoxBaudRateList = ui->comBoBoxBaudRates;
  comBoBoxDataBitList = ui->comBoBoxDataBits;
  comBoBoxParityList = ui->comBoBoxParities;
  comBoBoxStopBitList = ui->comBoBoxStopBits;
  comBoBoxSingleCMDList = ui->comBoBoxSingleCMDs;
  comBoBoxIniSerialPortList = ui->comBoBoxIniSerialPorts;
  comBoBoxIniBaudRateList = ui->comBoBoxIniBaudRates;
  comBoBoxIniDataBitList = ui->comBoBoxIniDataBits;
  comBoBoxIniParityList = ui->comBoBoxIniParities;
  comBoBoxIniStopBitList = ui->comBoBoxIniStopBits;
  comBoBoxIniIntervalTimeList = ui->comBoBoxIniIntervalTimes;
  LCDNumberIntervalTime = ui->lcdNumberInterval;
  plainTextEditSendedCMD = ui->plainTextEditSendedCMD;
  plainTextEditReceiveCMD = ui->plainTextEditReceiveCMD;
  plainTextEditThreadStatus = ui->plainTextEditThreadStatus;
  plainTextEditConnectStatus = ui->plainTextEditConnectStatus;
  plainTextEditIniSendCMD = ui->plainTextEditIniSendCMD;
  checkBoxOnTop = ui->checkBoxAlwaysOnTop;
  textGRCCT = ui->lineEditGRCCT;
  textGRL = ui->lineEditGRL;
  textGRXYZ_X = ui->lineEditGRXYZ_X;
  textGRXYZ_Y = ui->lineEditGRXYZ_Y;
  textGRXYZ_Z = ui->lineEditGRXYZ_Z;
  textGRYXY_Y1 = ui->lineEditGRYXY_Y1;
  textGRYXY_Y2 = ui->lineEditGRYXY_Y2;
  textGRYXY_X = ui->lineEditGRYXY_X;
  buttonTestConnect = ui->pushButtonTestConnect;
  buttonPlay = ui->pushButtonPlay;
  buttonPause = ui->pushButtonPause;
  buttonStop = ui->pushButtonStop;
  buttonSingleSendCMD = ui->pushButtonSendCMD;
  buttonSave = ui->pushButtonSave;
  buttonCancel = ui->pushButtonCancel;
  buttonClearLogs = ui->pushButtonClearLogs;
  serial = new QSerialPort(this);
  testConnect = new QSerialPort(this);
  tabWidgetMain = ui->tabWidgetMain;
  configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).append("/_serial_port_.json");
  logPath = "Log.log";
  this->addConfig(configPath);
  this->initiComponent();
}

MainWindow::~MainWindow()
{
  if(threadWrite->isRunning()){
      threadWrite->quit();
      threadWrite->wait();
    }
  delete ui;
}

void MainWindow::initiComponent(){
  tabWidgetMain->setCurrentIndex(0);
  checkBoxOnTop->setChecked(false);
  // Setting initi data
  //Interval Time for Ini page
  intervalTimeList <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20"
                   <<"21"<<"22"<<"23"<<"24"<<"25"<<"26"<<"27"<<"28"<<"29"<<"30"<<"31"<<"32"<<"33"<<"34"<<"35"<<"36"<<"37"<<"38"<<"39"<<"40"
                   <<"41"<<"42"<<"43"<<"44"<<"45"<<"46"<<"47"<<"48"<<"49"<<"50"<<"51"<<"52"<<"53"<<"54"<<"55"<<"56"<<"57"<<"58"<<"59"<<"60";
  // Setting baudrate
  baudList <<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
  // Setting data bits
  dataBitsList <<"5"<<"6"<<"7"<<"8";
  // Setting stop bits
  stopBitsList <<"1"<<"1.5"<<"2";
  // Setting parity
  parityList <<"None"<<"Even"<<"Odd"<<"Mark"<<"Space";
  // Assign  to comBoBox
  this->refreshConfig();
  // QSerial Port initial
  serialPortWrite = new QWriteCMD();
  threadWrite = new QThread();

  // When close MainWindow, and then kill thread and qwrite
  connect(threadWrite, SIGNAL(finished()), threadWrite, SLOT(deleteLater()));
  connect(threadWrite, SIGNAL(finished()), serialPortWrite, SLOT(deleteLater()));
  // Connect thread and Qwrite signal and slot: mainWindow to Thread
  connect(this, SIGNAL(mainToThreadOpenPort(QString,qint32,qint32,QString,double)), serialPortWrite, SLOT(openSerialPort(QString,qint32,qint32,QString,double)));
  connect(this, SIGNAL(mainToThreadClosePort()), serialPortWrite, SLOT(closeSerialPort()));
  connect(this, SIGNAL(mainToThreadAutoWrite(QByteArray,int)), serialPortWrite, SLOT(writeSerialPortCMDByAuto(QByteArray,int)));
  connect(this, SIGNAL(mainToThreadSingleWrite(QByteArray)), serialPortWrite, SLOT(writeSerialPortCMD(QByteArray)));
  connect(serialPortWrite, SIGNAL(threadToMainMessage(int)), this, SLOT(serialPortMessage(int)));
  // Connect thread and Qwrite signal and slot: Thread to mainWindow
  connect(serialPortWrite, SIGNAL(countDownIntervalTime(int)), LCDNumberIntervalTime, SLOT(display(int)));
  connect(serialPortWrite, SIGNAL(threadReceiveData(QByteArray)), this, SLOT(threadToMainReceiveData(QByteArray)));
  connect(serialPortWrite, SIGNAL(sendSerialPortCMD(QString)), this, SLOT(threadToMainSendCMDLog(QString)));
  // Connect button clicked's event
  connect(buttonTestConnect, SIGNAL(clicked()), this, SLOT(onTestConnectClicked()));
  connect(buttonPlay, SIGNAL(clicked()), this, SLOT(onPlayClicked()));
  connect(buttonPause, SIGNAL(clicked()), this, SLOT(onPauseClicked()));
  connect(buttonStop, SIGNAL(clicked()), this, SLOT(onStopClicked()));
  connect(buttonSingleSendCMD, SIGNAL(clicked()), this, SLOT(onSingleSendCMDClicked()));
  connect(buttonSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
  connect(checkBoxOnTop, SIGNAL(clicked(bool)), this, SLOT(onTopStatusChanged(bool)));
  connect(buttonClearLogs, SIGNAL(clicked()), this, SLOT(onClearLogsClicked()));
}

void MainWindow::onClearLogsClicked(){
  plainTextEditSendedCMD ->clear();
  plainTextEditReceiveCMD ->clear();
  plainTextEditConnectStatus->clear();
  plainTextEditThreadStatus->clear();
}

void MainWindow::onCancelClicked(){
  this->refreshConfig();
}

void MainWindow::refreshConfig(){
  // Get singal cmd list from json file
  singleCMDList = this->readSingleCMDListFromConfig(configPath);

  //Clear all combobox items
  comBoBoxSerialPortList->clear();
  comBoBoxIniSerialPortList->clear();

  comBoBoxBaudRateList->clear();
  comBoBoxIniBaudRateList->clear();

  comBoBoxDataBitList->clear();
  comBoBoxIniDataBitList->clear();

  comBoBoxStopBitList->clear();
  comBoBoxIniStopBitList->clear();

  comBoBoxParityList->clear();
  comBoBoxIniParityList->clear();

  comBoBoxSingleCMDList->clear();

  comBoBoxIniIntervalTimeList->clear();

  plainTextEditIniSendCMD->clear();
  // Get serial port info
  foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
      QSerialPort testSerial;
      testSerial.setPort(info);
      if(info.portName().mid(0,2).toUpper() !="CU" && testSerial.open(QIODevice::ReadWrite))
      {
          comBoBoxSerialPortList->addItem(info.portName());
          comBoBoxIniSerialPortList->addItem(info.portName());
          qInfo() << "Serial Port name:" <<info.portName();
      }
      testSerial.close();
    }
  // Assign  to comBoBox
  // Assign baud rate data to comBoBox
  if(serialPortName != "" && (comBoBoxSerialPortList->findText(serialPortName) > -1)){
      comBoBoxSerialPortList->setCurrentText(serialPortName);
      comBoBoxIniSerialPortList->setCurrentText(serialPortName);
    }

  comBoBoxBaudRateList->addItems(baudList);
  comBoBoxIniBaudRateList->addItems(baudList);
  if(baudRate != "" && comBoBoxBaudRateList->findText(baudRate) > -1){
      comBoBoxBaudRateList->setCurrentText(baudRate);
      comBoBoxIniBaudRateList->setCurrentText(baudRate);
    }

  comBoBoxDataBitList->addItems(dataBitsList);
  comBoBoxIniDataBitList->addItems(dataBitsList);
  if(dataBit != "" && comBoBoxDataBitList->findText(dataBit) > -1){
      comBoBoxDataBitList->setCurrentText(dataBit);
      comBoBoxIniDataBitList->setCurrentText(dataBit);
    }

  comBoBoxStopBitList->addItems(stopBitsList);
  comBoBoxIniStopBitList->addItems(stopBitsList);
  if(stopBit != "" && comBoBoxStopBitList->findText(stopBit) > -1){
      comBoBoxStopBitList->setCurrentText(stopBit);
      comBoBoxIniStopBitList->setCurrentText(stopBit);
    }

  comBoBoxParityList->addItems(parityList);
  comBoBoxIniParityList->addItems(parityList);
  if(parity != "" && comBoBoxParityList->findText(parity) > -1){
      comBoBoxParityList->setCurrentText(parity);
      comBoBoxIniParityList->setCurrentText(parity);
    }

  comBoBoxSingleCMDList->addItems(singleCMDList);

  comBoBoxIniIntervalTimeList->addItems(intervalTimeList);
  if(QString::number(intervalTime) != "" && comBoBoxIniIntervalTimeList->findText(QString::number(intervalTime)) > -1){
        comBoBoxIniIntervalTimeList->setCurrentText(QString::number(intervalTime));
      }
  // Assign defaut value to LCDNumber
  LCDNumberIntervalTime->display(intervalTime);
  // Ini single cmd
  foreach(QString cmd , singleCMDList){
      this->refreshPlainTextEdit(plainTextEditIniSendCMD, cmd);
    }
}

void MainWindow::onTopStatusChanged(bool checked){
 (checked)? (this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint)): (this->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint));
  this->show();
}

void MainWindow::addConfig(QString configPath){
    QDir *photo = new QDir;
    bool exist = photo->exists(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if(!exist)
    {
        photo->mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));//沒有文件夾就創建文件夾
    }
    exist = photo->exists(configPath);
    if(!exist) {
        this->writeConfigFromIniPage(configPath, "tty.usbserial-D30C22MM", "115200", "8", "None", "1", 2, "GRCCT,GRL,GRXYZ,GRYXY,");
      }
}

bool MainWindow::writeConfigFromIniPage(QString jsonPath, QString portpath, QString baudrate, QString databit, QString paritypath, QString stopbit, qint32 intervaltime, QString sendCMDList){
  bool result =false;
  QJsonObject rootObj;
  //{"Port Path": "tty.usbserial-D30C22MM", "Baud Rate": "115200", "Parity": "None", "Data Bits": "8", "Stop Bit": "1", "Time Out": 10, "Interval Time": 2, "Send CMD List": ["GRCCT","GRL","GRXYZ","GRYXY"], "Log File Name": "Log.log"}
  rootObj.insert("Port Path", portpath);
  rootObj.insert("Baud Rate", baudrate);
  rootObj.insert("Parity", paritypath);
  rootObj.insert("Data Bits", databit);
  rootObj.insert("Stop Bit", stopbit);
  rootObj.insert("Time Out", 0);
  rootObj.insert("Interval Time", intervaltime);
  QJsonArray intervalTimeArray;
  foreach(QString cmd, (sendCMDList).split(",")){
    if(cmd != "") {
        intervalTimeArray.append(cmd);
    }
  }
  rootObj.insert("Send CMD List", intervalTimeArray);
  rootObj.insert("Log File Name", "Log.log");
  QJsonDocument writeDoc;
  writeDoc.setObject(rootObj);

  QFile writeFile(jsonPath);
  if (!writeFile.open(QIODevice::ReadWrite)) {
      qDebug() << "can't open error!";
      return result;
    }
  writeFile.write(writeDoc.toJson());
  writeFile.close();
  // Refresh UI
  this->refreshConfig();
  result = true;
  return result;
}

QStringList MainWindow::readSingleCMDListFromConfig(QString jsonPath){
  QStringList jsonResult;
  QString value;
  QFile file;

  file.setFileName(jsonPath);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  value = file.readAll();
  file.close();

  QJsonDocument jsonDoc = QJsonDocument::fromJson(value.toUtf8());
  QJsonObject jsonObj = jsonDoc.object();

  QJsonValue jsonValue_serialportname = jsonObj.value(QString("Port Path"));
  QJsonValue jsonValue_baudrate = jsonObj.value(QString("Baud Rate"));
  QJsonValue jsonValue_databit = jsonObj.value(QString("Data Bits"));
  QJsonValue jsonValue_parity = jsonObj.value(QString("Parity"));
  QJsonValue jsonValue_stopbis = jsonObj.value(QString("Stop Bit"));
  QJsonValue jsonValue_singelCMD = jsonObj.value(QString("Send CMD List"));
  QJsonValue jsonValue_IntervalTime = jsonObj.value(QString("Interval Time"));
  QJsonArray jsonArray = jsonValue_singelCMD.toArray();

  serialPortName = jsonValue_serialportname.toString();
  baudRate = jsonValue_baudrate.toString();
  dataBit = jsonValue_databit.toString();
  parity = jsonValue_parity.toString();
  stopBit = jsonValue_stopbis.toString();
  intervalTime = jsonValue_IntervalTime.toInt();

  foreach (const QJsonValue &item, jsonArray) {
      jsonResult.append(item.toString());
    }
  return jsonResult;
}

void MainWindow::refreshPlainTextEdit(QPlainTextEdit *textEdit, QString msg){
  QTextCursor cursor = textEdit->textCursor();
  cursor.setPosition(0);
  textEdit->setTextCursor(cursor);
  textEdit->insertPlainText(msg + "\r\n");
}

void MainWindow::clearStatusLogPlainTextEdit(QPlainTextEdit *textEdit){
  textEdit->clear();
}

void MainWindow::onTestConnectClicked(){
  testConnect->setPortName(comBoBoxSerialPortList->currentText());
  switch (comBoBoxBaudRateList->currentText().toInt()){
     case 1200:
         testConnect->setBaudRate(QSerialPort::Baud1200, QSerialPort::AllDirections);
         break;
     case 2400:
         testConnect->setBaudRate(QSerialPort::Baud2400, QSerialPort::AllDirections);
         break;
     case 4800:
         testConnect->setBaudRate(QSerialPort::Baud4800, QSerialPort::AllDirections);
         break;
     case 9600:
         testConnect->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
         break;
     case 19200:
         testConnect->setBaudRate(QSerialPort::Baud19200, QSerialPort::AllDirections);
         break;
     case 38400:
         testConnect->setBaudRate(QSerialPort::Baud38400, QSerialPort::AllDirections);
         break;
     case 57600:
         testConnect->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
         break;
     case 115200:
         testConnect->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
         break;
  }
  switch (comBoBoxDataBitList->currentText().toInt()) {
     case 8:
         testConnect->setDataBits(QSerialPort::Data8);
         break;
     case 7:
         testConnect->setDataBits(QSerialPort::Data7);
         break;
     case 6:
         testConnect->setDataBits(QSerialPort::Data6);
         break;
     case 5:
         testConnect->setDataBits(QSerialPort::Data5);
         break;
  }
  if(comBoBoxParityList->currentText() =="None") {
      testConnect->setParity(QSerialPort::NoParity);
    } else if(comBoBoxParityList->currentText() =="Odd"){
      testConnect->setParity(QSerialPort::OddParity);
    } else if(comBoBoxParityList->currentText() =="Even"){
      testConnect->setParity(QSerialPort::EvenParity);
    } else if(comBoBoxParityList->currentText() =="Mark"){
      testConnect->setParity(QSerialPort::MarkParity);
    } else if(comBoBoxParityList->currentText() =="Space"){
      testConnect->setParity(QSerialPort::SpaceParity);
    }
  if(comBoBoxStopBitList->currentText().toDouble() == 1){
      testConnect->setStopBits(QSerialPort::OneStop);
    } else if(comBoBoxStopBitList->currentText().toDouble() == 1.5) {
      testConnect->setStopBits(QSerialPort::OneAndHalfStop);
    } else {
      testConnect->setStopBits(QSerialPort::TwoStop);
    }
  testConnect->setFlowControl(QSerialPort::NoFlowControl);
  testConnect->setReadBufferSize(500);
  if (testConnect->open(QSerialPort::ReadWrite)){
    this->refreshPlainTextEdit(plainTextEditConnectStatus, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": " + "Test connect port:" + comBoBoxSerialPortList->currentText() + " is success!");
   } else {
    this->refreshPlainTextEdit(plainTextEditConnectStatus, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": " + "Test connect port:"+ comBoBoxSerialPortList->currentText() + " is fail!");
   }
  testConnect->close();
}

void MainWindow::onSaveClicked(){
 bool saveResult = false;
 QString ini_serialportpath = comBoBoxIniSerialPortList->currentText();
 QString ini_baudrate = comBoBoxIniBaudRateList->currentText();
 QString ini_databits = comBoBoxIniDataBitList->currentText();
 QString ini_stopbit = comBoBoxIniStopBitList->currentText();
 QString ini_parity = comBoBoxIniParityList->currentText();
 qint32 ini_intervaltime = comBoBoxIniIntervalTimeList->currentText().toInt();
 QString ini_sendcmd = (plainTextEditIniSendCMD->toPlainText()).replace("\n",",");

 saveResult = this->writeConfigFromIniPage(configPath, ini_serialportpath, ini_baudrate, ini_databits, ini_parity, ini_stopbit, ini_intervaltime, ini_sendcmd);

 if(saveResult){
     QMessageBox msgBox;
     msgBox.setText("Save Success!");
     msgBox.exec();
     tabWidgetMain->setCurrentIndex(0);
   }
}

void MainWindow::onPlayClicked(){
  try {
    if(threadWrite == NULL){
        threadWrite = new QThread();
      }
    serialPortWrite->m_is_pause = false;
    serialPortWrite->moveToThread(threadWrite);
    threadWrite->start();
    emit mainToThreadOpenPort(comBoBoxSerialPortList->currentText(), comBoBoxBaudRateList->currentText().toInt(), comBoBoxDataBitList->currentText().toInt(), comBoBoxParityList->currentText(), comBoBoxStopBitList->currentText().toDouble());

    //QString autoCMD = "GRCCT\rGRL\rGRXYZ\rGRYXY\r";
    QString autoCMD = singleCMDList.join('\r');
    autoCMD = autoCMD.append('\r');
    QByteArray byteArrayAutoCMD = autoCMD.toLatin1();
    emit mainToThreadAutoWrite(byteArrayAutoCMD.data(), intervalTime);

    buttonTestConnect->setEnabled(false);
    buttonPlay->setEnabled(false);
    buttonPause->setEnabled(true);
    buttonStop->setEnabled(false);
    this->refreshPlainTextEdit(plainTextEditThreadStatus, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": Do to play." );
  }
  catch (std::exception &e) {
    qDebug() << QString(QChar::fromLatin1(*e.what()));
   }
}

void MainWindow::onPauseClicked(){
  serialPortWrite->m_is_pause = true;
  buttonTestConnect->setEnabled(false);
  buttonPlay->setEnabled(true);
  buttonPause->setEnabled(false);
  buttonStop->setEnabled(true);
  this->refreshPlainTextEdit(plainTextEditThreadStatus, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": Do to pause." );
}

void MainWindow::onStopClicked(){
  threadWrite->quit();
  threadWrite->wait();
  buttonTestConnect->setEnabled(true);
  buttonPlay->setEnabled(true);
  buttonPause->setEnabled(false);
  buttonStop->setEnabled(false);
  this->refreshPlainTextEdit(plainTextEditThreadStatus, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": Do to stop." );
}


void MainWindow::onSingleSendCMDClicked(){
  if(threadWrite->isRunning()){
      QString CMD = comBoBoxSingleCMDList->currentText() + "\r";
      QByteArray byteArrayCMD = CMD.toLatin1();
      emit mainToThreadSingleWrite(byteArrayCMD.data());
    }
}

void MainWindow::serialPortMessage(int messageID){
  switch(messageID){
      case QWRITECMD_MESSAGE_OK:
            ui->statusbar->showMessage("Serial port is ready!", 30000);
           break;
      case QWRITECMD_MESSAGE_OPEN_SUCCESS:
            ui->statusbar->showMessage("Serial port is open!", 30000);
           break;
      case QWRITECMD_MESSAGE_OPEN_FAILED:
            ui->statusbar->showMessage("Serial port is fail!", 30000);
           break;
      case QWRITECMD_MESSAGE_CLOSE:
            ui->statusbar->showMessage("Serial port is close!", 30000);
           break;
    }
}

void MainWindow::threadToMainReceiveData(QByteArray receiveData){
  if (receiveData.length() > 0){ //&& (receiveData.contains("GRCCT") && receiveData.contains("\r\n>"))){
      QString showData =  QString(receiveData).replace(">","").replace("\u001BEGRCCT",",").replace("\u001BEGRL",",").replace("\u001BEGRXYZ 0",",").replace("\u001BEGRYXY 0",",");
      QStringList showDatas = showData.split("\r\n");
      QStringList responseList;
      foreach (QString data, showDatas) {
          if(data.length() >3){
              responseList = data.split(",");
              if(responseList.length() > 0){
                  if(responseList[0].toUpper() == "GRCCT"){
                      textGRCCT->setText(QString(responseList[1]));
                    } else if(responseList[0].toUpper() == "GRL"){
                      textGRL->setText(QString(responseList[1]));
                    } else if(responseList[0].toUpper() == "GRXYZ"){
                      if(responseList.length() > 3){
                          textGRXYZ_X->setText(QString(responseList[1]));
                          textGRXYZ_Y->setText(QString(responseList[2]));
                          textGRXYZ_Z->setText(QString(responseList[3]));
                      }
                    } else if(responseList[0].toUpper() == "GRYXY"){
                      if(responseList.length() > 3){
                          textGRYXY_Y1->setText(QString(responseList[1]));
                          textGRYXY_X->setText(QString(responseList[2]));
                          textGRYXY_Y2->setText(QString(responseList[3]));
                        }
                    }
                }
            }
        }
      this->refreshPlainTextEdit(plainTextEditReceiveCMD, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": " + showData.replace("\r\n",""));
      //this->refreshPlainTextEdit(plainTextEditReceiveCMD, showData.replace("\r\n",""));
      serialPortWrite->m_is_pause = false;
    }
}

void MainWindow::threadToMainSendCMDLog(QString sendedCMD){
  if (sendedCMD.length() > 0){
      this->refreshPlainTextEdit(plainTextEditSendedCMD, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ": " + sendedCMD.replace("\r",""));
    }
}

void MainWindow::closeEvent (QCloseEvent *event){
  QMessageBox::StandardButton buttonResult = QMessageBox::question(this, "Meter Tools", tr("Are you sure?\n"),  QMessageBox::Cancel |QMessageBox::No |QMessageBox::Yes, QMessageBox::Yes);
  if (buttonResult != QMessageBox::Yes){
      event->ignore();
    } else {
      event->accept();
    }
}
