#include "charactersview.h"
#include "ui_charactersview.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QDateTime>

CharactersView::CharactersView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CharactersView)
{
    ui->setupUi(this);
    QHeaderView* header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void CharactersView::mapDict()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(dict->characters.count());

    int rowIndex = 0;
    foreach (auto k, dict->characters.keys()) {
        ui->tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(k));
        ui->tableWidget->setItem(rowIndex, 1, new QTableWidgetItem((*dict).characters[k].name));
        ui->tableWidget->setItem(rowIndex, 2, new QTableWidgetItem((*dict).characters[k].portrait));
        rowIndex++;
    }
}

CharactersView::~CharactersView()
{
    delete ui;
}

void CharactersView::on_pushButton_3_clicked()
{
    dict->characters.clear();
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        Characters::Character c;
        c.name = ui->tableWidget->item(i, 1)->text();
        c.portrait = ui->tableWidget->item(i, 2)->text();
        (*dict).characters[ui->tableWidget->item(i, 0)->text()] = c;
    }
    mapDict();
    emit editingFinished();
    this->close();
}


void CharactersView::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row >= 0) {
        auto key = ui->tableWidget->item(row, 0)->text();
        if (dict->characters.contains(key))
            dict->characters.remove(key);
        ui->tableWidget->removeRow(row);
        mapDict();
    }
}


void CharactersView::on_pushButton_clicked()
{
    auto fileList = QFileDialog::getOpenFileNames();
    if (!fileList.empty()) {
        foreach (auto fn, fileList) {
            QFileInfo info(fn);
            QString name = info.baseName();
            if (dict->characters.contains(name))
                name = name + QString::number(QDateTime::currentMSecsSinceEpoch());
            Characters::Character c;
            c.name = info.baseName();
            c.portrait = fn;
            (*dict).characters[name] = c;
        }
        mapDict();
    }
}

