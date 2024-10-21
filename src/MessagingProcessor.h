#ifndef MESSAGINGPROCESSOR_H
#define MESSAGINGPROCESSOR_H

#include <QObject>
#include <QVideoFrame>

class MessagingProcessor : public QObject
{
  Q_OBJECT
public:
  explicit MessagingProcessor(QObject *parent = nullptr);

  void processData(const QByteArray& data);

  QByteArray packFrame(const QVideoFrame& frame);

signals:
  void receivedFrame(const QVideoFrame&);

};

#endif // MESSAGINGPROCESSOR_H
