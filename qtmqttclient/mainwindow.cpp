#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client = new QMqttClient(this);
    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(" Received Topic: ")
                    + topic.name()
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        ui->text_log->insertPlainText(content);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bt_conn_clicked()
{
    m_client->setHostname(ui->edit_addr->text());
    m_client->setPort(ui->edit_port->text().toInt());
    m_client->setUsername(ui->edit_user->text());
    m_client->setPassword(ui->edit_pwd->text());

    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->connectToHost();
        ui->bt_conn->setText("Disconnect");
    } else {
        m_client->disconnectFromHost();
        ui->bt_conn->setText("Connect");
    }

}

void MainWindow::on_bt_sub_clicked()
{
    auto subscription = m_client->subscribe(ui->edit_sub->text());
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

}

void MainWindow::on_bt_pub_clicked()
{
    if (m_client->publish(ui->edit_sub->text(), ui->edit_pub->text().toUtf8()) == -1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));

}

void MainWindow::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    ui->text_log->insertPlainText(content);
}

void MainWindow::brokerDisconnected()
{
    ui->bt_conn->setText(tr("Connect"));
}

