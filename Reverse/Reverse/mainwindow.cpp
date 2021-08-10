#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QAudioFormat formatAudio;
    formatAudio.setCodec("audio/pcm");
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(16);
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);
    m_soundListen.setFileName("Count 10.wav");
    m_soundListen.open(QIODevice::ReadOnly);
    if(!m_soundListen.isOpen()){
        qDebug() << "Файл нельзя открыть";
    }
//    m_audioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(),formatAudio,this);
//    m_buffer=new QBuffer(this);
//    m_buffer->open(QIODevice::WriteOnly);
//    m_audioInput->start(m_buffer);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultInputDevice(),formatAudio,this);
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    connect(&m_soundListen,SIGNAL(sendBuffer(QBuffer*)),this,SLOT(listenBuffer(QBuffer*)));
    m_audioOutput->start(&m_soundListen);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleStateChanged(QAudio::State newState)
{
    qDebug() << "handleStateChanged" << newState;
    switch (newState) {
              case QAudio::IdleState:
                  // Finished playing (no more data)
                  m_audioOutput->stop();
                  m_soundListen.bufferClose();
                  m_soundListen.close();
                  delete m_audioOutput;
                  break;

              case QAudio::StoppedState:
                  // Stopped for other reasons
                  if (m_audioOutput->error() != QAudio::NoError) {
                      // Error handling
                  }
                  break;

              default:
                  // ... other cases as appropriate
                  break;
    }
}

void MainWindow::listenBuffer(QBuffer* m_Buffer)
{
    qDebug() << "ListenBuffer";
    m_Buffer->open(QIODevice::ReadOnly);
    m_Buffer->seek(0);
    qDebug()<<m_Buffer->size();
    QByteArray source;
    QBuffer x_Buff;
    QDataStream in(m_Buffer);
    in>>source;
    qDebug() << source.size();
    QVector<qint16> reverse;
    in>>reverse;
    qDebug()<<reverse.size();
    m_Buffer->close();
    QBuffer x_buffer;
    x_buffer.open(QIODevice::WriteOnly);
    QDataStream out(&x_buffer);
    for(int i=reverse.size()-1; i>=0; i--){
        out<<reverse[i];
    }
    x_buffer.close();
    qDebug() << "SIZE=" << x_buffer.size();

}
