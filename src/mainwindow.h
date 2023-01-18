#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAINWINDOW_BUTTON_TESTCONNECT 0
#define MAINWINDOW_BUTTON_PLAY 1
#define MAINWINDOW_BUTTON_PAUSE 2
#define MAINWINDOW_BUTTON_STOP 3
#define MAINWINDOW_BUTTON_SINGLESENDCMD 4

#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
#include <QString>
#include <QPlainTextEdit>
#include "qwork/qwritecmd.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

  private:
    Ui::MainWindow *ui;
    QWriteCMD *serialPortWrite;
    QThread *threadWrite;
    QThread *threadReceive;
    void initiComponent();
    bool writeConfigFromIniPage(QString jsonPath, QString portPath, QString baudRate, QString dataBits, QString parityPath, QString stopBit, qint32 intervalTime, QString sendCMDList);
    QStringList readSingleCMDListFromConfig(QString jsonPath);
    void refreshPlainTextEdit(QPlainTextEdit *textEdit, QString msg);
    void clearStatusLogPlainTextEdit(QPlainTextEdit *textEdit);
    void addConfig(QString configPahth);
    void refreshConfig();
    void onClearLogsClicked();

  signals:
    void mainToThreadOpenPort(QString serialPortName, qint32 serialPortBaudrate, qint32 serialPortDataBits, QString serialPortParity, double serialPortStopBits);
    void mainToThreadClosePort();
    qint64 mainToThreadSingleWrite(const QByteArray data);
    void mainToThreadAutoWrite(const QByteArray data, int intervalTime);
  private slots:
    void threadToMainReceiveData(QByteArray receiveData);
    void threadToMainSendCMDLog(QString sendCMD);
    void serialPortMessage(int messageID);
    void onTestConnectClicked();
    void onCancelClicked();
    void onSaveClicked();
    void onPlayClicked();
    void onPauseClicked();
    void onStopClicked();
    void onSingleSendCMDClicked();
    void onTopStatusChanged(bool checked);
};
#endif // MAINWINDOW_H
