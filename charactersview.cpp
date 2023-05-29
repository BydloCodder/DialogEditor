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
    ui->person_name->setText("");
    ui->mood_name->setText("");

    ui->persons_list->clear();
    dict->persons.remove("");
    foreach (auto k, dict->persons.keys()) {
        ui->persons_list->addItem(k);
    }
    ui->moods_box->setEnabled(false);
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
        dict->characters[ui->tableWidget->item(i, 0)->text()] = c;
    }
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
    auto fn = QFileDialog::getOpenFileName();
    if (fn.isEmpty())
        fn = "new_character";

    QFileInfo info(fn);
    QString name = info.exists() ? info.baseName() : fn;
    if (dict->characters.contains(name))
        name = name + QString::number(QDateTime::currentMSecsSinceEpoch());
    Characters::Character c;
    c.name = info.baseName();
    c.portrait = fn;
    dict->characters[name] = c;
    mapDict();
}

void CharactersView::on_persons_list_currentTextChanged(const QString &currentText)
{
    if (!currentText.isEmpty()) {
        ui->moods_list->clear();
        foreach (auto s, dict->persons[currentText].keys())
            ui->moods_list->addItem(s);
        ui->person_name->setText(currentText);
        ui->moods_box->setEnabled(true);
    }
}


void CharactersView::on_moods_list_currentTextChanged(const QString &currentText)
{
    if (!currentText.isEmpty()) {
        ui->mood_name->setText(currentText);
        ui->mood_file->setText(dict->persons[ui->persons_list->currentItem()->text()][currentText]);
    }
}

void CharactersView::on_mood_name_returnPressed()
{
    QString s = ui->mood_name->text();
    QString oldKey = ui->moods_list->currentItem()->text();
    QString person = ui->persons_list->currentItem()->text();
    QString value =  dict->persons[person][oldKey];
    dict->persons[person][s] = value;
    dict->persons[person].remove(oldKey);
    ui->moods_list->currentItem()->setText(s);
}

void CharactersView::on_person_name_returnPressed()
{
    int currentRow = ui->persons_list->currentRow();
    if (currentRow >= 0) {
        QString oldName = ui->persons_list->currentItem()->text();
        dict->persons[ui->person_name->text()] = dict->persons[oldName];
        dict->persons.remove(oldName);
        ui->persons_list->currentItem()->setText(ui->person_name->text());
    }
}

void CharactersView::on_pushButton_4_clicked()
{
    QHash<QString, QString> moods;
    dict->persons["new person"] = moods;
    mapDict();
}

void CharactersView::on_pushButton_5_clicked()
{
    if (ui->persons_list->currentRow() >= 0 && !ui->persons_list->currentItem()->text().isEmpty()) {
        dict->persons.remove(ui->persons_list->currentItem()->text());
        mapDict();
    }
}

void CharactersView::on_pushButton_6_clicked()
{
    int pRow = ui->persons_list->currentRow();
    if (pRow >= 0) {
        dict->persons[ui->persons_list->currentItem()->text()]["newMood"] = "";
        mapDict();
    }
}

void CharactersView::on_mood_file_returnPressed()
{
    int pRow = ui->persons_list->currentRow();
    int mRow = ui->moods_list->currentRow();
    if (pRow >= 0 && mRow >= 0) {
        dict->persons[ui->persons_list->currentItem()->text()][ui->moods_list->currentItem()->text()] = ui->mood_file->text();
        mapDict();
    }
}



void CharactersView::on_pushButton_7_clicked()
{
    int pRow = ui->persons_list->currentRow();
    int mRow = ui->moods_list->currentRow();
    if (pRow >= 0 && mRow >= 0) {
        dict->persons[ui->persons_list->currentItem()->text()].remove(ui->moods_list->currentItem()->text());
        mapDict();
    }
}

void CharactersView::on_openfile_button_clicked()
{
    int pRow = ui->persons_list->currentRow();
    int mRow = ui->moods_list->currentRow();
    if (pRow >= 0 && mRow >= 0) {
        QString filename = QFileDialog::getOpenFileName();
        if (!filename.isEmpty()) {
            dict->persons[ui->persons_list->currentItem()->text()][ui->moods_list->currentItem()->text()] = filename;
            mapDict();
        }
    }
}
