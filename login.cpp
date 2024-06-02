#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

Login::~Login()
{
    delete ui;
}


void Login::on_regestration_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();
}
