#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaRecorder>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QUrl>
#include <QFileDialog>
#include <QStandardPaths>
#include <QString>
#include <QTimer>
#include <QAudioInput>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QBuffer>
#include <QAudioOutput>
#include <QFile>
#include "ListenSound.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QAudioRecorder *m_audioRecorder = nullptr;
    int number;
    bool alreadyInRun;
    uint countTime;
    QAudioInput* m_audioInput;
    QThread* m_ThreadTimer;
    QTimer* timer;
    QString path;
    QUrl m_nextUrl;
    QBuffer* m_buffer;
    QAudioOutput* m_audioOutput;
    ListenSound m_soundListen;
    QBuffer* buffer;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void handleStateChanged(QAudio::State);
    void listenBuffer(QBuffer*);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
