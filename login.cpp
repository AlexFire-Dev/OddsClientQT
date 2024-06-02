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
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
           this, [](QNetworkReply *reply) {
               QString answer = reply->readAll();
               qDebug() << answer;
           }
       );
}

Login::~Login()
{
    delete ui;
}


void Login::on_regestration_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    //тут нужно кидать на сервак запрос на чек пароля
    request.setUrl(QUrl("http://af.shvarev.com/oauth/token/login/"));
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // Создаем запрос
    QUrl url("http://af.shvarev.com/oauth/token/login/");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Подготавливаем данные
    QJsonObject data;
    data.insert("email", login);
    data.insert("password", password);
    QByteArray jsonData = QJsonDocument(data).toJson();

    // Отправляем запрос
    QNetworkReply *reply = manager->post(request, jsonData);

    // Обработка ответа
    QByteArray main_data;
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray main_data = reply->readAll();
            qDebug() << "Ответ сервера: " << main_data;
            this->hide();
            odds win;
            QString tk = "";
            int kol = 0;
            for(int i = 0; i< main_data.length(); i++){
                if(main_data[i] == '"'){
                    kol++;
                    continue;
                }
                if(kol==3)
                    tk+=main_data[i];
            }
            win.token ="Token " + tk;
            win.ready();
            qDebug() << win.token << endl;
            win.setWindowTitle("Odds_Table");
            win.exec();
        } else {
            qDebug() << "Ошибка: " << reply->errorString();
            QMessageBox::warning(this, "Authorization", "Login or password incorrect!");
        }
        reply->deleteLater();
    });

//    if(main_data != "Error transferring http://af.shvarev.com/oauth/token/login/ - server replied: Bad Request"){
//        hide();
//        odds win;
//        win.exec();
//    }
//    else{
//        QMessageBox::warning(this, "Авторизация", "Пароль или логин не коректен");
//    }
}
