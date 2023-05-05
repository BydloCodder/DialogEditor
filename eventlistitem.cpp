#include "eventlistitem.h"
#include "ui_eventlistitem.h"
#include <QDebug>
#include "soundeditdialog.h"
#include "backgroundeditdialog.h"
#include "dictionaryview.h"
#include "choiceseditdialog.h"
#include "conditioneditor.h"

EventListItem::EventListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventListItem)
{
    ui->setupUi(this);

    connect(ui->bgPreview, &ClickableLabel::clicked, this, [this]() {

        BackgroundEditDialog * dialog = new BackgroundEditDialog();
        dialog->setWindowTitle("Background Event Editor");
        dialog->setEvent(e);
        dialog->setBase(backgrounds, videos);
        dialog->represent();
        dialog->exec();
        represent();

        qDebug() << "clicked bg";
    });
}

void EventListItem::setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *sounds, QHash<QString, QString> *videos, Characters *characters, QStringList *idList)
{

    this->backgrounds = backgrounds;
    this->sounds = sounds;
    this->videos = videos;
    this->characters = characters;
    this->idList = idList;

    ui->characters->clear();
    foreach (auto c, characters->characters.keys()) {
        ui->characters->addItem(c);
    }
    ui->jumpto->clear();
    foreach (auto s, *idList) {
        ui->jumpto->addItem(s);
    }
}

