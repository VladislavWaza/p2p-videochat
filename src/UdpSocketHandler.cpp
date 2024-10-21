#include "UdpSocketHandler.h"

#include <QNetworkDatagram>

#include <stdexcept>

UdpSocketHandler::UdpSocketHandler(QHostAddress remoteAddress, quint16 remotePort, quint16 localPort, QObject *parent)
  : QObject{parent}, m_remoteAddress(remoteAddress), m_remotePort(remotePort), m_localPort(localPort)
{
  if (!m_socket.bind(m_localPort))
  {
    QString msg = "The socket could not be bound to the port " + QString::number(m_localPort) +
        ": " + m_socket.errorString();
    throw std::runtime_error(msg.toStdString());
  }

  m_localPort = m_socket.localPort();
  connect(&m_socket, &QUdpSocket::readyRead, this, &UdpSocketHandler::onSocketReadyRead);
}

UdpSocketHandler::~UdpSocketHandler()
{
  m_socket.close();
}

qint64 UdpSocketHandler::sendData(const QByteArray &data)
{
  return m_socket.writeDatagram(data, m_remoteAddress, m_remotePort);
}

void UdpSocketHandler::onSocketReadyRead()
{
  while (m_socket.hasPendingDatagrams())
  {
    QNetworkDatagram datagram = m_socket.receiveDatagram();
    emit receivedData(datagram.data());
  }
}
