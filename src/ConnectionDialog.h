#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>

class ConnectionDialog : public QDialog
{
  Q_OBJECT
public:
  explicit ConnectionDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~ConnectionDialog();

  QString getRemoteAddress() const { return m_remoteAddress->text(); }
  QString getRemotePort() const { return m_remotePort->text(); }
  QString getLocalPort() const { return m_localPort->text(); }

private:
  QFormLayout *m_formLayout;
  QLineEdit *m_localPort;
  QLineEdit *m_remotePort;
  QLineEdit *m_remoteAddress;
};

#endif // CONNECTIONDIALOG_H
