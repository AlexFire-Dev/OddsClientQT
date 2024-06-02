#ifndef ODDS_H
#define ODDS_H

#include <ui_odds.h>
#include <iostream>
#include <QDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QtNetwork>
#include <QtDebug>
#include <QJsonDocument>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

struct odd_table{
    QString time;
    QString leagueName;
    QString home;
    QString away;
    QString date;
    QString bookmaker;
    double homeOd;
    double awayOd;
    QString odAddTime;
    int stamp;
};



namespace Ui {
class odds;
}

class odds : public QDialog
{
    Q_OBJECT

public:
    std::vector <std::pair<int,std::pair <int,QString>>> sorted;
    std::map <std::pair <int, QString> , std::vector <odd_table>> all;
    std::set<QString> Book;
    std::set<QString> League;
    void ready(){}


    void filter() {}


    QString token;
    explicit odds(QWidget *parent = nullptr);
    ~odds();

private slots:
    void on_pushButton_clicked();

    void on_Apply_Button_clicked();

    void on_Graph_clicked();

    void on_Update_button_clicked();

private:
    Ui::odds *ui;
};

#endif // ODDS_H
