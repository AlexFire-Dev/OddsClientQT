#include "graph_table.h"
#include "ui_graph_table.h"

Graph_table::Graph_table(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Graph_table)
{
    ui->setupUi(this);
}

Graph_table::~Graph_table()
{
    delete ui;
}
