#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "UdpSocketHandler.h"
#include "MessagingProcessor.h"

#include <QMainWindow>
#include <QVideoWidget>
#include <QPushButton>
#include <QLabel>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QTimer>

#include <memory>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void toggleConnection();
  void toggleMicrophone();
  void toggleCamera();

  void onReceivedData(const QByteArray &data);
  void onReceivedFrame(const QVideoFrame &frame);

  void sendFrame(const QVideoFrame &frame);

private:
  void setupUi();
  void setupMetricHandler();
  bool getConnectionParams(QHostAddress& remoteAddress, quint16& remotePort, quint16& localPort);

  QVideoWidget *m_localVideoWidget = nullptr;
  QVideoWidget *m_remoteVideoWidget = nullptr;
  QPushButton *m_connectionButton = nullptr;
  QPushButton *m_microphoneButton = nullptr;
  QPushButton *m_cameraButton = nullptr;

  QLabel *m_pingLabel = nullptr;
  QLabel *m_fpsLabel = nullptr;
  QLabel *m_packetLossLabel = nullptr;
  QLabel *m_bitrateLabel = nullptr;
  QLabel *m_localPortLabel = nullptr;

  QTimer m_timer;
  int m_frameCount = 0;
  int m_dataSizeCount = 0;

  QMediaCaptureSession *m_localCaptureSession = nullptr;
  QMediaCaptureSession *m_remoteCaptureSession = nullptr;
  QCamera *m_camera = nullptr;

  bool m_isConnected = false;
  std::unique_ptr<UdpSocketHandler> m_socketHandler;

  MessagingProcessor m_msgProc;
};

#endif // MAINWINDOW_H
