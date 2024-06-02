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
    void ready(){
        ui->tableWidget->clear();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->scrollToItem(ui->tableWidget->item(25,0),QAbstractItemView::PositionAtTop);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList({"ID","Bookmaker", "League", "time", "Home", "Away", "Home_odd", "Away_odd","Change %"}));
        QNetworkRequest request(QUrl("http://af.shvarev.com/api/v1/odds/today/"));
        request.setRawHeader("Authorization", QString(token).toLocal8Bit());
        //qDebug() <<token<<"OOOOOOOOtvet"<<endl;
       // qDebug() << QString(token).toLocal8Bit()<< "  OTVET" << endl;
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        QNetworkReply *reply = manager->get(request);


        connect(reply, &QNetworkReply::finished, [reply,this]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray main_data = reply->readAll();
                //qDebug() << "Ответ сервера: " << main_data;
                QString string = QString::fromStdString(main_data.constData());
                QJsonDocument jsonDocument = QJsonDocument::fromJson(string.toUtf8());
                if (jsonDocument.isNull()) {
                        qDebug() << "Неверный формат JSON-строки";
                 }
                //QJsonObject jsonObject = jsonDocument.object();
                QJsonArray jsonArray = jsonDocument.array();

                    // Перебор элементов массива
                    for (const QJsonValue &value : jsonArray) {
                        if (value.isObject()) {
                            QJsonObject jsonObject = value.toObject();
                            odd_table game;
                            // Доступ к данным в объекте
                            int id = jsonObject.value("id").toInt();
                            int gameId = jsonObject.value("game_id").toInt();
                            QString time = jsonObject.value("time").toString();
                            QString leagueName = jsonObject.value("league_name").toString();
                            int leagueId = jsonObject.value("league_id").toInt();
                            QString home = jsonObject.value("home").toString();
                            QString away = jsonObject.value("away").toString();
                            QString date = jsonObject.value("date").toString();
                            QString bookmaker = jsonObject.value("bookmaker").toString();
                            double homeOd = jsonObject.value("home_od").toDouble();
                            double awayOd = jsonObject.value("away_od").toDouble();
                            QString odAddTime = jsonObject.value("od_add_time").toString();
                            QString stamp = jsonObject.value("stamp").toString();
                            QString stamp_to_add = "";
                            int kol = 0;
                            for(int i = 0;i<stamp.length();i++){
                                if(stamp[i]=='_'){
                                    kol++;
                                    continue;
                                }
                                if(stamp[i]=='.')
                                    break;
                                if(kol==2)
                                    stamp_to_add +=stamp[i];


                            }
                            QJsonValue result = jsonObject.value("result");
                            game.stamp = std::stoi(stamp_to_add.toStdString());
                            game.time = time;
                            game.leagueName = leagueName;
                            game.home = home;
                            game.away = away;
                            game.date = date;
                            game.bookmaker = bookmaker;
                            game.homeOd = homeOd;
                            game.awayOd = awayOd;
                            game.odAddTime = odAddTime;
                            //qDebug() << "Stamp: " << game.stamp<<endl;
                            all[ {gameId,bookmaker} ].push_back(game);

                           //  Вывод данных
//                            qDebug() << "ID: " << id;
//                            qDebug() << "Game ID: " << gameId;
//                            qDebug() << "Time: " << time;
//                            qDebug() << "League Name: " << leagueName;
//                            qDebug() << "League ID: " << leagueId;
//                            qDebug() << "Home: " << home;
//                            qDebug() << "Away: " << away;
//                            qDebug() << "Date: " << date;
//                            qDebug() << "Bookmaker: " << bookmaker;
//                            qDebug() << "Home OD: " << homeOd;
//                            qDebug() << "Away OD: " << awayOd;
//                            qDebug() << "OD Add Time: " << odAddTime;
//                            qDebug() << "Stamp: " << stamp;
//                            qDebug() << "Result: " << result.toString();


                        }
                    }

//                    int game_cnt = 0;
//                    for(auto u : all){
//                        qDebug() << "ID"<< u.first.first<< "   Bookmaker:"<<u.first.second<<endl;
//                        qDebug() << "Игра номер: "<<++game_cnt<<endl;
//                        int kol_up = 0;
//                        for (auto g : u.second){
//                            qDebug() << "Номер обновления: "<<++kol_up<<endl;
//                            qDebug() << g.time <<endl;
//                            qDebug() << g.leagueName <<endl;
//                            qDebug() << g.home <<endl;
//                            qDebug() << g.away <<endl;
//                            qDebug() << g.date <<endl;
//                            qDebug() << g.bookmaker <<endl;
//                            qDebug() << g.homeOd <<endl;
//                            qDebug() << g.awayOd <<endl;
//                            qDebug() << g.odAddTime <<endl;
//                        }
//                    }
               sorted.clear();
               for(auto u : all){

                     for (auto g : u.second){
                         double last = 0;
                         for(auto st : u.second){
                             last = st.homeOd;
                         }
                         sorted.push_back({abs(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0),{u.first.first,u.first.second}});
                         break;
                     }
               }
               sort(sorted.begin(),sorted.end());
               reverse(sorted.begin(),sorted.end());
                int kol_up = 0;





                for(auto u : sorted){

                    for (auto g : all[{u.second.first, u.second.second}]){
                            ui->tableWidget->insertRow( ui->tableWidget->rowCount());
                            ui->tableWidget->setItem(kol_up, 0, new QTableWidgetItem(QString::number(u.second.first)));
                            ui->tableWidget->setItem(kol_up, 1, new QTableWidgetItem(g.bookmaker));
                            Book.insert(g.bookmaker);
                            ui->tableWidget->setItem(kol_up, 2, new QTableWidgetItem(g.leagueName));
                            League.insert(g.leagueName);
                            ui->tableWidget->setItem(kol_up, 3, new QTableWidgetItem(g.time));
                            ui->tableWidget->setItem(kol_up, 4, new QTableWidgetItem(g.home));
                            ui->tableWidget->setItem(kol_up, 5, new QTableWidgetItem(g.away));
                            ui->tableWidget->setItem(kol_up, 6, new QTableWidgetItem(QString::number(g.homeOd)));
                            ui->tableWidget->setItem(kol_up, 7, new QTableWidgetItem(QString::number(g.awayOd)));
                            double last = 0;
                            for(auto st : all[{u.second.first, u.second.second}]){
                                last = st.homeOd;
                            }

                            ui->tableWidget->setItem(kol_up, 8, new QTableWidgetItem(QString::number(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0)));
                            //ui->tableWidget->item(kol_up, 8)->setData(Qt::DisplayRole, QVariant(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0));


                            ++kol_up;
                            break;
                         }
                   }












//                    for(auto u : all){

//                           for (auto g : u.second){
//                                ui->tableWidget->insertRow( ui->tableWidget->rowCount());
//                                ui->tableWidget->setItem(kol_up, 0, new QTableWidgetItem(QString::number(u.first.first)));
//                                ui->tableWidget->setItem(kol_up, 1, new QTableWidgetItem(g.bookmaker));
//                                Book.insert(g.bookmaker);
//                                ui->tableWidget->setItem(kol_up, 2, new QTableWidgetItem(g.leagueName));
//                                League.insert(g.leagueName);
//                                ui->tableWidget->setItem(kol_up, 3, new QTableWidgetItem(g.time));
//                                ui->tableWidget->setItem(kol_up, 4, new QTableWidgetItem(g.home));
//                                ui->tableWidget->setItem(kol_up, 5, new QTableWidgetItem(g.away));
//                                ui->tableWidget->setItem(kol_up, 6, new QTableWidgetItem(QString::number(g.homeOd)));
//                                ui->tableWidget->setItem(kol_up, 7, new QTableWidgetItem(QString::number(g.awayOd)));
//                                double last = 0;
//                                for(auto st : u.second){
//                                    last = st.homeOd;
//                                }

//                                ui->tableWidget->setItem(kol_up, 8, new QTableWidgetItem(QString::number(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0)));
//                                //ui->tableWidget->item(kol_up, 8)->setData(Qt::DisplayRole, QVariant(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0));


//                                ++kol_up;
//                                break;
//                             }
//                       }
                    //ui->tableWidget->sortItems(8, Qt::DescendingOrder);
                    ui->Time_box->addItem("ALL");
                    ui->Time_box->addItem("1 hour");
                    ui->Time_box->addItem("3 hours");
                    ui->Time_box->addItem("8 hours");
                    ui->Time_box->addItem("12 hours");
                    ui->Bookmaker_box->addItem("ALL");
                    for(auto u : Book)
                        ui->Bookmaker_box->addItem(u);
                    ui->League_box->addItem("ALL");
                    for(auto u : League)
                        ui->League_box->addItem(u);

                //ui->tableWidget->sortItems(8, Qt::DescendingOrder);
            } else {
                qDebug() << "Ошибка: " << reply->errorString();
            }
            reply->deleteLater();
        });
    }


    void filter(){
        ui->tableWidget->clear();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList({"ID","Bookmaker", "League", "time", "Home", "Away", "Home_odd", "Away_odd","Change %"}));


        sorted.clear();
        for(auto u : all){

              for (auto g : u.second){
                  QDateTime currentDateTime1 = QDateTime::currentDateTime();
                  double currentDateTime = currentDateTime1.toSecsSinceEpoch();
                  for(auto st : u.second){
                          currentDateTime = st.stamp;
                          break;
                  }



                  double last = 0;
                  if(ui->Time_box->currentText() == QString("ALL")){
                       for(auto st : u.second){
                               last = st.homeOd;
                       }
                   }
                  else{
                      int tm = 0;
                      if(ui->Time_box->currentText() == QString("1 hour")) tm = 3600;
                      if(ui->Time_box->currentText() == QString("3 hours")) tm = 3*3600;
                      if(ui->Time_box->currentText() == QString("8 hours")) tm = 8*3600;
                      if(ui->Time_box->currentText() == QString("12 hours")) tm = 12*3600;
                      for(auto st : u.second){
                          if(currentDateTime - tm < st.stamp)
                              last = st.homeOd;
                      }
                  }
                  if(last == 0)
                      last = g.homeOd;
                  sorted.push_back({abs(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0),{u.first.first,u.first.second}});
                  break;
              }
        }
        sort(sorted.begin(),sorted.end());
        reverse(sorted.begin(),sorted.end());





        int kol_up = 0;
        for(auto u : sorted){

               for (auto g : all[{u.second.first, u.second.second}]){
                   if((ui->Bookmaker_box->currentText() == g.bookmaker || ui->Bookmaker_box->currentText() == QString("ALL")) && (ui->League_box->currentText() == g.leagueName || ui->League_box->currentText() == QString("ALL"))){
                       ui->tableWidget->insertRow( ui->tableWidget->rowCount());
                       ui->tableWidget->setItem(kol_up, 0, new QTableWidgetItem(QString::number(u.second.first)));
                       ui->tableWidget->setItem(kol_up, 1, new QTableWidgetItem(g.bookmaker));
                       Book.insert(g.bookmaker);
                       ui->tableWidget->setItem(kol_up, 2, new QTableWidgetItem(g.leagueName));
                       League.insert(g.leagueName);
                       ui->tableWidget->setItem(kol_up, 3, new QTableWidgetItem(g.time));
                       ui->tableWidget->setItem(kol_up, 4, new QTableWidgetItem(g.home));
                       ui->tableWidget->setItem(kol_up, 5, new QTableWidgetItem(g.away));
                       ui->tableWidget->setItem(kol_up, 6, new QTableWidgetItem(QString::number(g.homeOd)));
                       ui->tableWidget->setItem(kol_up, 7, new QTableWidgetItem(QString::number(g.awayOd)));
                       QDateTime currentDateTime1 = QDateTime::currentDateTime();
                       double currentDateTime = currentDateTime1.toSecsSinceEpoch();
                       for(auto st : all[{u.second.first, u.second.second}]){
                               currentDateTime = st.stamp;
                               break;
                       }

                       double last = 0;
                       if(ui->Time_box->currentText() == QString("ALL")){
                            for(auto st : all[{u.second.first, u.second.second}]){
                                    last = st.homeOd;
                            }
                        }
                       else{
                           int tm = 0;
                           if(ui->Time_box->currentText() == QString("1 hour")) tm = 3600;
                           if(ui->Time_box->currentText() == QString("3 hours")) tm = 3*3600;
                           if(ui->Time_box->currentText() == QString("8 hours")) tm = 8*3600;
                           if(ui->Time_box->currentText() == QString("12 hours")) tm = 12*3600;
                           for(auto st : all[{u.second.first, u.second.second}]){
                               if(currentDateTime - tm < st.stamp)
                                   last = st.homeOd;
                           }
                       }
                       if(last == 0)
                           last = g.homeOd;
                       ui->tableWidget->setItem(kol_up, 8, new QTableWidgetItem(QString::number(round(((last-g.homeOd)/g.homeOd*100)*10)/10.0)));
                       ++kol_up;
                       break;
                   }


               }
        }
        //ui->tableWidget->sortItems(8, Qt::DescendingOrder);
    }


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
