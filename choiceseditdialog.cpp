#include "choiceseditdialog.h"
#include "eventlistitem.h"
#include "ui_choiceseditdialog.h"
#include "scriptdialog.h"
#include <QModelIndex>

ChoicesEditDialog::ChoicesEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoicesEditDialog)
{
    ui->setupUi(this);

    auto model = ui->choices_list->model();
    connect(model, &QAbstractItemModel::rowsMoved, this, [this](QModelIndex, int start, int, QModelIndex, int end) {
        choices->swapItemsAt(start, end);
    });
}

ChoicesEditDialog::~ChoicesEditDialog()
{
    delete ui;
}

void ChoicesEditDialog::setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *sounds, QHash<QString, QString> *videos, Characters *characters, QStringList *idList)
{
    this->backgrounds = backgrounds;
    this->sounds = sounds;
    this->videos = videos;
    this->characters = characters;
    this->idList = idList;
}

void ChoicesEditDialog::represent()
{
    int prevRow = ui->choices_list->currentRow();
    ui->choices_list->clear();
    foreach (auto c, *choices) {
        ui->choices_list->addItem(c->text);
    }
    if (!ready) {
        ready = true;
        ui->choices_list->setCurrentRow(0);
    } else {
        ui->choices_list->setCurrentRow(prevRow);
    }
    ui->choices_list->focusWidget();
}

void ChoicesEditDialog::on_choices_list_currentRowChanged(int currentRow)
{
    if (ready && currentRow >= 0 && choices->count() > 0) {
        ui->choices_text->setText((*choices)[currentRow]->text);
        ui->events_list->clear();

        for (int i = 0; i < (*choices)[currentRow]->events.count(); i++) {
            auto evPointer = (*choices)[currentRow]->events[i];
            auto itemWidget = new EventListItem();
            itemWidget->setBase(backgrounds, sounds, videos, characters, idList);
            itemWidget->setEvent(evPointer);
            itemWidget->represent();
            itemWidget->setFocusPolicy(Qt::NoFocus);

            QListWidgetItem * item = new QListWidgetItem();
            ui->events_list->addItem(item);
            ui->events_list->setItemWidget(item, itemWidget);
        }
        ui->script->setChecked(!((*choices)[currentRow]->condition.isEmpty()));
    }
}


void ChoicesEditDialog::on_pushButton_clicked()
{
    auto c = new Event::Choice();
    c->text = "New choice";
    choices->append(c);
    ui->choices_list->addItem(c->text);
    ui->choices_list->setCurrentRow(ui->choices_list->count() - 1);
}

void ChoicesEditDialog::on_choices_text_returnPressed()
{
    if (ui->choices_list->currentRow() >= 0) {
        (*choices)[ui->choices_list->currentRow()]->text = ui->choices_text->text();
        ui->choices_list->item(ui->choices_list->currentRow())->setText(ui->choices_text->text());
    }
}


void ChoicesEditDialog::on_pushButton_2_clicked()
{
    if (ui->choices_list->currentRow() >= 0) {
        int currentRow = ui->choices_list->currentRow();
        delete ui->choices_list->takeItem(currentRow);
        choices->removeAt(currentRow);
        if (ui->choices_list->count() == 0) {
            ui->events_list->clear();
        }
    }
}


void ChoicesEditDialog::on_addEventButton_clicked()
{
    int choiceIndex = ui->choices_list->currentRow();
    if (choiceIndex >= 0 && choices->count() > 0) {
        auto e = new Event();
        ((*choices)[choiceIndex])->events.append(e);
        represent();
    }
}


void ChoicesEditDialog::on_removeEventButton_clicked()
{
    int currentRow = ui->events_list->currentRow();
    int choiceRow = ui->choices_list->currentRow();
    if (currentRow >= 0) {
        (*choices)[choiceRow]->events.removeAt(currentRow);
        auto item = ui->events_list->takeItem(currentRow);
        delete item;
    }
}

void ChoicesEditDialog::on_choices_list_currentTextChanged(const QString &currentText)
{

}


void ChoicesEditDialog::on_script_clicked()
{
    if (ready) {
        auto dialog = new ScriptDialog();
        dialog->c = (*choices)[ui->choices_list->currentRow()];
        dialog->represent();
        dialog->exec();
        represent();
        dialog->deleteLater();
    }
}

