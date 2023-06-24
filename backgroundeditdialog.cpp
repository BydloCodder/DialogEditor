#include "backgroundeditdialog.h"
#include "ui_backgroundeditdialog.h"
#include <QImage>
#include <QPixmap>
#include <QColorDialog>

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
    ui->clickable_checkbox->setCheckState(e->background->clickable == -1 ? Qt::Unchecked :
                                                                           e->background->clickable == 0 ? Qt::PartiallyChecked : Qt::Checked);

    if (!e->background->name.isEmpty() && backgrounds->contains(e->background->name)) {
        QImage img((*backgrounds)[e->background->name]);
        ui->preview->setPixmap(QPixmap::fromImage(img.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }

    if (e->background->transition) {
        ui->transition_combobox->setChecked(true);

        ui->swipe_groupbox->setChecked(e->background->swipe_mode_h || e->background->swipe_mode_v);
        ui->swipe_h_checkbox->setChecked(e->background->swipe_mode_h);
        ui->swipe_v_checkbox->setChecked(e->background->swipe_mode_v);
        ui->swipe_h_speed->setValue(e->background->swipe_speed_h);
        ui->swipe_v_speed->setValue(e->background->swipe_speed_v);
        ui->swipe_h_min->setValue(e->background->swipe_min_h);
        ui->swipe_h_max->setValue(e->background->swipe_max_h);
        ui->swipe_v_min->setValue(e->background->swipe_min_v);
        ui->swipe_v_max->setValue(e->background->swipe_max_v);
        ui->swipe_h_shift->setValue(e->background->swipe_shift_h);
        ui->swipe_v_shift->setValue(e->background->swipe_shift_v);

        ui->scale_groupbox->setChecked(e->background->scale_mode);
        ui->scale_speed->setValue(e->background->scale_speed);
        ui->scale_min->setValue(e->background->scale_min);
        ui->scale_max->setValue(e->background->scale_max);
        ui->scale_shift->setValue(e->background->scale_shift);

        ui->shake_groupbox->setChecked(e->background->shake_mode);
        ui->shake_horizontal->setChecked(e->background->shake_h);
        ui->shake_vertical->setChecked(e->background->shake_v);
        ui->shake_speed->setValue(e->background->shake_speed);
        ui->shake_height->setValue(e->background->shake_height);
        ui->shake_time->setValue(e->background->shake_time);

        ui->blend_checkbox->setChecked(e->background->blend_mode);
        ui->blend_speed->setValue(e->background->blend_speed);

        ui->fade_groupbox->setChecked(e->background->fade_to || e->background->fade_from);
        ui->fade_from->setChecked(e->background->fade_from);
        ui->fade_to->setChecked(e->background->fade_to);
        ui->fade_speed->setValue(e->background->fade_speed);
        ui->color->setText(e->background->fade_color);

        ui->slide_groupbox->setChecked(e->background->slide_h || e->background->slide_v);
        ui->slide_horizontal->setChecked(e->background->slide_h);
        ui->slide_vertical->setChecked(e->background->slide_v);
        ui->slide_reverse->setChecked(e->background->slide_reverse);
        ui->slide_speed->setValue(e->background->slide_speed);

        ui->curtain_h->setChecked(e->background->curtain_h);
        ui->curtain_v->setChecked(e->background->curtain_v);
        ui->curtain_reverse_h->setChecked(e->background->curtain_reverse_h);
        ui->curtain_reverse_v->setChecked(e->background->curtain_reverse_v);
        ui->curtain_speed_h->setValue(e->background->curtain_speed_h);
        ui->curtain_speed_v->setValue(e->background->curtain_speed_v);

    } else {
        ui->transition_combobox->setChecked(false);
    }

    ready = true;
}



void BackgroundEditDialog::on_imageCheckbox_toggled(bool checked)
{
    if (ready) {
        if (checked) {
            e->background->name = ui->image_combobox->currentText();
            ui->image_combobox->setEnabled(true);
            if (!e->background->name.isEmpty()) {
                on_image_combobox_currentTextChanged(e->background->name);
            }
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



void BackgroundEditDialog::on_transition_combobox_toggled(bool arg1)
{
    if (!ready) return;
    e->background->transition = arg1;
}

void BackgroundEditDialog::on_swipe_h_checkbox_toggled(bool checked)
{
    if (!ready) return;
    e->background->swipe_mode_h = checked;
}


void BackgroundEditDialog::on_swipe_v_checkbox_toggled(bool checked)
{
    if (!ready) return;
    e->background->swipe_mode_v = checked;
}


void BackgroundEditDialog::on_swipe_h_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->swipe_speed_h = arg1;
}


void BackgroundEditDialog::on_swipe_v_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->swipe_speed_v = arg1;
}


void BackgroundEditDialog::on_swipe_h_min_valueChanged(int arg1)
{
    if (!ready) return;
    e->background->swipe_min_h = arg1;
}


void BackgroundEditDialog::on_swipe_v_min_valueChanged(int arg1)
{
    if (!ready) return;
    e->background->swipe_min_v = arg1;
}


void BackgroundEditDialog::on_swipe_h_max_valueChanged(int arg1)
{
    if (!ready) return;
    e->background->swipe_max_h = arg1;
}


void BackgroundEditDialog::on_swipe_v_max_valueChanged(int arg1)
{
    if (!ready) return;
    e->background->swipe_max_v = arg1;
}


void BackgroundEditDialog::on_swipe_h_shift_valueChanged(int arg1)
{
    if (!ready) return;
    e->background->swipe_shift_h = arg1;
}


void BackgroundEditDialog::on_swipe_v_shift_valueChanged(int arg1)
{
    if (!ready) return;
    e->background->swipe_shift_v = arg1;
}


void BackgroundEditDialog::on_scale_groupbox_toggled(bool arg1)
{
    if (!ready) return;
    e->background->scale_mode = arg1;
}


void BackgroundEditDialog::on_scale_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->scale_speed = arg1;
}


void BackgroundEditDialog::on_scale_min_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->scale_min = arg1;
}


