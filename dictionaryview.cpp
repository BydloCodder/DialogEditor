#include "dictionaryview.h"
#include "ui_dictionaryview.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QStringList>

DictionaryView::DictionaryView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictionaryView)
{
    ui->setupUi(this);
    QHeaderView* header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void DictionaryView::mapDict()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(dict->count());

    int rowIndex = 0;
    foreach (auto k, dict->keys()) {
        ui->tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(k));
        ui->tableWidget->setItem(rowIndex, 1, new QTableWidgetItem((*dict)[k]));
        rowIndex++;
    }
}

DictionaryView::~DictionaryView()
{
    delete ui;
}

void DictionaryView::setColumnNames(QString key, QString value)
{
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << key << value);
}

void DictionaryView::on_pushButton_3_clicked()
{
    dict->clear();
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        (*dict)[ui->tableWidget->item(i, 0)->text()] = ui->tableWidget->item(i, 1)->text();
    }
    mapDict();
    emit editingFinished();
    this->close();
}


void DictionaryView::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row >= 0) {
        auto key = ui->tableWidget->item(row, 0)->text();
        if (dict->contains(key))
            dict->remove(key);
        ui->tableWidget->removeRow(row);
        mapDict();
    }
}


void DictionaryView::on_pushButton_clicked()
{
    if (addFiles) {
        auto fileList = QFileDialog::getOpenFileNames();
        if (!fileList.empty()) {
            foreach (auto fn, fileList) {
                QFileInfo info(fn);
                QString name = info.baseName();
                if (dict->contains(name))
                    name = name + QString::number(QDateTime::currentMSecsSinceEpoch());
                (*dict)[name] = fn;
            }
            mapDict();
        }
    } else {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    }
}

