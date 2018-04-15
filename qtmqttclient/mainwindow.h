#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMqttClient.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_bt_conn_clicked();
    void updateLogStateChange();
    void brokerDisconnected();

    void on_bt_sub_clicked();

    void on_bt_pub_clicked();

private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
};

#endif // MAINWINDOW_H
