#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QPushButton>
#include <QLabel>
#include <QUdpSocket>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QHostAddress>
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
  void onSocketReadyRead();
  void sendFrame(const QVideoFrame &frame);

private:
  void setupUi();
  void setupMetricHandler();

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

  std::unique_ptr<QUdpSocket> m_socket;
  bool m_isConnected = false;
  quint16 m_localPort;
  quint16 m_remotePort;
  QHostAddress m_remoteAddress;
};

#endif // MAINWINDOW_H
