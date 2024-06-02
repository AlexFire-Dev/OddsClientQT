#include "odds.h"
#include <graph_table.h>
#include "ui_odds.h"
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QtNetwork>
#include <QMessageBox>
#include <QtDebug>

odds::odds(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::odds)
{
    ui->setupUi(this);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->setRowCount(100);




}
odds::~odds()
{
    delete ui;
}


void odds::on_pushButton_clicked()
{

}

void odds::on_Apply_Button_clicked()
{
    this->filter();
}

void odds::on_Graph_clicked()
{
    int row = 0;
    if(ui->tableWidget->currentRow() != -1){
        row = ui->tableWidget->currentRow();
        int id = ui->tableWidget->item(row, 0)->data(Qt::DisplayRole).toInt();
        QString book = ui->tableWidget->item(row, 1)->data(Qt::DisplayRole).toString();
        std::vector <std::pair <int, std::pair <double, double>>> for_graph;
        Graph_table gr_win;
        for(auto u : all[{id,book}]){
            for_graph.push_back({u.stamp,{u.homeOd,u.awayOd}});
            gr_win.away = u.away;
            gr_win.home = u.home;
            gr_win.time = u.time;
        }
        gr_win.for_graph=for_graph;
        gr_win.setWindowTitle("GraphTable");
        gr_win.ready();
        gr_win.exec();
    }
    else{
        QMessageBox::warning(this, "ERROR", "Выберите строку!!!");
    }
}

void odds::on_Update_button_clicked()
{
    ready();
}
