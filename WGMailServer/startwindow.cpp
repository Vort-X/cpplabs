#include "startwindow.h"
#include "ui_startwindow.h"
#include "emailserver.h"
#include <QMessageBox>
StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWindow)
    , m_emailServer(new EmailServer(this))
{
    ui->setupUi(this);
    connect(ui->startStopButton, &QPushButton::clicked, this, &StartWindow::toggleStartServer);
    connect(m_emailServer, &EmailServer::logMessage, this, &StartWindow::logMessage);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::toggleStartServer()
{
    if (m_emailServer->isListening()) {
        m_emailServer->stopServer();
        ui->startStopButton->setText(tr("Start Server"));
        logMessage(QStringLiteral("Server Stopped"));
    } else {
        if (!m_emailServer->startServer(QHostAddress::Any, 1967)) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to start the server"));
            return;
        }
        logMessage(QStringLiteral("Server Started"));
        ui->startStopButton->setText(tr("Stop Server"));
    }
}

void StartWindow::logMessage(const QString &msg)
{
    ui->logEditor->appendPlainText(msg + QLatin1Char('\n'));
}
