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
{}

void odds::on_Update_button_clicked()
{
    ready();
}
