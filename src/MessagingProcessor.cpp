#include "MessagingProcessor.h"

#include <QImage>
#include <QVideoFrameFormat>
#include <QBuffer>

MessagingProcessor::MessagingProcessor(QObject *parent)
    : QObject{parent}
{

}

void MessagingProcessor::processData(const QByteArray &data)
{
  // Формируем изображение
  QImage img;
  img.loadFromData(data);

  // Формируем кадр
  auto f = QVideoFrameFormat::pixelFormatFromImageFormat(img.format());
  QVideoFrameFormat format(img.size(), f);
  QVideoFrame frame(format);

  // Отправляем кадр в интерфейс
  frame.map(QVideoFrame::ReadWrite);
  memcpy(frame.bits(0), img.bits(), img.sizeInBytes());
  frame.unmap();
  emit receivedFrame(frame);
}

QByteArray MessagingProcessor::packFrame(const QVideoFrame &frame)
{
  QImage img = frame.toImage();
  QBuffer buffer;
  img.save(&buffer, "JPEG");
  return buffer.data();
}
