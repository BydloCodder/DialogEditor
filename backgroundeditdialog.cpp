#include "backgroundeditdialog.h"
#include "ui_backgroundeditdialog.h"
#include <QImage>
#include <QPixmap>

BackgroundEditDialog::BackgroundEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackgroundEditDialog)
{
    ui->setupUi(this);
}

BackgroundEditDialog::~BackgroundEditDialog()
{
    delete ui;
}

void BackgroundEditDialog::setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *videos)
{
    this->backgrounds = backgrounds;
    this->videos = videos;

    ui->image_combobox->clear();
    ui->video_combobox->clear();
    foreach (auto k, backgrounds->keys()) {
        ui->image_combobox->addItem(k);
    }
    ui->image_combobox->model()->sort(0, Qt::AscendingOrder);
    ui->image_combobox->setCurrentIndex(0);

    foreach (auto k, videos->keys()) {
        ui->video_combobox->addItem(k);
    }
    ui->video_combobox->model()->sort(0, Qt::AscendingOrder);
    ui->video_combobox->setCurrentIndex(0);
}

void BackgroundEditDialog::represent()
{
    ui->background_box->setChecked(e->backgroundActive);
    ui->imageCheckbox->setChecked(!e->background->name.isEmpty());
    if (!e->background->name.isEmpty()) {
        ui->image_combobox->setCurrentText(e->background->name);
        ui->image_combobox->setEnabled(true);
    } else {
        ui->image_combobox->setEnabled(false);
    }
    ui->video_checkbox->setChecked(!e->background->video.isEmpty());
    if (!e->background->video.isEmpty()) {
        ui->video_combobox->setCurrentText(e->background->video);
        ui->video_combobox->setEnabled(true);
    } else
        ui->video_combobox->setEnabled(false);
    ui->fade_combobox->setChecked(e->background->fade > 0.0);
    if (e->background->fade > 0.0) {
        ui->fade->setEnabled(true);
        ui->fade->setValue(e->background->fade);
    }
    ui->clickable_checkbox->setCheckState(e->background->clickable == -1 ? Qt::Unchecked :
                                                                           e->background->clickable == 0 ? Qt::PartiallyChecked : Qt::Checked);

    if (!e->background->name.isEmpty() && backgrounds->contains(e->background->name)) {
        QImage img((*backgrounds)[e->background->name]);
        ui->preview->setPixmap(QPixmap::fromImage(img.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }
    ready = true;
}



void BackgroundEditDialog::on_imageCheckbox_toggled(bool checked)
{
    if (ready) {
        if (checked) {
            e->background->name = ui->image_combobox->currentText();
            ui->image_combobox->setEnabled(true);
        } else {
            e->background->name = "";
            ui->image_combobox->setEnabled(false);
        }
    }
}



void BackgroundEditDialog::on_image_combobox_currentTextChanged(const QString &arg1)
{
    if (ready) {
        if (ui->imageCheckbox->isChecked()) {
            e->background->name = arg1;
            if ((*backgrounds).contains(arg1)) {
                QImage img((*backgrounds)[e->background->name]);
                ui->preview->setPixmap(QPixmap::fromImage(img.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
            }
        }
    }
}


void BackgroundEditDialog::on_video_checkbox_toggled(bool checked)
{
    if (ready) {
        ui->video_combobox->setEnabled(checked);
        if (checked) e->background->video = ui->video_combobox->currentText();
        else e->background->video = "";
    }
}

void BackgroundEditDialog::on_video_combobox_currentTextChanged(const QString &arg1)
{
    if (ready) {
        if (ui->imageCheckbox->isChecked()) {
            e->background->video = arg1;
        }
    }
}

void BackgroundEditDialog::on_fade_combobox_toggled(bool checked)
{
    if (ready) {
        ui->fade->setEnabled(checked);
        if (!checked)
            e->background->fade = 0.0;
        else
            e->background->fade = ui->fade->value();
    }
}

void BackgroundEditDialog::on_fade_valueChanged(double arg1)
{
    if (ready) {
        if (ui->fade_combobox->isChecked()) {
            e->background->fade = arg1;
        }
    }
}

void BackgroundEditDialog::on_clickable_checkbox_stateChanged(int arg1)
{
    if (ready) {
        e->background->clickable = arg1 - 1;
    }
}



void BackgroundEditDialog::on_background_box_toggled(bool arg1)
{
    if (ready)
        e->backgroundActive = arg1;
}


























