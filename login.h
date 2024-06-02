#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtGui>
#include <QtCore>
#include <QObject>
#include <odds.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_regestration_clicked();

private:
    Ui::Login *ui;
    odds *win;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};
#endif // LOGIN_H
