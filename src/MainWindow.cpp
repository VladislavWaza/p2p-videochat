#include "MainWindow.h"
#include "ConnectionDialog.h"

#include <QDockWidget>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QVideoSink>
#include <QVideoFrame>
#include <QBuffer>
#include <QStatusBar>
#include <QMessageBox>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  setupUi();
  setupMetricHandler();

  connect(&m_msgProc, &MessagingProcessor::receivedFrame, this, &MainWindow::onReceivedFrame);
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleConnection()
{
  if (!m_isConnected)
  {
    QHostAddress remoteAddress;
    quint16 remotePort = 0;
    quint16 localPort = 0;

    if (!getConnectionParams(remoteAddress, remotePort, localPort))
    {
      return;
    }

    try
    {
      m_socketHandler = std::make_unique<UdpSocketHandler>(remoteAddress, remotePort, localPort);
    }
    catch (const std::exception& e)
    {
      QMessageBox::critical(this, "Ошибка!", e.what());
      return;
    }

    connect(m_socketHandler.get(), &UdpSocketHandler::receivedData, this, &MainWindow::onReceivedData);

    m_isConnected = true;
    m_connectionButton->setText("Отключиться");
    m_localPortLabel->setText("Локальный порт: " + QString::number(m_socketHandler->getLocalPort()));
  }
  else
  {
    m_socketHandler.reset();
    m_isConnected = false;
    m_connectionButton->setText("Подключиться");
    m_localPortLabel->setText("Локальный порт: -----");
  }
}

void MainWindow::toggleMicrophone()
{

}

void MainWindow::toggleCamera()
{
}

void MainWindow::onReceivedData(const QByteArray &data)
{
  m_dataSizeCount += data.size();
  m_msgProc.processData(data);
}

void MainWindow::onReceivedFrame(const QVideoFrame &frame)
{
  ++m_frameCount;
  m_remoteCaptureSession->videoSink()->setVideoFrame(frame);
}

void MainWindow::sendFrame(const QVideoFrame &frame)
{
  if (m_isConnected)
  {
    qDebug() << m_socketHandler->sendData(m_msgProc.packFrame(frame));
  }
}

void MainWindow::setupUi()
{
  setWindowTitle("p2p-videochat");
  resize(400, 300);

  // Виджеты отображения видео
  m_localVideoWidget = new QVideoWidget(this);
  m_remoteVideoWidget = new QVideoWidget(this);

  // Выдвижные виджеты
  QDockWidget *localDock = new QDockWidget("Local Video", this);
  localDock->setWidget(m_localVideoWidget);
  localDock->setMinimumSize(100, 100);

  QDockWidget *remoteDock = new QDockWidget("Remote Video", this);
  remoteDock->setWidget(m_remoteVideoWidget);
  remoteDock->setMinimumSize(100, 100);

  // Расположение выдивжных виджетов
  addDockWidget(Qt::TopDockWidgetArea, localDock);
  addDockWidget(Qt::TopDockWidgetArea, remoteDock);

  // Главный виджет
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QGridLayout *mainLayout = new QGridLayout(centralWidget);

  // Кнопки
  m_connectionButton = new QPushButton("Подключиться", this);
  m_microphoneButton = new QPushButton("Выключить микрофон", this);
  m_cameraButton = new QPushButton("Выключить камеру", this);

  mainLayout->addWidget(m_connectionButton, 1, 0);
  mainLayout->addWidget(m_microphoneButton, 1, 1);
  mainLayout->addWidget(m_cameraButton, 1, 2);

  connect(m_connectionButton, &QPushButton::clicked, this, &MainWindow::toggleConnection);
  connect(m_microphoneButton, &QPushButton::clicked, this, &MainWindow::toggleMicrophone);
  connect(m_cameraButton, &QPushButton::clicked, this, &MainWindow::toggleCamera);

  // Метрики
  m_pingLabel = new QLabel("Пинг: -- ms", this);
  m_fpsLabel = new QLabel("FPS: -- ", this);
  m_packetLossLabel = new QLabel("Потеря пакетов: -- %", this);
  m_bitrateLabel = new QLabel("Скорость: -- Kbps", this);
  m_localPortLabel = new QLabel("Локальный порт: -----", this);
  m_localPortLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

  statusBar()->addWidget(m_pingLabel);
  statusBar()->addWidget(m_fpsLabel);
  statusBar()->addWidget(m_packetLossLabel);
  statusBar()->addWidget(m_bitrateLabel);
  statusBar()->addWidget(m_localPortLabel);

  // Подключения видеовещания
  m_localCaptureSession = new QMediaCaptureSession(this);
  m_localCaptureSession->setVideoOutput(m_localVideoWidget);

  m_remoteCaptureSession = new QMediaCaptureSession(this);
  m_remoteCaptureSession->setVideoOutput(m_remoteVideoWidget);

  // Подключение камеры
  m_camera = new QCamera(this);
  m_localCaptureSession->setCamera(m_camera);
  m_camera->start();

  // Зеркалим кадр
  connect(m_localCaptureSession->videoSink(), &QVideoSink::videoFrameChanged, [this](QVideoFrame frame)
  {
    // QVideoFrame явно используется совместно,
    // поэтому любые изменения, внесенные в видеокадр,
    // также будут применяться к любым копиям
    frame.setMirrored(true);
  });

  // Отправляем кадр
  connect(m_localCaptureSession->videoSink(), &QVideoSink::videoFrameChanged, this, &MainWindow::sendFrame);
}

void MainWindow::setupMetricHandler()
{
  // Обработчик метрик
  connect(&m_timer, &QTimer::timeout, [this]()
  {
    m_fpsLabel->setText(QString("FPS: %1").arg(m_frameCount));
    m_frameCount = 0;

    m_bitrateLabel->setText(QString("Скорость: %1 Kbps").arg(m_dataSizeCount/1024));
    m_dataSizeCount = 0;
  });

  m_timer.start(1000);
}

bool MainWindow::getConnectionParams(QHostAddress &remoteAddress, quint16 &remotePort, quint16 &localPort)
{
  ConnectionDialog dialog(this);
  if (dialog.exec() != QDialog::Accepted)
  {
    return false;
  }

  remoteAddress = QHostAddress(dialog.getRemoteAddress());

  bool ok;
  remotePort = dialog.getRemotePort().toUShort(&ok);
  if (!ok)
  {
    QMessageBox::critical(this, "Ошибка!", "Получен некорректный удаленный порт: " + dialog.getRemotePort());
    return false;
  }

  localPort = dialog.getLocalPort().toUShort(&ok);
  if (!ok)
  {
    QMessageBox::critical(this, "Ошибка!", "Получен некорректный локальный порт: " + dialog.getLocalPort());
    return false;
  }

  return true;
}
