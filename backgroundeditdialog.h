#ifndef BACKGROUNDEDITDIALOG_H
#define BACKGROUNDEDITDIALOG_H

#include <QDialog>
#include <QHash>
#include "datatypes.h"

namespace Ui {
class BackgroundEditDialog;
}

class BackgroundEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BackgroundEditDialog(QWidget *parent = nullptr);
    ~BackgroundEditDialog();
    void setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *videos);
    void setEvent(Event *e) {this->e = e;}
    void represent();


private slots:
    void on_imageCheckbox_toggled(bool checked);

    void on_image_combobox_currentTextChanged(const QString &arg1);

    void on_video_checkbox_toggled(bool checked);

    void on_video_combobox_currentTextChanged(const QString &arg1);

    void on_clickable_checkbox_stateChanged(int arg1);

    void on_background_box_toggled(bool arg1);

    void on_transition_combobox_toggled(bool arg1);

    void on_swipe_h_checkbox_toggled(bool checked);

    void on_swipe_v_checkbox_toggled(bool checked);

    void on_swipe_h_speed_valueChanged(double arg1);

    void on_swipe_v_speed_valueChanged(double arg1);

    void on_swipe_h_min_valueChanged(int arg1);

    void on_swipe_v_min_valueChanged(int arg1);

    void on_swipe_h_max_valueChanged(int arg1);

    void on_swipe_v_max_valueChanged(int arg1);

    void on_swipe_h_shift_valueChanged(int arg1);

    void on_swipe_v_shift_valueChanged(int arg1);

    void on_scale_groupbox_toggled(bool arg1);

    void on_scale_speed_valueChanged(double arg1);

    void on_scale_min_valueChanged(double arg1);

    void on_scale_max_valueChanged(double arg1);

    void on_scale_shift_valueChanged(double arg1);

    void on_shake_groupbox_toggled(bool arg1);

    void on_shake_horizontal_toggled(bool checked);

    void on_shake_vertical_toggled(bool checked);

    void on_shake_speed_valueChanged(double arg1);

    void on_shake_height_valueChanged(double arg1);

    void on_shake_time_valueChanged(double arg1);

    void on_blend_checkbox_toggled(bool arg1);

    void on_blend_speed_valueChanged(double arg1);

    void on_fade_to_toggled(bool checked);

    void on_fade_from_toggled(bool checked);

    void on_color_textChanged(const QString &arg1);

    void on_fade_speed_valueChanged(double arg1);

    void on_slide_horizontal_toggled(bool checked);

    void on_slide_vertical_toggled(bool checked);

    void on_slide_speed_valueChanged(double arg1);

    void on_slide_reverse_toggled(bool checked);

    void on_pick_button_clicked();

    void on_slide_groupbox_toggled(bool arg1);

    void on_fade_groupbox_toggled(bool arg1);

    void on_curtain_h_toggled(bool checked);

    void on_curtain_v_toggled(bool checked);

    void on_curtain_reverse_h_toggled(bool checked);

    void on_curtain_reverse_v_toggled(bool checked);

    void on_curtain_speed_h_valueChanged(double arg1);

    void on_curtain_speed_v_valueChanged(double arg1);

private:
    Ui::BackgroundEditDialog *ui;

    Event *e;
    QHash<QString, QString> *backgrounds, *videos;
    bool ready = false;
};

#endif // BACKGROUNDEDITDIALOG_H
