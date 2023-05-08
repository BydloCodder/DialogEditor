#include "personeditor.h"
#include "ui_personeditor.h"

PersonEditor::PersonEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonEditor)
{
    ui->setupUi(this);
    setWindowTitle("Persons Editor");
}

void PersonEditor::represent(QString person)
{
    if (!e->persons->data.contains(person)) {
        e->persons->data[person] = new Event::Persons::Person();
    }
    ui->comboBox_4->setCurrentText(person);
}

void PersonEditor::show(QString arg1)
{
    if (!e->persons->data.contains(arg1)) {
        e->persons->data[arg1] = new Event::Persons::Person();
    }
    auto p = e->persons->data[arg1];
    ui->operation->setCurrentText(p->op);
    ui->person->setCurrentText(p->person);
    ui->mood->setCurrentText(p->mood);
    if (p->appear) {
        ui->appear_checkbox->setChecked(true);
        ui->appear_combobox->setCurrentText(p->appearAnimation);
        ui->appear_time->setValue(p->appearTime);
        ui->appear_backwards->setChecked(p->appearBackwards);
    } else {
        ui->appear_checkbox->setChecked(false);
        ui->appear_combobox->setCurrentText("");
        ui->appear_time->setValue(1.0);
        ui->appear_backwards->setChecked(false);
    }
    if (p->fade) {
        ui->fade_checkbox->setChecked(true);
        ui->fade_time->setValue(p->fadeTime);
        ui->fade_backwards->setChecked(p->fadeBackwards);
    } else {
        ui->fade_checkbox->setChecked(false);
        ui->fade_time->setValue(1.0);
        ui->fade_backwards->setChecked(false);
    }
}

void PersonEditor::setBase(Characters *c)
{
    this->c = c;
    foreach (auto p, c->persons.keys()) {
        ui->person->addItem(p);
    }
}

PersonEditor::~PersonEditor()
{
    delete ui;
}

void PersonEditor::on_comboBox_4_currentTextChanged(const QString &arg1)
{
    show(arg1);
}


void PersonEditor::on_operation_currentTextChanged(const QString &arg1)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->op = arg1;
    ui->person_box->setEnabled(arg1 != "Ignore");
}


void PersonEditor::on_person_currentTextChanged(const QString &arg1)
{
    if (!e)
        return;
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->person = arg1;

    QString oldMood = ui->mood->currentText().isEmpty() ? p->mood : ui->mood->currentText();
    ui->mood->clear();
    if (c->persons.contains(p->person)) {
        foreach (auto m, c->persons[p->person].keys()) {
            ui->mood->addItem(m);
        }
    }
    ui->mood->setCurrentText(oldMood);
}


void PersonEditor::on_mood_currentTextChanged(const QString &arg1)
{
    if (!e)
        return;
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->mood = arg1;
}


void PersonEditor::on_appear_checkbox_toggled(bool checked)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    ui->appear_combobox->setEnabled(checked);
    ui->appear_time->setEnabled(checked);
    ui->appear_backwards->setEnabled(checked);
    p->appear = checked;
    if (!ui->appear_combobox->currentText().isEmpty())
        p->appearAnimation = ui->appear_combobox->currentText();
}


void PersonEditor::on_appear_combobox_currentTextChanged(const QString &arg1)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->appearAnimation = arg1;
}


void PersonEditor::on_appear_time_valueChanged(double arg1)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->appearTime = arg1;
}


void PersonEditor::on_appear_backwards_toggled(bool checked)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->appearBackwards = checked;
}


void PersonEditor::on_fade_checkbox_toggled(bool checked)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];

    ui->fade_time->setEnabled(checked);
    ui->fade_backwards->setEnabled(checked);
    p->fade = checked;
}


void PersonEditor::on_fade_time_valueChanged(double arg1)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->fadeTime = arg1;
}


void PersonEditor::on_fade_backwards_toggled(bool checked)
{
    auto p = e->persons->data[ui->comboBox_4->currentText()];
    p->fadeBackwards = checked;
}

