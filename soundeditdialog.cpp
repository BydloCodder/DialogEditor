#include "soundeditdialog.h"
#include "ui_soundeditdialog.h"

SoundEditDialog::SoundEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SoundEditDialog)
{
    ui->setupUi(this);
}

void SoundEditDialog::setBase(QHash<QString, QString> *sounds)
{
    this->sounds = sounds;
    ui->sound->clear();
    foreach (auto k, sounds->keys()) {
        ui->sound->addItem(k);
    }
    ui->sound->setCurrentIndex(0);
}

void SoundEditDialog::represent()
{
    ui->playBox->setChecked(e->playSoundActive);
    ui->stopBox->setChecked(!e->stopSound.isEmpty());
    ui->stopsound_channel->setText(e->stopSound);
    ui->playsound_channel->setText(e->playSound->channel);
    ui->bus->setText(e->playSound->bus);
    ui->fade_checkbox->setChecked(e->playSound->fade != 0.0);
    ui->fade->setValue(e->playSound->fade);
    ui->volume->setValue(e->playSound->volume * 100);
    ui->sound->setCurrentText(e->playSound->name);
    ui->loop->setChecked(e->playSound->loop);
    ready = true;
}

SoundEditDialog::~SoundEditDialog()
{
    delete ui;
}

void SoundEditDialog::on_playBox_toggled(bool arg1)
{
    if (ready) {
        e->playSoundActive = arg1;
        e->playSound->name = ui->sound->currentText();
    }
}


void SoundEditDialog::on_stopBox_toggled(bool arg1)
{
    if (ready) e->stopSound = arg1 ? ui->stopsound_channel->text() : "";
}


void SoundEditDialog::on_stopsound_channel_editingFinished()
{
    if (ready)
        if (ui->stopBox->isChecked()) {
            e->stopSound = ui->stopsound_channel->text();
        }
}


void SoundEditDialog::on_loop_stateChanged(int arg1)
{
    if (ready) e->playSound->loop = arg1;
}


void SoundEditDialog::on_fade_valueChanged(double arg1)
{
    if (ready) e->playSound->fade = arg1;
}


void SoundEditDialog::on_fade_checkbox_toggled(bool checked)
{
    if (ready) e->playSound->fade = checked ? ui->fade->value() : 0.0;
}


void SoundEditDialog::on_volume_sliderReleased()
{
    if (ready) e->playSound->volume = 0.01 * ui->volume->value();
}


void SoundEditDialog::on_bus_editingFinished()
{
    if (ready) e->playSound->bus = ui->bus->text();
}


void SoundEditDialog::on_sound_currentTextChanged(const QString &arg1)
{
    if (ready) e->playSound->name = arg1;
}


void SoundEditDialog::on_playsound_channel_editingFinished()
{
    if (ready) e->playSound->channel = ui->playsound_channel->text();
}

