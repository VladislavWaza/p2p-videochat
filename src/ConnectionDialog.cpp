#include "ConnectionDialog.h"

#include <QPushButton>
#include <QVBoxLayout>

ConnectionDialog::ConnectionDialog(QWidget *parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
  setWindowTitle("Подключение");

  QVBoxLayout *layout = new QVBoxLayout(this);
  m_formLayout = new QFormLayout(this);

  m_localPort = new QLineEdit("0", this);
  m_remotePort = new QLineEdit("0", this);
  m_remoteAddress = new QLineEdit("127.0.0.1", this);

  m_formLayout->addRow("Удаленный адрес:", m_remoteAddress);
  m_formLayout->addRow("Удаленный порт:", m_remotePort);
  m_formLayout->addRow("Локальный порт:", m_localPort);

  QPushButton *button = new QPushButton("Подключиться", this);
  layout->addLayout(m_formLayout);
  layout->addWidget(button);

  connect(button, &QPushButton::clicked, this, &ConnectionDialog::accept);
}

ConnectionDialog::~ConnectionDialog()
{
}
