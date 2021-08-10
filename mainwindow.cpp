#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIODevice>
#include <QLocale>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QAudioFormat formatAudio;
  formatAudio.setCodec("audio/pcm");
  formatAudio.setSampleRate(44100);
  formatAudio.setChannelCount(1);
  formatAudio.setSampleSize(16);
  formatAudio.setByteOrder(QAudioFormat::LittleEndian);
  formatAudio.setSampleType(QAudioFormat::SignedInt);

  m_audioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(),
                                 formatAudio, this);

  m_deviceOriginal = new QBuffer(this);
  m_deviceOriginal->open(QIODevice::WriteOnly);

  ui->pb_saveOrigin->hide();
  ui->pb_saveTransformed->hide();

  loadPalindroms();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pb_record_toggled(bool checked) {
  if (checked) {
    m_deviceOriginal->deleteLater();
    delete m_deviceTransformed;
    m_deviceTransformed = nullptr;
    m_deviceOriginal = new QBuffer(this);
    m_deviceOriginal->open(QIODevice::WriteOnly);
    ui->pb_record->setText("STOP");
    m_audioInput->start(m_deviceOriginal);
    ui->pb_magic->setDisabled(true);
    ui->pb_playOriginal->setDisabled(true);
    ui->pb_playTransformed->setDisabled(true);
    ui->pb_saveOrigin->setDisabled(true);
    ui->pb_saveTransformed->setDisabled(true);
  } else {
    ui->pb_record->setText("RECORD");
    m_audioInput->stop();
    m_deviceOriginal->close();
    ui->pb_magic->setEnabled(true);
    ui->pb_playOriginal->setEnabled(true);
    ui->pb_saveOrigin->setEnabled(true);
  }
  setMagicButtonAnimation();
}

void MainWindow::on_pb_playTransformed_clicked() {
  QAudioFormat formatAudio;
  formatAudio.setCodec("audio/pcm");
  formatAudio.setSampleRate(44100);
  formatAudio.setChannelCount(1);
  formatAudio.setSampleSize(16);
  formatAudio.setByteOrder(QAudioFormat::LittleEndian);
  formatAudio.setSampleType(QAudioFormat::SignedInt);
  m_listenBuffer = new QBuffer(this);
  m_listenBuffer->open(QIODevice::ReadOnly);

  m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(),
                                   formatAudio, this);
  m_deviceTransformed->open(QIODevice::ReadOnly);
  m_audioOutput->start(m_deviceTransformed);
}

void MainWindow::listenBuffer(QBuffer *m_Buffer) {
  qDebug() << "ListenBuffer";
  m_deviceOriginal->open(QIODevice::ReadWrite);
  m_deviceOriginal->seek(0);
  QDataStream stream(m_deviceOriginal);

  QVector<qint16> reverse;
  qint16 temp;
  while (!stream.atEnd()) {
    stream >> temp;
    reverse.push_back(temp);
  }
  qDebug() << reverse.size();
  m_deviceOriginal->seek(0);
  m_deviceOriginal->close();

  m_deviceTransformed = new QBuffer(this);
  m_deviceTransformed->open(QIODevice::ReadWrite);
  m_deviceTransformed->seek(0);
  QDataStream streamOut(m_deviceTransformed);
  for (int i = reverse.size() - 1; i >= 0; i--) {
    streamOut << reverse[i];
  }
  m_deviceTransformed->seek(0);
  m_deviceTransformed->close();
  qDebug() << "SIZE=" << m_deviceOriginal->size();
  ui->pb_playTransformed->setEnabled(true);
  ui->pb_saveTransformed->setEnabled(true);
  setMagicButtonAnimation();
}

void MainWindow::on_pb_magic_clicked() { listenBuffer(m_deviceOriginal); }

void MainWindow::on_pb_playOriginal_clicked() {
  QAudioFormat formatAudio;
  formatAudio.setCodec("audio/pcm");
  formatAudio.setSampleRate(44100);
  formatAudio.setChannelCount(1);
  formatAudio.setSampleSize(16);
  formatAudio.setByteOrder(QAudioFormat::LittleEndian);
  formatAudio.setSampleType(QAudioFormat::SignedInt);
  m_listenBuffer = new QBuffer(this);
  m_listenBuffer->open(QIODevice::ReadOnly);

  m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(),
                                   formatAudio, this);
  m_deviceOriginal->open(QIODevice::ReadOnly);
  m_audioOutput->start(m_deviceOriginal);
}

void MainWindow::setMagicButtonAnimation() {
  if (ui->pb_magic->isEnabled() && m_deviceTransformed == nullptr) {
    qDebug() << "HERE";
    m_colorizeEffect = new QGraphicsDropShadowEffect(ui->widgetMagic);
    m_colorizeEffect->setBlurRadius(60);
    m_colorizeEffect->setOffset(0);
    m_colorizeEffect->setColor(QColor("#6371DB"));
    ui->widgetMagic->setGraphicsEffect(m_colorizeEffect);

    QPropertyAnimation *anim2 =
        new QPropertyAnimation(m_colorizeEffect, "color");
    anim2->setStartValue(QColor("#6371DB"));
    anim2->setKeyValueAt(0.17f, QColor("#374ADB"));
    anim2->setKeyValueAt(0.34f, QColor("#0015B7"));
    anim2->setKeyValueAt(0.5f, QColor("#000E77"));
    anim2->setKeyValueAt(0.67f, QColor("#0015B7"));
    anim2->setKeyValueAt(0.84f, QColor("#374ADB"));
    anim2->setEndValue(QColor("#6371DB"));
    anim2->setDuration(2000);
    anim2->setLoopCount(-1);
    anim2->start();
  } else {
    qDebug() << "ELSE";
    ui->widgetMagic->setGraphicsEffect(Q_NULLPTR);
    if (m_colorizeEffect) {
      m_colorizeEffect->setEnabled(false);
      m_colorizeEffect = Q_NULLPTR;
    }
  }
}

void MainWindow::on_pb_nextText_clicked() {
  if (m_currentPolyndrom == m_listPalindrome.size() - 1) {
    m_currentPolyndrom = 0;
  } else {
    m_currentPolyndrom++;
  }
  ui->label->setText(m_listPalindrome[m_currentPolyndrom]);
}

void MainWindow::on_pb_previousText_clicked() {
  if (m_currentPolyndrom == 0) {
    m_currentPolyndrom = m_listPalindrome.size() - 1;
  } else {
    m_currentPolyndrom--;
  }
  ui->label->setText(m_listPalindrome[m_currentPolyndrom]);
}

void MainWindow::on_comboBoxLocale_currentIndexChanged(int index) {
  if (index == 0) {
    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));
  } else if (index == 1) {
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  } else {
    return;
  }
  m_listPalindrome.clear();
  m_currentPolyndrom = 0;
  loadPalindroms();
}

void MainWindow::loadPalindroms() {
  QLocale locale;
  QFile filePalindrome(QString(":/docs/Palindrome_%1.txt").arg(locale.name()));
  if (filePalindrome.open(QIODevice::ReadOnly)) {
    while (!filePalindrome.atEnd()) {
      m_listPalindrome.push_back(filePalindrome.readLine());
    }
  }
  filePalindrome.close();
  ui->label->setText(m_listPalindrome[m_currentPolyndrom]);
}
