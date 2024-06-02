#ifndef GRAPH_TABLE_H
#define GRAPH_TABLE_H

#include <QDialog>
#include <ui_graph_table.h>
#include <qcustomplot.h>
#include <QVector>
#include <cmath>
#include <algorithm>

namespace Ui {
class Graph_table;
}

class Graph_table : public QDialog
{
    Q_OBJECT

public:
    std::vector <std::pair <int, std::pair <double, double>>> for_graph;
    QString home;
    QString away;
    QString time;
    void ready(){
        ui->widget->xAxis->setRange(0,for_graph.size());
        QVector <double> home_gr;
        QVector <double> away_gr;
        QVector <double> stamp;
        int kol = 0;
        double mn = 1000.0;
        double mx = -1000.0;
        for(auto u : for_graph){
            //stamp.push_back(u.first);
            stamp.push_back(kol++);
            if(mn > u.second.first)
                mn = u.second.first;
            if(mn > u.second.second)
                mn = u.second.second;
            if(mx < u.second.first)
                mx = u.second.first;
            if(mx < u.second.second)
                mx = u.second.second;
            home_gr.push_back(u.second.first);
            away_gr.push_back(u.second.second);
        }
        if(mn == 1000000000 || mx == -1000000000)
            ui->widget->yAxis->setRange(-10,10);
        else
            ui->widget->yAxis->setRange(mn-1,mx+1);

        ui->widget->xAxis->setLabel("Time");
        ui->widget->yAxis->setLabel("Odd");
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(stamp,home_gr);
        ui->widget->graph(0)->setPen(QPen(Qt::blue,3));
        ui->widget->addGraph();
        ui->widget->graph(1)->addData(stamp,away_gr);
        ui->widget->graph(1)->setPen(QPen(Qt::red,3));
        ui->Away->setText(away);
        ui->Home->setText(home);
        ui->widget->replot();

    }
    explicit Graph_table(QWidget *parent = nullptr);
    ~Graph_table();

private:
    Ui::Graph_table *ui;
};

#endif // GRAPH_TABLE_H