void EventListItem::represent()
{
    ui->text->setPlainText(e->text);
    if (e->character == ":ignore:") {
        ui->characterCombobox->setChecked(false);
        ui->characters->setEnabled(false);
    } else {
        ui->characterCombobox->setChecked(true);
        ui->characters->setEnabled(true);
        ui->characters->setCurrentText(e->character);
    }
    if (e->id.isEmpty()) {
        ui->idCombobox->setChecked(false);
        ui->id->setText("");
        ui->id->setEnabled(false);
    } else {
        ui->idCombobox->setChecked(true);
        ui->id->setEnabled(true);
        ui->id->setText(e->id);
    }
    if (e->jump.isEmpty()) {
        ui->jumpCombobox->setChecked(false);
        ui->jumpto->setCurrentText("");
        ui->jumpto->setEnabled(false);
    } else {
        ui->jumpCombobox->setChecked(true);
        ui->jumpto->setEnabled(true);
        ui->jumpto->setCurrentText(e->jump);
    }

    if (e->conditionActive) {
        ui->checkBox->setChecked(true);
        QString s = QString("if " + e->condition.toString());
        ui->checkBox->setText(s.length() > 30 ? s.first(30) : s);
        ui->checkBox->setToolTip("if " + e->condition.toString());
    } else {
        ui->checkBox->setChecked(false);
        ui->checkBox->setText("Condition");
        ui->checkBox->setToolTip("");
    }

    if (e->backgroundActive) {
        ui->backgroundCombobox->setChecked(true);
        if (!e->background.name.isEmpty() && backgrounds->contains(e->background.name)) {
            QImage img((*backgrounds)[e->background.name]);
            ui->bgPreview->setPixmap(QPixmap::fromImage(img.scaled(ui->bgPreview->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        } else {
            ui->bgPreview->clear();
            QString s = "[";

            if (e->background.clickable >= 0)
                s += QString(" clickable: ") + (e->background.clickable ? "true;" : "false;");
            else if (!e->background.video.isEmpty())
                s += " video: " + e->background.video + ";";
            else s += " clean;";
            s += " ]";
            ui->bgPreview->setText(s);
        }
    } else {
        ui->backgroundCombobox->setChecked(false);
        ui->bgPreview->clear();
    }

    if (e->playSoundActive) {
        ui->soundButton->setText("Play sound [" + e->playSound.name + "->" +e->playSound.channel + "]");
        ui->soundButton->setChecked(true);
    } else {
        ui->soundButton->setText("Sound");
        ui->soundButton->setChecked(false);
    }
    if (!e->stopSound.isEmpty()) {
        ui->soundButton->setText("Stop sound [" + e->stopSound + "]");
        ui->soundButton->setChecked(true);
    } else if (!e->playSoundActive) {
        ui->soundButton->setText("Sound");
        ui->soundButton->setChecked(false);
    }

    if (!e->state.empty()) {
        ui->statsButton->setText("State [" + QString::number(e->state.count()) + "]");
        ui->statsButton->setChecked(true);
    } else {
        ui->statsButton->setText("Stats");
        ui->statsButton->setChecked(false);
    }

    if (!e->choices.empty()) {
        ui->choicesButton->setText("Choices [" + QString::number(e->choices.count()) + "]");
        ui->choicesButton->setChecked(true);
    } else {
        ui->choicesButton->setText("Choices");
        ui->choicesButton->setChecked(false);
    }

    if (e->timer > 0) {
        ui->timer_checkbox->setChecked(true);
        ui->timer->setValue(e->timer);
        ui->timer->setEnabled(true);
    } else {
        ui->timer_checkbox->setChecked(false);
        ui->timer->setEnabled(false);
    }
    ready = true;
}

EventListItem::~EventListItem()
{
    delete ui;
}

void EventListItem::on_characterCombobox_toggled(bool checked)
{
    ui->characters->setEnabled(checked);
    if (ready) e->character = checked ? ui->characters->currentText() : "";
}


void EventListItem::on_jumpCombobox_toggled(bool checked)
{
    ui->jumpto->setEnabled(checked);
    if (ready)
        e->jump = checked ? ui->jumpto->currentText() : "";
}


void EventListItem::on_idCombobox_toggled(bool checked)
{
    ui->id->setEnabled(checked);
    if (ready) {
        e->id = checked ? ui->id->text() : "";
        if (checked && !ui->id->text().isEmpty() && !idList->contains(ui->id->text())) {
                idList->append(ui->id->text());
                idList->removeDuplicates();
        }
    }
}


void EventListItem::on_backgroundCombobox_clicked()
{
}


void EventListItem::on_soundButton_clicked()
{
    auto dialog = new SoundEditDialog();
    dialog->setWindowTitle("Sound Event Config");
    dialog->setEvent(e);
    dialog->setBase(sounds);
    dialog->represent();
    dialog->exec();
    this->represent();
    dialog->deleteLater();
}


void EventListItem::on_backgroundCombobox_toggled(bool arg1)
{
    e->backgroundActive = arg1;
    represent();
}


void EventListItem::on_characters_currentTextChanged(const QString &arg1)
{
    if (ready) e->character = arg1;
}


void EventListItem::on_jumpto_currentTextChanged(const QString &arg1)
{
    if (ready) e->jump = arg1;
}


void EventListItem::on_id_editingFinished()
{
    if (ready && ui->idCombobox->isChecked()) {
        e->id = ui->id->text();
        if (!idList->contains(ui->id->text()))
            idList->append(ui->id->text());
        idList->removeDuplicates();
    }
}


void EventListItem::on_text_textChanged()
{
    if (ready) e->text = ui->text->toPlainText();
}


void EventListItem::on_statsButton_clicked()
{
    auto dialog = new DictionaryView();
    dialog->setColumnNames("Key", "Value");
    dialog->setWindowTitle("Stats");
    dialog->dict = &e->state;
    dialog->addFiles = false;
    dialog->mapDict();
    dialog->exec();
    represent();
    dialog->deleteLater();
}


void EventListItem::on_choicesButton_clicked()
{
    auto dialog = new ChoicesEditDialog();
    dialog->setWindowTitle("Choices Edit");
    dialog->choices = &e->choices;
    dialog->setBase(backgrounds, sounds, videos, characters,idList);
    dialog->represent();
    dialog->exec();
    represent();
    dialog->deleteLater();
}


void EventListItem::on_timer_checkbox_toggled(bool checked)
{
    if (ready) {
        ui->timer->setEnabled(checked);
        e->timer = checked ? ui->timer->value() : 0.0;
    }
}


void EventListItem::on_id_returnPressed()
{
    on_id_editingFinished();
}


void EventListItem::on_timer_valueChanged(double arg1)
{
    if (ready)
        e->timer = arg1;
}


void EventListItem::on_checkBox_toggled(bool checked)
{
    if (ready) {
        if (checked) {
            auto dialog = new ConditionEditor();
            dialog->condition = &e->condition;
            dialog->represent();
            dialog->exec();
            e->conditionActive = true;
            represent();
            dialog->deleteLater();
        } else {
            e->conditionActive = false;
            represent();
        }
    }
}