void BackgroundEditDialog::on_scale_max_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->scale_max = arg1;
}


void BackgroundEditDialog::on_scale_shift_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->scale_shift = arg1;
}


void BackgroundEditDialog::on_shake_groupbox_toggled(bool arg1)
{
    if (!ready) return;
    e->background->shake_mode = arg1;
}


void BackgroundEditDialog::on_shake_horizontal_toggled(bool checked)
{
    if (!ready) return;
    e->background->shake_h = checked;
}


void BackgroundEditDialog::on_shake_vertical_toggled(bool checked)
{
    if (!ready) return;
    e->background->shake_v = checked;
}


void BackgroundEditDialog::on_shake_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->shake_speed = arg1;
}


void BackgroundEditDialog::on_shake_height_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->shake_height = arg1;
}


void BackgroundEditDialog::on_shake_time_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->shake_time = arg1;
}


void BackgroundEditDialog::on_blend_checkbox_toggled(bool arg1)
{
    if (!ready) return;
    e->background->blend_mode = arg1;
}


void BackgroundEditDialog::on_blend_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->blend_speed = arg1;
}


void BackgroundEditDialog::on_fade_to_toggled(bool checked)
{
    if (!ready) return;
    e->background->fade_to = checked;
}


void BackgroundEditDialog::on_fade_from_toggled(bool checked)
{
    if (!ready) return;
    e->background->fade_from = checked;
}


void BackgroundEditDialog::on_color_textChanged(const QString &arg1)
{
    if (!ready) return;
    e->background->fade_color = arg1;
}


void BackgroundEditDialog::on_fade_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->fade_speed = arg1;
}


void BackgroundEditDialog::on_slide_horizontal_toggled(bool checked)
{
    if (!ready) return;
    e->background->slide_h = checked;
}


void BackgroundEditDialog::on_slide_vertical_toggled(bool checked)
{
    if (!ready) return;
    e->background->slide_v = checked;
}


void BackgroundEditDialog::on_slide_speed_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->slide_speed = arg1;
}


void BackgroundEditDialog::on_slide_reverse_toggled(bool checked)
{
    if (!ready) return;
    e->background->slide_reverse = checked;
}


void BackgroundEditDialog::on_pick_button_clicked()
{
    auto result = QColorDialog::getColor(Qt::white, 0, "", QColorDialog::ShowAlphaChannel);
    if (result.isValid()) {
        QString argb = result.name(QColor::HexArgb);
        QString clr = argb.first(1) + argb.mid(3) + argb.mid(1, 2);
        ui->color->setText(clr);
    }
}



void BackgroundEditDialog::on_slide_groupbox_toggled(bool arg1)
{
    if (!ready) return;
    if (!arg1) {
        ui->slide_horizontal->setChecked(false);
        ui->slide_vertical->setChecked(false);
    }
}


void BackgroundEditDialog::on_fade_groupbox_toggled(bool arg1)
{
    if (!ready) return;
    if (!arg1) {
        ui->fade_from->setChecked(false);
        ui->fade_to->setChecked(false);
    }
}


void BackgroundEditDialog::on_curtain_h_toggled(bool checked)
{
    if (!ready) return;
    e->background->curtain_h = checked;
}


void BackgroundEditDialog::on_curtain_v_toggled(bool checked)
{
    if (!ready) return;
    e->background->curtain_v = checked;
}


void BackgroundEditDialog::on_curtain_reverse_h_toggled(bool checked)
{
    if (!ready) return;
    e->background->curtain_reverse_h = checked;
}


void BackgroundEditDialog::on_curtain_reverse_v_toggled(bool checked)
{
    if (!ready) return;
    e->background->curtain_reverse_v = checked;
}


void BackgroundEditDialog::on_curtain_speed_h_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->curtain_speed_h = arg1;
}


void BackgroundEditDialog::on_curtain_speed_v_valueChanged(double arg1)
{
    if (!ready) return;
    e->background->curtain_speed_v = arg1;
}

