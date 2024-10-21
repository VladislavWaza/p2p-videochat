#ifndef UDPSOCKETHANDLER_H
#define UDPSOCKETHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UdpSocketHandler : public QObject
{
  Q_OBJECT
public:
  explicit UdpSocketHandler(QHostAddress remoteAddress,
                            quint16 remotePort,
                            quint16 localPort,
                            QObject *parent = nullptr);
  ~UdpSocketHandler();

  quint16 getLocalPort() const { return m_localPort; }
  qint64 sendData(const QByteArray &data);

signals:
  void receivedData(const QByteArray&);

private slots:
  void onSocketReadyRead();

private:
  QUdpSocket m_socket;
  QHostAddress m_remoteAddress;
  quint16 m_remotePort;
  quint16 m_localPort;
};

#endif // UDPSOCKETHANDLER_H
