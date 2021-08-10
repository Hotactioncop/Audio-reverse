#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QGraphicsDropShadowEffect>
#include <QMainWindow>

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
} // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
  QAudioInput *m_audioInput = nullptr;
  QBuffer *m_deviceOriginal = nullptr;
  QBuffer *m_deviceTransformed = nullptr;

  QAudioOutput *m_audioOutput = nullptr;
  QBuffer *m_listenBuffer = nullptr;
  QGraphicsDropShadowEffect *m_colorizeEffect = nullptr;
  QStringList m_listPalindrome;
  uint m_currentPolyndrom = 0;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_pb_record_toggled(bool checked);

  void on_pb_playTransformed_clicked();
  void listenBuffer(QBuffer *m_Buffer);

  void on_pb_magic_clicked();

  void on_pb_playOriginal_clicked();
  void setMagicButtonAnimation();

  void on_pb_nextText_clicked();

  void on_pb_previousText_clicked();

  void on_comboBoxLocale_currentIndexChanged(int index);

private:
  Ui::MainWindow *ui;
  void loadPalindroms();
};
#endif // MAINWINDOW_H
